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

extern const float BOUNDARY_SIZE = 130.0f;
extern const float VIEWER_BOUNDARY_SIZE = (3 * BOUNDARY_SIZE);

const float FIELD_OF_VIEW = 30.0f;
const float Z_NEAR = 0.01f;

//This is the length of the main diagonal of the scene. This is sqrt(8) times viewer boundary size squared. 
const float Z_FAR = (2.828427124746f * VIEWER_BOUNDARY_SIZE * VIEWER_BOUNDARY_SIZE);

const float VIEWER_SPEED = 80.0f;

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
   renderingState.reset( new Locus::RenderingState(Locus::GLInfo::GLSLVersion::V_110, true) );

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

   Locus::Mesh meshTemplates[Num_Templates] =
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
      collidableMeshTemplates[templateIndex].GrabMesh(meshTemplates[templateIndex]);

      collidableMeshTemplates[templateIndex].gpuVertexDataTransferInfo.sendColors = true;
      collidableMeshTemplates[templateIndex].gpuVertexDataTransferInfo.sendNormals = false;
      collidableMeshTemplates[templateIndex].gpuVertexDataTransferInfo.sendPositions = true;
      collidableMeshTemplates[templateIndex].gpuVertexDataTransferInfo.sendTexCoords = false;

      collidableMeshTemplates[templateIndex].CreateBoundingVolumeHierarchy();
   }

   const int Num_Colors = 3;

   const Locus::Color colors[Num_Colors] =
   {
      Locus::Color(255,  0 ,  0 , 255), //red
      Locus::Color(255, 255,  0 , 255), //yellow
      Locus::Color( 0 ,  0 , 255, 255)  //blue
   };

   collisionManager.StartAddRemoveBatch();

   const float maxDistance = BOUNDARY_SIZE - 5.0f;

   const std::size_t Num_Collidable_Meshes = 100;

   //units/second
   const float Min_Speed = 10.0f;
   const float Max_Speed = 20.0f;

   //radians/second
   const float Min_Rotation_Speed = 1.0f;
   const float Max_Rotation_Speed = 3.0f;

   const float Min_Scale = 5.0f;
   const float Max_Scale = 15.0f;

   collidableMeshes.resize(Num_Collidable_Meshes);

   std::size_t whichMesh = 0;

   Locus::Random random;

   for (std::size_t i = 0; i < Num_Collidable_Meshes; ++i)
   {
      collidableMeshes[i].GrabMeshAndCollidable(collidableMeshTemplates[whichMesh]);

      whichMesh = (whichMesh + 1) % Num_Templates;

      //randomize direction
      float xDirection = static_cast<float>(random.randomDouble(-1, 1));
      float yDirection = static_cast<float>(random.randomDouble(-1, 1));
      float zDirection = static_cast<float>(random.randomDouble(-1, 1));

      collidableMeshes[i].motionProperties.direction.set(xDirection, yDirection, zDirection);
      collidableMeshes[i].motionProperties.direction.normalize();

      //randomize speed
      collidableMeshes[i].motionProperties.speed = static_cast<float>(random.randomDouble(Min_Speed, Max_Speed));

      //randomize rotation direction
      xDirection = static_cast<float>(random.randomDouble(-1, 1));
      yDirection = static_cast<float>(random.randomDouble(-1, 1));
      zDirection = static_cast<float>(random.randomDouble(-1, 1));

      collidableMeshes[i].motionProperties.rotation.set(xDirection, yDirection, zDirection);

      //randomize rotation speed
      collidableMeshes[i].motionProperties.angularSpeed = static_cast<float>(random.randomDouble(Min_Rotation_Speed, Max_Rotation_Speed));

      //randomize size
      float scale = static_cast<float>(random.randomDouble(Min_Scale, Max_Scale));

      collidableMeshes[i].Scale( Locus::Vector3(scale, scale, scale) );

      //randomize position (centroid)
      Locus::Vector3 position;

      position.x = static_cast<float>(random.randomDouble(-maxDistance, maxDistance));
      position.y = static_cast<float>(random.randomDouble(-maxDistance, maxDistance));
      position.z = static_cast<float>(random.randomDouble(-maxDistance, maxDistance));

      collidableMeshes[i].Translate(position);

      //randomize color

      int colorIndex = random.randomInt(0, (Num_Colors - 1));

      collidableMeshes[i].SetColor(colors[colorIndex]);

      //finalize CollidableMesh
      collidableMeshes[i].CreateGPUVertexData();
      collidableMeshes[i].UpdateGPUVertexData();
      collidableMeshes[i].UpdateMaxDistanceToCenter();
      collidableMeshes[i].UpdateBroadCollisionExtent();

      collisionManager.Add(collidableMeshes[i]);
   }

   collisionManager.FinishAddRemoveBatch();
}

void CollisionScene::InitializeBoundary()
{
   const Locus::Color green(0, 255, 0, 255);

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

   Locus::LineCollection::line_t line;

   for (const std::pair<int, int>& linePointIndexPair : linePointIndices)
   {
      line.first = boundaryPoints[ linePointIndexPair.first ];
      line.second = boundaryPoints[ linePointIndexPair.second ];

      boundary.AddLine(line, green);
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
   Locus::Vector3 difference(static_cast<float>(x), static_cast<float>(y), 0.0f);
   Locus::Vector3 rotation((-difference.y/resolutionY) * FIELD_OF_VIEW * Locus::TO_RADIANS, (-difference.x/resolutionX)* FIELD_OF_VIEW * Locus::TO_RADIANS, 0.0f);

   viewpoint.RotateBy(rotation);

   sceneManager.CenterMouse();
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

      collisionManager.Update(collidableMesh);
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