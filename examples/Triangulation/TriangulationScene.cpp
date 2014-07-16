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
#include "Locus/Rendering/Mesh.h"
#include "Locus/Rendering/ShaderSourceStore.h"
#include "Locus/Rendering/RenderingState.h"
#include "Locus/Rendering/DrawUtility.h"

#include "Locus/Geometry/Triangulation.h"

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
const Locus::Key_t KEY_TRIANGULATE = Locus::Key_T;

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

   for (std::unique_ptr<Locus::Mesh>& triangulatedPolygon : triangulatedPolygons)
   {
      triangulatedPolygon->DeleteGPUVertexData();
   }
}

void TriangulationScene::KeyPressed(Locus::Key_t key)
{
   switch (key)
   {
      case KEY_TRIANGULATE:
         TriangulateCompletedPolygons();
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

      coloredLineSegment.color = CurrentColor();

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

Locus::Color TriangulationScene::CurrentColor() const
{
   return polygonColors[triangulatedPolygons.size() % polygonColors.size()];;
}

void TriangulationScene::TriangulateCompletedPolygons()
{
   if (completedPolygons.size() > 0)
   {
      std::vector<Locus::Polygon2D_t> polygonsToTriangulate;

      Locus::Vector2 point2D;

      for (std::unique_ptr<Locus::LineSegmentCollection>& completedPolygon : completedPolygons)
      {
         Locus::Polygon2D_t polygon;

         for (size_t lineSegmentIndex = 0, numLineSegments = completedPolygon->NumLineSegments(); lineSegmentIndex < numLineSegments; ++lineSegmentIndex)
         {
            const Locus::Vector3& point3D = (*completedPolygon)[lineSegmentIndex].segment.P2;

            point2D.x = point3D.x;
            point2D.y = point3D.y;

            polygon.AddPoint(point2D);
         }

         polygonsToTriangulate.push_back(polygon);
      }

      std::vector<const Vector2*> triangles;

      Locus::EarClipping::Triangulate(polygonsToTriangulate, Locus::PolygonWinding::CounterClockwise, triangles);

      std::size_t numTriangles = triangles.size() / 3;

      std::vector<std::vector<Locus::MeshVertex>> faceTriangles(numTriangles, std::vector<Locus::MeshVertex>(3));

      const Locus::TextureCoordinate Dont_Care_Tex_Coord(0.0f, 0.0f);

      for (size_t triangleIndex = 0; triangleIndex < numTriangles; ++triangleIndex)
      {
         for (std::size_t pointIndex = 0; pointIndex < 3; ++pointIndex)
         {
            Locus::MeshVertex& meshVertex = faceTriangles[triangleIndex][pointIndex];

            meshVertex.position.x = triangles[triangleIndex * 3 + pointIndex]->x;
            meshVertex.position.y = triangles[triangleIndex * 3 + pointIndex]->y;
            meshVertex.position.z = 0.0f;

            meshVertex.textureCoordinate = Dont_Care_Tex_Coord;
         }
      }

      Locus::Color color = CurrentColor();

      triangulatedPolygons.push_back( std::unique_ptr<Locus::Mesh>(new Mesh(faceTriangles)) );

      std::unique_ptr<Locus::Mesh>& polygonJustTriangulated = triangulatedPolygons.back();

      polygonJustTriangulated->gpuVertexDataTransferInfo.sendColors = true;
      polygonJustTriangulated->gpuVertexDataTransferInfo.sendNormals = false;
      polygonJustTriangulated->gpuVertexDataTransferInfo.sendPositions = true;
      polygonJustTriangulated->gpuVertexDataTransferInfo.sendTexCoords = false;

      polygonJustTriangulated->Translate(polygonJustTriangulated->centroid);

      polygonJustTriangulated->SetColor(color);

      polygonJustTriangulated->CreateGPUVertexData();
      polygonJustTriangulated->UpdateGPUVertexData();

      currentPolygon.Clear();
      currentPolygon.UpdateGPUVertexData();

      for (std::unique_ptr<Locus::LineSegmentCollection>& lineSegmentCollection : completedPolygons)
      {
         lineSegmentCollection->Clear();
         lineSegmentCollection->UpdateGPUVertexData();
      }

      completedPolygons.clear();
   }
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

   for (std::unique_ptr<Locus::Mesh>& triangulatedPolygon : triangulatedPolygons)
   {
      renderingState->transformationStack.UploadTransformations(renderingState->shaderController, triangulatedPolygon->CurrentModelTransformation());
      triangulatedPolygon->Draw(*renderingState);
   }

   viewpoint.Deactivate(renderingState->transformationStack);
}

}

}