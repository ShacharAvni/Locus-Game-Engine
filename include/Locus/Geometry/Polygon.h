 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

#pragma once

#include "LocusGeometryAPI.h"

#include "PolygonFwd.h"
#include "PolygonWinding.h"
#include "Geometry.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Plane.h"
#include "LineSegment.h"

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
   static_assert(std::is_same<PointType, Vector2>::value || std::is_same<PointType, Vector3>::value, "PointType must be Vector3 or Vector2");

   Polygon();

   virtual bool IsWellDefined() const;
   virtual bool AddPoint(const PointType& point);

   std::size_t NumPoints() const;

   const PointType& operator[](std::size_t i) const;
   PointType& operator[](std::size_t i);

   PointType MidpointOfEdge(std::size_t i) const;

   PointType Centroid() const;
   const Vector3& Normal() const;
   Plane GetPlane() const;
   LineSegment<PointType> GetEdge(std::size_t i) const;

   void Reverse();
   PolygonWinding GetWinding(const Vector3& normalVector) const;

   bool PointIsOnPolygon(const PointType& point, float toleranceFactor = 1) const;
   bool PointIsWithinPolygon(const PointType& point, float toleranceFactor = 1) const;

private:
   bool PointIsOnPolygon(const PointType& point, bool allowPointOnEdges, float toleranceFactor = 1) const;

   Vector3 CrossAtVertex(std::size_t vertexIndex) const;

protected:
   std::vector<PointType> points;
   std::size_t numPoints;
   Vector3 normal;

   void UpdateNormal();

   virtual bool PointIsOnPolygonCommonPath(const PointType& point, bool allowPointOnEdges, float toleranceFactor = 1) const;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

template <class T>
void ReorderCoplanarVertices(std::vector<T>& vertices, const Vector3& normal, PolygonWinding winding, const std::function<Vector3(const T&)>& getPositionFunc)
{
   std::size_t numVertices = vertices.size();

   if (numVertices > 2)
   {
      Vector3 verticesCentroid;
      for (const T& vertex : vertices)
      {
         verticesCentroid += getPositionFunc(vertex);
      }
      verticesCentroid = verticesCentroid / static_cast<float>(vertices.size());

      Vector3 firstVector = getPositionFunc(vertices[0]) - verticesCentroid;

      //get two orthogonal vectors
      Vector3 V1 = firstVector;
      V1.normalize();
      Vector3 V2 = V1;
      V2.rotateAround(normal, PI/2);

      std::sort(vertices.begin() + 1, vertices.end(), [&](const T& first, const T& second)->bool
      {
         Vector3 vec1 = getPositionFunc(first) - verticesCentroid;
         float angleBetween1 = Vector2::AngleWithXAxisRadians(vec1.dot(V1), vec1.dot(V2));

         Vector3 vec2 = getPositionFunc(second) - verticesCentroid;
         float angleBetween2 = Vector2::AngleWithXAxisRadians(vec2.dot(V1), vec2.dot(V2));

         bool counterClockwiseOrder = (angleBetween1 < angleBetween2);

         return ( (winding == PolygonWinding::CounterClockwise) ?  counterClockwiseOrder : !counterClockwiseOrder );
      });
   }
}

#ifdef LOCUS_GEOMETRY_SHARED_IMPORTS

extern template class LOCUS_SHARED_IMPORTS Polygon<Vector3>;
extern template class LOCUS_SHARED_IMPORTS Polygon<Vector2>;

#endif

}