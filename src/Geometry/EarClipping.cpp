/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "EarClipping.h"
#include "EarClippingImpl.h"

#include "Locus/Geometry/Polygon.h"

namespace Locus
{

namespace EarClipping
{

void Triangulate(const Polygon2D_t& polygon, std::vector<const Vector2*>& triangles)
{
   if (polygon.IsWellDefined())
   {
      VertexList vertices;

      for (std::size_t pointIndex = 0, numPoints = polygon.NumPoints(); pointIndex < numPoints; ++pointIndex)
      {
         vertices.push_back( Vertex(&polygon[pointIndex]) );
      }

      InternalTriangulate(vertices, polygon.GetWinding(Vector3::ZAxis()), triangles);
   }
}

//{CodeReview:Triangulation}
void Triangulate(const Polygon2D_t& polygon, const std::vector<const Polygon2D_t*>& innerPolygons, std::vector<const Vector2*>& triangles)
{
   if (polygon.IsWellDefined())
   {
      Vector3 checkNormal = Vector3::ZAxis();

      PolygonWinding polygonWinding = polygon.GetWinding(checkNormal);

      for (const Polygon2D_t* innerPolygon : innerPolygons)
      {
         if ((innerPolygon == nullptr) || !innerPolygon->IsWellDefined() || (innerPolygon->GetWinding(checkNormal) == polygonWinding))
         {
            return;
         }
      }

      VertexList vertices;

      for (std::size_t pointIndex = 0, numPoints = polygon.NumPoints(); pointIndex < numPoints; ++pointIndex)
      {
         vertices.push_back( Vertex(&polygon[pointIndex]) );
      }

      MakeSimple(vertices, innerPolygons, polygonWinding);

      InternalTriangulate(vertices, polygonWinding, triangles);
   }
}

}

}