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

#include "Locus/Rendering/Mesh.h"
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
const Locus::Key_t KEY_TRIANGULATE = Locus::Key_T;

TriangulationScene::TriangulationScene(Locus::SceneManager& sceneManager, unsigned int resolutionX, unsigned int resolutionY)
   : Scene(sceneManager),
     polygonColors({ Locus::Color::Red(), Locus::Color::Green(), Locus::Color::Blue() }),
     resolutionX(resolutionX),
     resolutionY(resolutionY),
     lastMouseX(0),
     lastMouseY(0)
{
   InitializeRenderingState();

   viewpoint.TranslateBy( Locus::FVector3(0.0f, 0.0f, REAL_Z_VIEWER) );

   currentPolygonAsLineSegments.CreateGPUVertexData();
}

void TriangulationScene::InitializeRenderingState()
{
   //throws an exception if GLSL can't be loaded, or if the requested GLSL version isn't supported
   renderingState = std::make_unique<Locus::RenderingState>(Locus::GLInfo::GLSLVersion::V_110, true);

   Locus::GLInfo::GLSLVersion activeGLSLVersion = renderingState->shaderController.GetActiveGLSLVersion();

   Locus::ID_t shaderProgramID = renderingState->shaderController.LoadShaderProgram(activeGLSLVersion, false, 0);

   renderingState->SetOpenGLStateToDefault();

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   renderingState->transformationStack.SetTransformationMode(Locus::TransformationStack::Projection);
   renderingState->transformationStack.Load( Locus::Transformation::Orthographic(-1, 1, -1, 1, Z_NEAR, Z_FAR) );

   renderingState->transformationStack.SetTransformationMode(Locus::TransformationStack::ModelView);
   renderingState->transformationStack.LoadIdentity();

   renderingState->shaderController.UseProgram(shaderProgramID);
}

void TriangulationScene::KeyPressed(Locus::Key_t key)
{
   switch (key)
   {
      case KEY_TRIANGULATE:
         TriangulateCompletedPolygons();
         break;
   }
}

void TriangulationScene::MouseMoved(int x, int y)
{
   if (!currentPolygonAsLineSegments.lineSegments.empty())
   {
      Locus::FVector3 worldCoordinate;

      if (Unproject(x, y, worldCoordinate))
      {
         worldCoordinate.z = REAL_Z_SCENE;

         currentPolygonAsLineSegments.lineSegments.back().segment.P2 = worldCoordinate;
         currentPolygonAsLineSegments.UpdateGPUVertexData();
      }
   }

   lastMouseX = x;
   lastMouseY = y;
}

