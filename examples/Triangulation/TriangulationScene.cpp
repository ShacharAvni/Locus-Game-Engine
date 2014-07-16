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
#include "Locus/Rendering/DrawUtility.h"

#include "Locus/Simulation/UserEvents.h"
#include "Locus/Simulation/SceneManager.h"

#include <Locus/Rendering/Locus_glew.h>

#include <cmath>

namespace Locus
{

namespace Examples
{

//Environment Constants
const float FIELD_OF_VIEW = 30.0f;
const float Z_NEAR = 0.5f;
const float Z_FAR = 1.5f;
const float REAL_Z_VIEWER = 1.0f;
const float REAL_Z_SCENE = 0.0f;
const float DEPTH_OF_SCENE = ( (std::fabs(REAL_Z_SCENE - REAL_Z_VIEWER) - Z_NEAR) / (Z_FAR - Z_NEAR) );

//Input Constants
const Locus::Key_t KEY_LEFT = Locus::Key_A;
const Locus::Key_t KEY_RIGHT = Locus::Key_F;
const Locus::Key_t KEY_UP = Locus::Key_E;
const Locus::Key_t KEY_DOWN = Locus::Key_D;

TriangulationScene::TriangulationScene(Locus::SceneManager& sceneManager, unsigned int resolutionX, unsigned int resolutionY)
   : Scene(sceneManager),
     resolutionX(resolutionX),
     resolutionY(resolutionY),
     polygonColors({ Locus::Color::Red(), Locus::Color::Green(), Locus::Color::Blue() }),
     lastMouseX(0),
     lastMouseY(0),
     moveViewerRight(false),
     moveViewerLeft(false),
     moveViewerUp(false),
     moveViewerDown(false)
{
   InitializeRenderingState();
   
   viewpoint.TranslateBy( Locus::Vector3(0.0f, 0.0f, REAL_Z_VIEWER) );

   currentPolygon.CreateGPUVertexData();
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

   currentPolygon.DeleteGPUVertexData();

   for (std::unique_ptr<Locus::LineSegmentCollection>& lineSegmentCollection : completedPolygons)
   {
      lineSegmentCollection->DeleteGPUVertexData();
   }
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

void TriangulationScene::MouseMoved(int x, int y)
{
   if (currentPolygon.NumLineSegments() > 0)
   {
      Locus::Vector3 worldCoordinate;

      if (Unproject(x, y, worldCoordinate))
      {
         worldCoordinate.z = REAL_Z_SCENE;

         currentPolygon[currentPolygon.NumLineSegments() - 1].segment.P2 = worldCoordinate;
         currentPolygon.UpdateGPUVertexData();
      }
   }

   UpdateLastMousePosition();
}

void TriangulationScene::MousePressed(MouseButton_t button)
{
   Locus::Vector3 worldCoordinate;

   if (Unproject(lastMouseX, lastMouseY, worldCoordinate))
   {
      worldCoordinate.z = REAL_Z_SCENE;

      Locus::LineSegmentCollection::ColoredLineSegment coloredLineSegment;

      coloredLineSegment.color = polygonColors[completedPolygons.size() % polygonColors.size()];

      if (currentPolygon.NumLineSegments() == 0)
      {
         coloredLineSegment.segment.P1 = worldCoordinate;
      }
      else
      {
         coloredLineSegment.segment.P1 = currentPolygon[currentPolygon.NumLineSegments() - 1].segment.P2;
      }

      coloredLineSegment.segment.P2 = worldCoordinate;

      currentPolygon.AddLineSegment(coloredLineSegment);
      currentPolygon.UpdateGPUVertexData();

      if (button == Locus::Mouse_Button_Right)
      {
         if (currentPolygon.NumLineSegments() >= 3)
         {
            coloredLineSegment.segment.P1 = coloredLineSegment.segment.P2;
            coloredLineSegment.segment.P2 = currentPolygon[0].segment.P1;

            currentPolygon.AddLineSegment(coloredLineSegment);

            completedPolygons.push_back( std::unique_ptr<Locus::LineSegmentCollection>(new Locus::LineSegmentCollection(currentPolygon)) );
            completedPolygons.back()->CreateGPUVertexData();
            completedPolygons.back()->UpdateGPUVertexData();

            currentPolygon.Clear();
            currentPolygon.UpdateGPUVertexData();
         }
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

bool TriangulationScene::Unproject(int x, int y, Locus::Vector3& worldCoordinate) const
{
   const Locus::Transformation& modelView = viewpoint.GetTransformation().GetInverse();
   const Locus::Transformation& projection = renderingState->transformationStack.TopTransformation(Locus::TransformationStack::Projection);

   return Locus::DrawUtility::Unproject
   (
      static_cast<float>(x),
      static_cast<float>(y),
      DEPTH_OF_SCENE,
      modelView,
      projection,
      resolutionX,
      resolutionY,
      worldCoordinate
   );
}

void TriangulationScene::UpdateLastMousePosition()
{
   sceneManager.GetMousePosition(lastMouseX, lastMouseY);
}

void TriangulationScene::Draw()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   viewpoint.Activate(renderingState->transformationStack);

   renderingState->transformationStack.UploadTransformations(renderingState->shaderController, Locus::Transformation::Identity());

   for (std::unique_ptr<Locus::LineSegmentCollection>& polygon : completedPolygons)
   {
      polygon->Draw(*renderingState);
   }

   currentPolygon.Draw(*renderingState);

   viewpoint.Deactivate(renderingState->transformationStack);
}

}

}