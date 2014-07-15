/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "TriangulationScene.h"

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

//Environment Constants
const float FIELD_OF_VIEW = 30.0f;
const float Z_NEAR = 0.5f;
const float Z_FAR = 1.5f;

//Input Constants
const Locus::Key_t KEY_LEFT = Locus::Key_A;
const Locus::Key_t KEY_RIGHT = Locus::Key_F;
const Locus::Key_t KEY_UP = Locus::Key_E;
const Locus::Key_t KEY_DOWN = Locus::Key_D;

TriangulationScene::TriangulationScene(Locus::SceneManager& sceneManager, unsigned int resolutionX, unsigned int resolutionY)
   : Scene(sceneManager),
     resolutionX(resolutionX),
     resolutionY(resolutionY),
     lastMouseX(0),
     lastMouseY(0),
     moveViewerRight(false),
     moveViewerLeft(false),
     moveViewerUp(false),
     moveViewerDown(false)
{
   InitializeRenderingState();

   //Move the viewer back one unit so they will focus on
   //Z = 0 (this is where all the objects will be drawn
   //in this scene)
   
   viewpoint.TranslateBy( Locus::Vector3(0.0f, 0.0f, 1.0f) );

   pointCloud.CreateGPUVertexData();
}

TriangulationScene::~TriangulationScene()
{
   DestroyRenderingState();
}

void TriangulationScene::InitializeRenderingState()
{
   //throws an exception if GLSL can't be loaded, or if the requested GLSL version isn't supported
   renderingState.reset( new Locus::RenderingState(Locus::GLInfo::GLSLVersion::V_110, true) );

   Locus::GLInfo::GLSLVersion activeGLSLVersion = renderingState->shaderController.GetActiveGLSLVersion();

   const unsigned int Shader_ID = 0;

   renderingState->shaderController.LoadShaderProgram(Shader_ID, activeGLSLVersion, false, 0);

   renderingState->SetOpenGLStateToDefault();

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   renderingState->transformationStack.SetTransformationMode(Locus::TransformationStack::Projection);
   renderingState->transformationStack.Load( Locus::Transformation::Orthographic(-1, 1, -1, 1, Z_NEAR, Z_FAR) );

   renderingState->transformationStack.SetTransformationMode(Locus::TransformationStack::ModelView);
   renderingState->transformationStack.LoadIdentity();

   renderingState->shaderController.UseProgram(Shader_ID);
}

void TriangulationScene::DestroyRenderingState()
{
   renderingState.reset();

   pointCloud.DeleteGPUVertexData();
}

bool TriangulationScene::Unproject(int mouseX, int mouseY, Locus::Vector3& worldCoordinate) const
{
   const Locus::Transformation& modelView = viewpoint.GetTransformation().GetInverse();
   const Locus::Transformation& projection = renderingState->transformationStack.TopTransformation(Locus::TransformationStack::Projection);

   Locus::SquareMatrix<float> modelViewProjectionInverted = projection * modelView;

   if (!modelViewProjectionInverted.Invert())
   {
      return false;
   }

   std::vector<float> windowCoordinate{ ( 2.0f * (static_cast<float>(mouseX) / resolutionX) ) - 1.0f,
                                        ( 2.0f * (static_cast<float>(resolutionY - mouseY) / resolutionY) ) - 1.0f,
                                        0.0f,
                                        1.0f };

   std::vector<float> objectCoordinate = modelViewProjectionInverted * windowCoordinate;

   if (objectCoordinate[3] == 0.0f)
   {
      return false;
   }

   objectCoordinate[3] = (1.0f / objectCoordinate[3]);

   worldCoordinate.x = (objectCoordinate[0] * objectCoordinate[3]);
   worldCoordinate.y = (objectCoordinate[1] * objectCoordinate[3]);
   worldCoordinate.z = (objectCoordinate[2] * objectCoordinate[3]);

   return true;
}

void TriangulationScene::KeyPressed(Locus::Key_t key)
{
   switch (key)
   {
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

void TriangulationScene::KeyReleased(Locus::Key_t key)
{
   switch (key)
   {
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

void TriangulationScene::MouseMoved(int /*x*/, int /*y*/)
{
   UpdateLastMousePosition();
}

void TriangulationScene::MousePressed(MouseButton_t button)
{
   if (button == Locus::Mouse_Button_Left)
   {
      Locus::Vector3 worldCoordinate;

      if (Unproject(lastMouseX, lastMouseY, worldCoordinate))
      {
         worldCoordinate.z = 0;

         pointCloud.AddPosition(worldCoordinate, Locus::Color::White());
         pointCloud.UpdateGPUVertexData();
      }
   }
}

void TriangulationScene::Resized(int width, int height)
{
   glViewport(0, 0, width, height);

   resolutionX = width;
   resolutionY = height;
}

void TriangulationScene::Activate()
{
   sceneManager.CenterMouse();

   UpdateLastMousePosition();
}

void TriangulationScene::UpdateLastMousePosition()
{
   sceneManager.GetMousePosition(lastMouseX, lastMouseY);
}

bool TriangulationScene::Update(double DT)
{
   TickViewer(DT);

   return true;
}

void TriangulationScene::TickViewer(double DT)
{
   //translate the viewer based on the user's control key input

   Locus::Vector3 translation
   ( 
      (moveViewerLeft  ? -VIEWER_SPEED : 0.0f) + (moveViewerRight ?  VIEWER_SPEED : 0.0f),
      (moveViewerDown  ? -VIEWER_SPEED : 0.0f) + (moveViewerUp    ?  VIEWER_SPEED : 0.0f),
      0.0f
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
      //newPosition += -translation.z * viewpoint.GetForward();

      //move = ( (std::fabs(newPosition.x) <= VIEWER_BOUNDARY_SIZE) &&
      //         (std::fabs(newPosition.y) <= VIEWER_BOUNDARY_SIZE) &&
      //         (std::fabs(newPosition.z) <= VIEWER_BOUNDARY_SIZE) );
   }

   if (move)
   {
      viewpoint.TranslateBy(translation);
   }
}

void TriangulationScene::Draw()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   viewpoint.Activate(renderingState->transformationStack);

   renderingState->transformationStack.UploadTransformations(renderingState->shaderController, Locus::Transformation::Identity());
   pointCloud.Draw(*renderingState);

   viewpoint.Deactivate(renderingState->transformationStack);
}

}

}