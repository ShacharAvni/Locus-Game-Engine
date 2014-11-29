/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "CollisionScene.h"

#include "Locus/Common/Random.h"

#include "Locus/Rendering/MeshUtility.h"
#include "Locus/Rendering/ShaderSourceStore.h"
#include "Locus/Rendering/RenderingState.h"

#include "Locus/Simulation/UserEvents.h"
#include "Locus/Simulation/SceneManager.h"

#include <Locus/Rendering/Locus_glew.h>

#include <cmath>

namespace Locus
{

namespace Examples
{

//Simulation Constants
const float VIEWER_SPEED = 80.0f;

const std::size_t NUM_MESHES = 100;

const float MIN_MESH_SPEED = 10.0f; //units/second
const float MAX_MESH_SPEED = 20.0f;

const float MIN_MESH_ROTATION_SPEED = 1.0f; //radians/second
const float MAX_MESH_ROTATION_SPEED = 3.0f;

const float MIN_MESH_SCALE = 5.0f;
const float MAX_MESH_SCALE = 15.0f;

//Environment Constants
const float BOUNDARY_SIZE = 130.0f;
const float VIEWER_BOUNDARY_SIZE = (3 * BOUNDARY_SIZE);

const float FIELD_OF_VIEW = 30.0f;
const float Z_NEAR = 0.01f;

//This is the length of the main diagonal of the scene. The weird number is sqrt(8)
const float Z_FAR = (2.828427124746f * VIEWER_BOUNDARY_SIZE * VIEWER_BOUNDARY_SIZE);

//Input Constants
const Locus::Key_t KEY_FORWARD = Locus::Key_W;
const Locus::Key_t KEY_BACKWARD = Locus::Key_S;
const Locus::Key_t KEY_LEFT = Locus::Key_A;
const Locus::Key_t KEY_RIGHT = Locus::Key_F;
const Locus::Key_t KEY_UP = Locus::Key_E;
const Locus::Key_t KEY_DOWN = Locus::Key_D;
const Locus::Key_t KEY_PAUSE = Locus::Key_P;
const Locus::Key_t KEY_QUIT = Locus::Key_ESCAPE;

CollisionScene::CollisionScene(Locus::SceneManager& sceneManager, unsigned int resolutionX, unsigned int resolutionY)
   : Scene(sceneManager),
     dieOnNextFrame(false),
     paused(false),
     resolutionX(resolutionX),
     resolutionY(resolutionY),
     lastMouseX(0),
     lastMouseY(0),
     moveViewerAhead(false),
     moveViewerBack(false),
     moveViewerRight(false),
     moveViewerLeft(false),
     moveViewerUp(false),
     moveViewerDown(false)
{
   InitializeRenderingState();
   InitializeCollidableMeshes();
   InitializeBoundary();
}

CollisionScene::~CollisionScene()
{
   DestroyRenderingState();
}

void CollisionScene::InitializeRenderingState()
{
   //throws an exception if GLSL can't be loaded, or if the requested GLSL version isn't supported
   renderingState = std::make_unique<Locus::RenderingState>(Locus::GLInfo::GLSLVersion::V_110, true);

   Locus::GLInfo::GLSLVersion activeGLSLVersion = renderingState->shaderController.GetActiveGLSLVersion();

   const unsigned int Shader_ID = 0;

   renderingState->shaderController.LoadShaderProgram(Shader_ID, activeGLSLVersion, false, 0);

   renderingState->SetOpenGLStateToDefault();

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   renderingState->transformationStack.SetTransformationMode(Locus::TransformationStack::Projection);
   renderingState->transformationStack.Load( Locus::Transformation::Perspective(FIELD_OF_VIEW, static_cast<float>(resolutionX)/resolutionY, Z_NEAR, Z_FAR) );

   renderingState->transformationStack.SetTransformationMode(Locus::TransformationStack::ModelView);
   renderingState->transformationStack.LoadIdentity();

   renderingState->shaderController.UseProgram(Shader_ID);
}

void CollisionScene::DestroyRenderingState()
{
   renderingState.reset();

   for (CollidableMesh& collidableMesh : collidableMeshes)
   {
      collidableMesh.DeleteGPUVertexData();
   }

   boundary.DeleteGPUVertexData();
}

void CollisionScene::InitializeCollidableMeshes()
{
   const std::size_t Num_Templates = 5;

   std::unique_ptr<Locus::Mesh> meshTemplates[Num_Templates] =
   {
      MeshUtility::MakeCube(1.0f),
      MeshUtility::MakeIcosahedron(1.0f),
      MeshUtility::MakeOctahedron(1.0f),
      MeshUtility::MakeSphere(1.0f, 1),
      MeshUtility::MakeSphere(1.0f, 2)
   };

   CollidableMesh collidableMeshTemplates[Num_Templates];

   for (std::size_t templateIndex = 0; templateIndex < Num_Templates; ++templateIndex)
   {
      collidableMeshTemplates[templateIndex].GrabMesh(*meshTemplates[templateIndex]);

      collidableMeshTemplates[templateIndex].gpuVertexDataTransferInfo.sendColors = true;
      collidableMeshTemplates[templateIndex].gpuVertexDataTransferInfo.sendNormals = false;
      collidableMeshTemplates[templateIndex].gpuVertexDataTransferInfo.sendPositions = true;
      collidableMeshTemplates[templateIndex].gpuVertexDataTransferInfo.sendTexCoords = false;

      collidableMeshTemplates[templateIndex].CreateBoundingVolumeHierarchy();
   }

   const int Num_Colors = 3;

   const Locus::Color colors[Num_Colors] =
   {
      Locus::Color::Red(),
      Locus::Color::Yellow(),
      Locus::Color::Blue()
   };

   collisionManager.StartAddRemoveBatch();

   float maxDistance = BOUNDARY_SIZE - 5.0f;

   collidableMeshes.resize(NUM_MESHES);

   std::size_t whichMesh = 0;

   Locus::Random random;

   for (std::size_t i = 0; i < NUM_MESHES; ++i)
   {
      collidableMeshes[i].GrabMeshAndCollidable(collidableMeshTemplates[whichMesh]);

      whichMesh = (whichMesh + 1) % Num_Templates;

      //randomize direction
      float xDirection = static_cast<float>(random.RandomDouble(-1, 1));
      float yDirection = static_cast<float>(random.RandomDouble(-1, 1));
      float zDirection = static_cast<float>(random.RandomDouble(-1, 1));

      collidableMeshes[i].motionProperties.direction.set(xDirection, yDirection, zDirection);
      collidableMeshes[i].motionProperties.direction.normalize();

      //randomize speed
      collidableMeshes[i].motionProperties.speed = static_cast<float>(random.RandomDouble(MIN_MESH_SPEED, MAX_MESH_SPEED));

      //randomize rotation direction
      xDirection = static_cast<float>(random.RandomDouble(-1, 1));
      yDirection = static_cast<float>(random.RandomDouble(-1, 1));
      zDirection = static_cast<float>(random.RandomDouble(-1, 1));

      collidableMeshes[i].motionProperties.rotation.set(xDirection, yDirection, zDirection);

      //randomize rotation speed
      collidableMeshes[i].motionProperties.angularSpeed = static_cast<float>(random.RandomDouble(MIN_MESH_ROTATION_SPEED, MAX_MESH_ROTATION_SPEED));

      //randomize size
      float scale = static_cast<float>(random.RandomDouble(MIN_MESH_SCALE, MAX_MESH_SCALE));

      collidableMeshes[i].Scale( Locus::Vector3(scale, scale, scale) );

      //randomize position (centroid)
      Locus::Vector3 position;

      position.x = static_cast<float>(random.RandomDouble(-maxDistance, maxDistance));
      position.y = static_cast<float>(random.RandomDouble(-maxDistance, maxDistance));
      position.z = static_cast<float>(random.RandomDouble(-maxDistance, maxDistance));

      collidableMeshes[i].Translate(position);

      //randomize color

      int colorIndex = random.RandomInt(0, (Num_Colors - 1));

      collidableMeshes[i].SetColor(colors[colorIndex]);

      //finalize CollidableMesh
      collidableMeshes[i].CreateGPUVertexData();
      collidableMeshes[i].UpdateGPUVertexData();
      collidableMeshes[i].UpdateMaxDistanceToCenter();
      collidableMeshes[i].UpdateBroadCollisionExtent();

      collisionManager.Add(&collidableMeshes[i]);
   }

   collisionManager.FinishAddRemoveBatch();
}

void CollisionScene::InitializeBoundary()
{
   const Vector3 boundaryPoints[8] =
   {
      {-BOUNDARY_SIZE, -BOUNDARY_SIZE, -BOUNDARY_SIZE},
      { BOUNDARY_SIZE, -BOUNDARY_SIZE, -BOUNDARY_SIZE},
      { BOUNDARY_SIZE,  BOUNDARY_SIZE, -BOUNDARY_SIZE},
      {-BOUNDARY_SIZE,  BOUNDARY_SIZE, -BOUNDARY_SIZE},
      {-BOUNDARY_SIZE, -BOUNDARY_SIZE,  BOUNDARY_SIZE},
      { BOUNDARY_SIZE, -BOUNDARY_SIZE,  BOUNDARY_SIZE},
      { BOUNDARY_SIZE,  BOUNDARY_SIZE,  BOUNDARY_SIZE},
      {-BOUNDARY_SIZE,  BOUNDARY_SIZE,  BOUNDARY_SIZE}
   };

   const std::pair<int, int> linePointIndices[12] =
   {
      //bottom quad
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},

      //top quad
      {4, 5},
      {5, 6},
      {6, 7},
      {7, 4},

      //lines connecting bottom and top quads
      {0, 4},
      {1, 5},
      {2, 6},
      {3, 7}
   };

