/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Triangulation.h"
#include "Locus/Geometry/PolygonHierarchy.h"

#include "EarClipper.h"

#include <queue>

namespace Locus
{

void Triangulate(const Polygon2D_t& polygon, std::vector<const FVector2*>& triangles)
{
   EarClipper(polygon).Triangulate(triangles);
}

void Triangulate(const Polygon2D_t& polygon, const std::vector<const Polygon2D_t*>& innerPolygons, std::vector<const FVector2*>& triangles)
{
   EarClipper(polygon, innerPolygons).Triangulate(triangles);
}

//{CodeReview:Triangulation}
void Triangulate(std::vector<Polygon2D_t>& polygons, PolygonWinding winding, std::vector<const FVector2*>& triangles)
{
   std::vector<Polygon2D_t*> polygonsForHierarchy;
   polygonsForHierarchy.reserve(polygons.size());

   for (Polygon2D_t& polygon : polygons)
   {
      polygonsForHierarchy.push_back(&polygon);
   }

   PolygonHierarchy<Polygon2D_t> polygonHierarchy(polygonsForHierarchy, winding, Vec3D::ZAxis(), EarClipper::EXPERIMENTAL_TOLERANCE);

   std::queue< const PolygonHierarchy<Polygon2D_t>::Node* > polygonNodesToTriangulate;

   for (const std::unique_ptr<PolygonHierarchy<Polygon2D_t>::Node>& node : polygonHierarchy.Root()->children)
   {
      polygonNodesToTriangulate.push(node.get());
   }

   while (!polygonNodesToTriangulate.empty())
   {
      const PolygonHierarchy<Polygon2D_t>::Node* node = polygonNodesToTriangulate.front();
      polygonNodesToTriangulate.pop();

      std::size_t numChildren = node->children.size();

      if (numChildren == 0)
      {
         //The outer polygon is a simple polygon with no nested inner polygons
         Triangulate(*(node->polygon), triangles);
      }
      else
      {
         std::vector<const Polygon2D_t*> innerPolygons;
         innerPolygons.reserve(numChildren);

         //The outer polygon contains inner polygons
         for (const std::unique_ptr<PolygonHierarchy<Polygon2D_t>::Node>& child : node->children)
         {
            const PolygonHierarchy<Polygon2D_t>::Node* innerNode = child.get();
            innerPolygons.push_back(innerNode->polygon);

            for (const std::unique_ptr<PolygonHierarchy<Polygon2D_t>::Node>& grandChild : innerNode->children)
            {
               polygonNodesToTriangulate.push(grandChild.get());
            }
         }

         EarClipper(*(node->polygon), innerPolygons).Triangulate(triangles);
      }
   }
}

}