void TriangulationScene::MousePressed(MouseButton_t button)
{
   Locus::FVector3 worldCoordinate;

   if (Unproject(lastMouseX, lastMouseY, worldCoordinate))
   {
      worldCoordinate.z = REAL_Z_SCENE;

      Locus::LineSegmentCollection::ColoredLineSegment coloredLineSegment;

      coloredLineSegment.color = CurrentColor();

      if (button == Locus::Mouse_Button_Left)
      {
         if (currentPolygonAsLineSegments.lineSegments.empty())
         {
            coloredLineSegment.segment.P1 = worldCoordinate;
         }
         else
         {
            coloredLineSegment.segment.P1 = currentPolygonAsLineSegments.lineSegments.back().segment.P2;
         }

         coloredLineSegment.segment.P2 = worldCoordinate;

         currentPolygonAsLineSegments.lineSegments.push_back(coloredLineSegment);
         currentPolygonAsLineSegments.UpdateGPUVertexData();
      }
      else
      {
         //a right or middle mouse button click is an attempt
         //to close the polygon
         if (currentPolygonAsLineSegments.lineSegments.size() >= 2)
         {
            coloredLineSegment.segment.P1 = worldCoordinate;
            coloredLineSegment.segment.P2 = currentPolygonAsLineSegments.lineSegments[0].segment.P1;

            currentPolygonAsLineSegments.lineSegments.push_back(coloredLineSegment);

            completedPolygonsAsLineSegments.emplace_back( std::make_unique<Locus::LineSegmentCollection>() );

            completedPolygonsAsLineSegments.back()->lineSegments = currentPolygonAsLineSegments.lineSegments;
            completedPolygonsAsLineSegments.back()->CreateGPUVertexData();
            completedPolygonsAsLineSegments.back()->UpdateGPUVertexData();

            currentPolygonAsLineSegments.lineSegments.clear();
            currentPolygonAsLineSegments.UpdateGPUVertexData();
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

bool TriangulationScene::Unproject(int x, int y, Locus::FVector3& worldCoordinate) const
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

Locus::Color TriangulationScene::CurrentColor() const
{
   return polygonColors[triangulatedPolygons.size() % polygonColors.size()];
}

void TriangulationScene::GetCompletedPolygonLineSegmentsAsPolygons(std::vector<Locus::Polygon2D_t>& polygons) const
{
   Locus::FVector2 point2D;

   for (const std::unique_ptr<Locus::LineSegmentCollection>& completedPolygon : completedPolygonsAsLineSegments)
   {
      Locus::Polygon2D_t polygon;

      for (const Locus::LineSegmentCollection::ColoredLineSegment& coloredLineSegment : completedPolygon->lineSegments)
      {
         const Locus::FVector3& point3D = coloredLineSegment.segment.P1;

         point2D.x = point3D.x;
         point2D.y = point3D.y;

         polygon.AddPoint(point2D);
      }

      polygons.push_back(polygon);
   }
}

bool TriangulationScene::CompletedPolygonsAreWellFormed(const std::vector<Locus::Polygon2D_t>& polygons)
{
   for (const Locus::Polygon2D_t& polygon : polygons)
   {
      if (!polygon.IsWellDefined())
      {
         return false;
      }

      //determine if any line segments are degenerate
      for (std::size_t pointIndex = 0, numPoints = polygon.NumPoints(); pointIndex < numPoints; ++pointIndex)
      {
         const Locus::FVector2& point1 = polygon[pointIndex];
         const Locus::FVector2& point2 = polygon[(pointIndex + 1) % numPoints];

         if (point1 == point2)
         {
            return false;
         }
      }

      if (polygon.IsSelfIntersecting())
      {
         return false;
      }
   }

   std::size_t numPolygons = polygons.size();

   if (numPolygons > 1)
   {
      //determine if any pair of polygons intersect
      //This is a brute force all pairs check

      for (std::size_t polygonIndex = 0; polygonIndex < (numPolygons - 1); ++polygonIndex)
      {
         for (std::size_t polygonIndex2 = (polygonIndex + 1); polygonIndex2 < numPolygons; ++polygonIndex2)
         {
            if (polygons[polygonIndex].Intersects(polygons[polygonIndex2]))
            {
               return false;
            }
         }
      }
   }

   return true;
}

void TriangulationScene::TriangulateCompletedPolygons()
{
   if (!completedPolygonsAsLineSegments.empty())
   {
      std::vector<Locus::Polygon2D_t> polygonsToTriangulate;

      GetCompletedPolygonLineSegmentsAsPolygons(polygonsToTriangulate);

      if (CompletedPolygonsAreWellFormed(polygonsToTriangulate))
      {
         std::vector<const Locus::FVector2*> trianglePoints;

         Locus::Triangulate(polygonsToTriangulate, Locus::PolygonWinding::CounterClockwise, trianglePoints);

         std::size_t numTriangles = trianglePoints.size() / 3;

         if (numTriangles > 0)
         {
            const Locus::TextureCoordinate Dont_Care_Tex_Coord(0.0f, 0.0f);

            std::vector<std::vector<Locus::MeshVertex>> faceTriangles(numTriangles, std::vector<Locus::MeshVertex>(3, Locus::MeshVertex(CurrentColor(), Dont_Care_Tex_Coord)));

            for (size_t triangleIndex = 0; triangleIndex < numTriangles; ++triangleIndex)
            {
               for (std::size_t pointIndex = 0; pointIndex < 3; ++pointIndex)
               {
                  Locus::MeshVertex& meshVertex = faceTriangles[triangleIndex][pointIndex];

                  meshVertex.position.x = trianglePoints[triangleIndex * 3 + pointIndex]->x;
                  meshVertex.position.y = trianglePoints[triangleIndex * 3 + pointIndex]->y;
                  meshVertex.position.z = REAL_Z_SCENE;
               }
            }

            std::unique_ptr<Locus::Mesh> polygonJustTriangulated = std::make_unique<Locus::Mesh>(faceTriangles);

            polygonJustTriangulated->gpuVertexDataTransferInfo.sendColors = true;
            polygonJustTriangulated->gpuVertexDataTransferInfo.sendNormals = false;
            polygonJustTriangulated->gpuVertexDataTransferInfo.sendPositions = true;
            polygonJustTriangulated->gpuVertexDataTransferInfo.sendTexCoords = false;

            polygonJustTriangulated->Translate(polygonJustTriangulated->centroid);

            polygonJustTriangulated->SetColor(CurrentColor());

            polygonJustTriangulated->CreateGPUVertexData();
            polygonJustTriangulated->UpdateGPUVertexData();

            triangulatedPolygons.push_back( std::move(polygonJustTriangulated) );
         }
      }

      currentPolygonAsLineSegments.lineSegments.clear();
      currentPolygonAsLineSegments.UpdateGPUVertexData();

      completedPolygonsAsLineSegments.clear();
   }
}

void TriangulationScene::Draw()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   viewpoint.Activate(renderingState->transformationStack);

   renderingState->transformationStack.UploadTransformations(renderingState->shaderController, Locus::Transformation::Identity());

   for (std::unique_ptr<Locus::LineSegmentCollection>& polygon : completedPolygonsAsLineSegments)
   {
      polygon->Draw(*renderingState);
   }

   currentPolygonAsLineSegments.Draw(*renderingState);

   for (std::unique_ptr<Locus::Mesh>& triangulatedPolygon : triangulatedPolygons)
   {
      renderingState->transformationStack.UploadTransformations(renderingState->shaderController, triangulatedPolygon->CurrentModelTransformation());
      triangulatedPolygon->Draw(*renderingState);
   }

   viewpoint.Deactivate(renderingState->transformationStack);
}

}

}