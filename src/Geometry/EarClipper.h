/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

//Triangulation by Ear Clipping
//David Eberly
//Geometric Tools, LLC
//http://www.geometrictools.com/
//http://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf

#include "Locus/Geometry/PolygonFwd.h"
#include "Locus/Geometry/PolygonWinding.h"

#include "Locus/Common/CircularList.h"

#include <vector>
#include <forward_list>

namespace Locus
{

class Vector2;
class Vector3;

//{CodeReview:Triangulation}
class EarClipper
{
public:
   EarClipper(const Polygon2D_t& polygon);
   EarClipper(const Polygon2D_t& polygon, const std::vector<const Polygon2D_t*>& innerPolygons);

   void Triangulate(std::vector<const Vector2*>& triangles);

   static const float EXPERIMENTAL_TOLERANCE;

private:
   enum class VertexType
   {
      Convex,
      Reflex,
      Ear
   };

   struct Vertex
   {
      Vertex(const Vector2* point)
         : point(point),
           type(VertexType::Reflex)
      {
      }

      const Vector2* point;
      VertexType type;
      std::list<const Vector2*> collinearPoints;
   };

   typedef CircularList<Vertex> VertexList;

   typedef std::forward_list< VertexList::iterator > VertexListIteratorList;

   void MakeSimple(const std::vector<const Polygon2D_t*>& innerPolygons, const Vector3& checkNormal);
   void StitchOuterAndInnerPolygons(const Polygon2D_t& hole, std::size_t maxInteriorPointIndex, VertexList::iterator mutuallyVisibleVertexIter);

   VertexList::iterator FindMutuallyVisibleVertex(const Vector2& maxInteriorPoint);
   const Polygon2D_t* FindMaxInteriorPointInListAndRemovePolygonFromConsideration(std::forward_list<const Polygon2D_t*>& innerPolygons, std::size_t& maxInteriorPointIndex);
   VertexList::iterator DetermineMutuallyVisibleVertexFromRayCastResult(const Vector2& maxInteriorPoint, const Vector2& intersectionPointOnEdge, VertexList::iterator pointOnEdgeWithMaximumXIter);
   VertexList::iterator RayCastFromMaxInteriorPointToOuterPolygon(const Vector2& maxInteriorPoint, Vector2& intersectionPointOnEdge, VertexList::iterator& pointOnEdgeWithMaximumXIter);

   void RemoveCollinearPointsFromConsideration();
   void AdjustForPossibleResultingCollinearity(VertexListIteratorList& ears, VertexList::iterator& beforeEar, VertexList::iterator& afterEar);
   void MigrateCollinearPoints(VertexList::iterator& to, const VertexList::iterator& from);

   void GetPointsStraddlingVertex(VertexList::const_iterator vertexIterator, const Vector2*& pointBefore, const Vector2*& pointAtVertex, const Vector2*& pointAfter);

   VertexType GetConvexOrReflexVertexType(const Vector2* pointBefore, const Vector2* pointOfInterest, const Vector2* pointAfter);
   VertexType GetConvexOrReflexVertexType(VertexList::const_iterator vertexIterator);
   void CheckForEarAndUpdateVertexType(VertexList::iterator vertexIterator);

   void RemoveEar(const Vector2* point, VertexListIteratorList& ears);
   void ReclassifyVertex(VertexList::iterator vertexIterator, VertexListIteratorList& ears);

   void Triangulate_R(VertexListIteratorList& ears, std::vector<const Vector2*>& triangles);

   void AddTriangle(std::vector<const Vector2*>& triangles, const VertexList::iterator& ear, bool last);
   void AddRemainingTriangles(std::vector<const Vector2*>& triangles);

   static void AddTriangle_R(std::vector<const Vector2*>& triangles,
                             const Vector2* trianglePoint1, std::list<const Vector2*>& collinearPoints1, 
                             const Vector2* trianglePoint2, std::list<const Vector2*>& collinearPoints2,
                             const Vector2* trianglePoint3, std::list<const Vector2*>& collinearPoints3);

   VertexList vertices;
   PolygonWinding polygonWinding;
};

}