/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#pragma once

#include "Locus/Simulation/Scene.h"

#include "Locus/Geometry/Polygon.h"

#include "Locus/Rendering/Viewpoint.h"
#include "Locus/Rendering/LineSegmentCollection.h"

#include <memory>

namespace Locus
{

class SceneManager;
class RenderingState;
class Mesh;

namespace Examples
{

class TriangulationScene : public Locus::Scene
{
public:
   TriangulationScene(Locus::SceneManager& sceneManager, unsigned int resolutionX, unsigned int resolutionY);

   virtual void Draw() override;

   virtual void InitializeRenderingState() override;

   virtual void KeyPressed(Locus::Key_t key) override;

   virtual void MouseMoved(int x, int y) override;
   virtual void MousePressed(MouseButton_t button) override;

   virtual void Resized(int width, int height) override;

private:
   std::unique_ptr< Locus::RenderingState > renderingState;

   Locus::Viewpoint viewpoint;

   Locus::LineSegmentCollection currentPolygonAsLineSegments;

   std::vector< std::unique_ptr<Locus::LineSegmentCollection> > completedPolygonsAsLineSegments;

   std::vector<Locus::Color> polygonColors;

   std::vector< std::unique_ptr<Locus::Mesh> > triangulatedPolygons;

   unsigned int resolutionX;
   unsigned int resolutionY;

   int lastMouseX;
   int lastMouseY;

   bool Unproject(int x, int y, Locus::FVector3& worldCoordinate) const;

   Locus::Color CurrentColor() const;

   void GetCompletedPolygonLineSegmentsAsPolygons(std::vector<Locus::Polygon2D_t>& polygons) const;

   static bool CompletedPolygonsAreWellFormed(const std::vector<Locus::Polygon2D_t>& polygons);

   void TriangulateCompletedPolygons();
};

}

}