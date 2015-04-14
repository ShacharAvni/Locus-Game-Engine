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

#include "LocusGeometryAPI.h"

#include "PolygonFwd.h"
#include "PolygonWinding.h"
#include "Geometry.h"
#include "Plane.h"
#include "LineSegment.h"
#include "Vector2Geometry.h"
#include "Vector3Geometry.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <type_traits>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

template <class PointType>
class LOCUS_GEOMETRY_API Polygon
{
public:
   static_assert(std::is_same<PointType, FVector2>::value || std::is_same<PointType, FVector3>::value, "PointType must be FVector3 or FVector2");

   Polygon();

   virtual bool IsWellDefined() const;
   virtual bool AddPoint(const PointType& point);

   std::size_t NumPoints() const;

   const PointType& operator[](std::size_t i) const;
   PointType& operator[](std::size_t i);

   PointType MidpointOfEdge(std::size_t i) const;

   PointType Centroid() const;
   const FVector3& Normal() const;
   Plane GetPlane() const;
   LineSegment<PointType> GetEdge(std::size_t i) const;

   void Reverse();
   PolygonWinding GetWinding(const FVector3& normalVector) const;

   bool IsSelfIntersecting() const;
   bool Intersects(const Polygon<PointType>& other) const;

   bool PointIsOnPolygon(const PointType& point, float toleranceFactor = 1) const;
   bool PointIsWithinPolygon(const PointType& point, float toleranceFactor = 1) const;

private:
   bool PointIsOnPolygon(const PointType& point, bool allowPointOnEdges, float toleranceFactor = 1) const;

   FVector3 CrossAtVertex(std::size_t vertexIndex) const;

protected:
   std::vector<PointType> points;
   std::size_t numPoints;
   FVector3 normal;

   void UpdateNormal();

   virtual bool PointIsOnPolygonCommonPath(const PointType& point, bool allowPointOnEdges, float toleranceFactor = 1) const;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

template <class T>
void ReorderCoplanarVertices(std::vector<T>& vertices, const FVector3& normal, PolygonWinding winding, const std::function<FVector3(const T&)>& getPositionFunc)
{
   std::size_t numVertices = vertices.size();

   if (numVertices > 2)
   {
      FVector3 verticesCentroid;
      for (const T& vertex : vertices)
      {
         verticesCentroid += getPositionFunc(vertex);
      }
      verticesCentroid = verticesCentroid / static_cast<float>(vertices.size());

      FVector3 firstVector = getPositionFunc(vertices[0]) - verticesCentroid;

      //get two orthogonal vectors
      FVector3 V1 = firstVector;
      Normalize(V1);

      FVector3 V2 = V1;
      RotateAround(V2, normal, PI/2);

      std::sort(vertices.begin() + 1, vertices.end(), [&](const T& first, const T& second)->bool
      {
         FVector3 vec1 = getPositionFunc(first) - verticesCentroid;
         float angleBetween1 = AngleWithXAxisRadians(Dot(vec1, V1), Dot(vec1, V2));

         FVector3 vec2 = getPositionFunc(second) - verticesCentroid;
         float angleBetween2 = AngleWithXAxisRadians(Dot(vec2, V1), Dot(vec2, V2));

         bool counterClockwiseOrder = (angleBetween1 < angleBetween2);

         return ( (winding == PolygonWinding::CounterClockwise) ?  counterClockwiseOrder : !counterClockwiseOrder );
      });
   }
}

#ifdef LOCUS_GEOMETRY_SHARED_IMPORTS

extern template class LOCUS_SHARED_IMPORTS Polygon<FVector3>;
extern template class LOCUS_SHARED_IMPORTS Polygon<FVector2>;

#endif

}