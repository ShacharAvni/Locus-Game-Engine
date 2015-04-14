/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

// Implementation of Triangulation by Ear Clipping
// David Eberly
// Geometric Tools, LLC
// http://www.geometrictools.com/
// http://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf

#include "Locus/Geometry/PolygonFwd.h"
#include "Locus/Geometry/PolygonWinding.h"

#include "Locus/Math/VectorsFwd.h"

#include "Locus/Common/CircularList.h"

#include <vector>
#include <forward_list>

namespace Locus
{

//{CodeReview:Triangulation}
/// Internal class used by all overloads of Locus::Triangulate for triangulation via Ear Clipping.
class EarClipper
{
public:
   /// \param[in] polygon Should be planar and non self-intersecting.
   EarClipper(const Polygon2D_t& polygon);

   /*!
    * \param[in] polygon Should be planar and non self-intersecting.
    *
    * \param[in] innerPolygons Each inner polygon: (1) Should be completely
    * inside the polygon given as the other parameter. (2) Should not
    * intersect any other inner polygon. (3) Should not be inside any
    * other inner polygon. (4) Should have a winding order opposite
    * that of the polygon given as the other parameter. (5) Should be
    * planar. (6) Should be non self-intersecting.
    */
   EarClipper(const Polygon2D_t& polygon, const std::vector<const Polygon2D_t*>& innerPolygons);

   /*!
    * \brief Performs triangulation on the polygons given in the
    * constructor.
    *
    * \note The result is in the same winding order as the
    * outermost input polygon.
    *
    * \note The Vector2 pointers returned refer to points in the
    * input polygons. Therefore, this method should be called and
    * the result should be used before the input polygons go out
    * of scope.
    */ 
   void Triangulate(std::vector<const FVector2*>& triangles);

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
      Vertex(const FVector2* point)
         : point(point),
           type(VertexType::Reflex)
      {
      }

      const FVector2* point;
      VertexType type;
      std::list<const FVector2*> collinearPoints;
   };

   typedef CircularList<Vertex> VertexList;

   typedef std::forward_list< VertexList::iterator > VertexListIteratorList;

   void MakeSimple(const std::vector<const Polygon2D_t*>& innerPolygons, const FVector3& checkNormal);
   void StitchOuterAndInnerPolygons(const Polygon2D_t& hole, std::size_t maxInteriorPointIndex, VertexList::iterator mutuallyVisibleVertexIter);

   VertexList::iterator FindMutuallyVisibleVertex(const FVector2& maxInteriorPoint);
   const Polygon2D_t* FindMaxInteriorPointInListAndRemovePolygonFromConsideration(std::forward_list<const Polygon2D_t*>& innerPolygons, std::size_t& maxInteriorPointIndex);
   VertexList::iterator DetermineMutuallyVisibleVertexFromRayCastResult(const FVector2& maxInteriorPoint, const FVector2& intersectionPointOnEdge, VertexList::iterator pointOnEdgeWithMaximumXIter);
   VertexList::iterator RayCastFromMaxInteriorPointToOuterPolygon(const FVector2& maxInteriorPoint, FVector2& intersectionPointOnEdge, VertexList::iterator& pointOnEdgeWithMaximumXIter);

   void RemoveCollinearPointsFromConsideration();
   void AdjustForPossibleResultingCollinearity(VertexListIteratorList& ears, VertexList::iterator& beforeEar, VertexList::iterator& afterEar);
   void MigrateCollinearPoints(VertexList::iterator& to, const VertexList::iterator& from);

   void GetPointsStraddlingVertex(VertexList::const_iterator vertexIterator, const FVector2*& pointBefore, const FVector2*& pointAtVertex, const FVector2*& pointAfter);

   VertexType GetConvexOrReflexVertexType(const FVector2* pointBefore, const FVector2* pointOfInterest, const FVector2* pointAfter);
   VertexType GetConvexOrReflexVertexType(VertexList::const_iterator vertexIterator);
   void CheckForEarAndUpdateVertexType(VertexList::iterator vertexIterator);

   void RemoveEar(const FVector2* point, VertexListIteratorList& ears);
   void ReclassifyVertex(VertexList::iterator vertexIterator, VertexListIteratorList& ears);

   void Triangulate_R(VertexListIteratorList& ears, std::vector<const FVector2*>& triangles);

   void AddTriangle(std::vector<const FVector2*>& triangles, const VertexList::iterator& ear, bool last);
   void AddRemainingTriangles(std::vector<const FVector2*>& triangles);

   static void AddTriangle_R(std::vector<const FVector2*>& triangles,
                             const FVector2* trianglePoint1, std::list<const FVector2*>& collinearPoints1, 
                             const FVector2* trianglePoint2, std::list<const FVector2*>& collinearPoints2,
                             const FVector2* trianglePoint3, std::list<const FVector2*>& collinearPoints3);

   VertexList vertices;
   PolygonWinding polygonWinding;
};

}