   Locus::LineSegmentCollection::ColoredLineSegment coloredLineSegment;

   for (const std::pair<int, int>& linePointIndexPair : linePointIndices)
   {
      coloredLineSegment.segment.P1 = boundaryPoints[ linePointIndexPair.first ];
      coloredLineSegment.segment.P2 = boundaryPoints[ linePointIndexPair.second ];

      coloredLineSegment.color = Locus::Color::Green();

      boundary.AddLineSegment(coloredLineSegment);
   }

   boundary.CreateGPUVertexData();
   boundary.UpdateGPUVertexData();
}

void CollisionScene::KeyPressed(Locus::Key_t key)
{
   switch (key)
   {
      case KEY_QUIT:
         dieOnNextFrame = true;
         break;

      case KEY_PAUSE:
         paused = !paused;
         break;

      case KEY_FORWARD:
         moveViewerAhead = true;
         break;

      case KEY_BACKWARD:
         moveViewerBack = true;
         break;

      case KEY_LEFT:
         moveViewerLeft = true;
         break;

      case KEY_RIGHT:
         moveViewerRight = true;
         break;

      case KEY_UP:
         moveViewerUp = true;
         break;

      case KEY_DOWN:
         moveViewerDown = true;
         break;
   }
}

void CollisionScene::KeyReleased(Locus::Key_t key)
{
   switch (key)
   {
      case KEY_FORWARD:
         moveViewerAhead = false;
         break;

      case KEY_BACKWARD:
         moveViewerBack = false;
         break;

      case KEY_LEFT:
         moveViewerLeft = false;
         break;

      case KEY_RIGHT:
         moveViewerRight = false;
         break;

      case KEY_UP:
         moveViewerUp = false;
         break;

      case KEY_DOWN:
         moveViewerDown = false;
         break;
   }
}

void CollisionScene::MouseMoved(int x, int y)
{
   int diffX = x - lastMouseX;
   int diffY = y - lastMouseY;

   Locus::Vector3 difference(static_cast<float>(diffX), static_cast<float>(diffY), 0.0f);

   Locus::Vector3 rotation((-difference.y/resolutionY) * FIELD_OF_VIEW * Locus::TO_RADIANS, (-difference.x/resolutionX)* FIELD_OF_VIEW * Locus::TO_RADIANS, 0.0f);

   viewpoint.RotateBy(rotation);

   sceneManager.CenterMouse();

   UpdateLastMousePosition();
}

void CollisionScene::Resized(int width, int height)
{
   glViewport(0, 0, width, height);

   resolutionX = width;
   resolutionY = height;
}

void CollisionScene::Activate()
{
   sceneManager.HideMouse();
   sceneManager.CenterMouse();

   UpdateLastMousePosition();
}

void CollisionScene::UpdateLastMousePosition()
{
   sceneManager.GetMousePosition(lastMouseX, lastMouseY);
}

bool CollisionScene::Update(double DT)
{
   if (dieOnNextFrame)
   {
      return false;
   }

   TickViewer(DT);

   if (!paused)
   {
      TickCollidableMeshes(DT);

      collisionManager.UpdateCollisions();
      collisionManager.TransmitCollisions();
   }

   return true;
}

void CollisionScene::TickCollidableMeshes(double DT)
{
   //this function updates all the collidable mesh positions. If a colidable mesh
   //is about to go beyond the boundary, it bounces off the side.

   for (CollidableMesh& collidableMesh : collidableMeshes)
   {
      Locus::Vector3 nextPosition = collidableMesh.Position() + ((collidableMesh.motionProperties.speed * collidableMesh.motionProperties.direction) * static_cast<float>(DT));

      if (std::fabs(nextPosition.x) >= BOUNDARY_SIZE)
      {
         collidableMesh.motionProperties.direction.x *= -1;
      }

      if (std::fabs(nextPosition.y) >= BOUNDARY_SIZE)
      {
         collidableMesh.motionProperties.direction.y *= -1;
      }

      if (std::fabs(nextPosition.z) >= BOUNDARY_SIZE)
      {
         collidableMesh.motionProperties.direction.z *= -1;
      }

      collidableMesh.Tick(DT);
      collidableMesh.UpdateBroadCollisionExtent();

      collisionManager.Update(&collidableMesh);
   }
}

void CollisionScene::TickViewer(double DT)
{
   //translate the viewer based on the user's control key input
   //rotation is done directly when the game responds to the
   //user's mouse movements

   Locus::Vector3 translation
   ( 
      (moveViewerLeft  ? -VIEWER_SPEED : 0.0f) + (moveViewerRight ?  VIEWER_SPEED : 0.0f),
      (moveViewerDown  ? -VIEWER_SPEED : 0.0f) + (moveViewerUp    ?  VIEWER_SPEED : 0.0f),
      (moveViewerAhead ? -VIEWER_SPEED : 0.0f) + (moveViewerBack  ?  VIEWER_SPEED : 0.0f)
   );

   bool move = false;

   Locus::Vector3 originalPosition = viewpoint.GetPosition();
   Locus::Vector3 newPosition;

   if (!translation.ApproximatelyEqualTo(Locus::Vector3::ZeroVector()))
   {
      translation *= static_cast<float>(DT);

      newPosition = originalPosition;

      newPosition += translation.x * viewpoint.GetRight();
      newPosition += translation.y * viewpoint.GetUp();
      newPosition += -translation.z * viewpoint.GetForward();

      move = ( (std::fabs(newPosition.x) <= VIEWER_BOUNDARY_SIZE) &&
               (std::fabs(newPosition.y) <= VIEWER_BOUNDARY_SIZE) &&
               (std::fabs(newPosition.z) <= VIEWER_BOUNDARY_SIZE) );
   }

   if (move)
   {
      viewpoint.TranslateBy(translation);
   }
}

void CollisionScene::Draw()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   viewpoint.Activate(renderingState->transformationStack);

   for (const CollidableMesh& collidableMesh : collidableMeshes)
   {
      renderingState->transformationStack.UploadTransformations(renderingState->shaderController, collidableMesh.CurrentModelTransformation());
      collidableMesh.Draw(*renderingState);
   }

   renderingState->transformationStack.UploadTransformations(renderingState->shaderController, Locus::Transformation::Identity());
   boundary.Draw(*renderingState);

   viewpoint.Deactivate(renderingState->transformationStack);
}

}

}