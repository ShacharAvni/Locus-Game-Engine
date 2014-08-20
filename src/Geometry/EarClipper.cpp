/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "EarClipper.h"

#include "Locus/Geometry/Polygon.h"
#include "Locus/Geometry/Triangle.h"

#include "Locus/Common/Float.h"

#include <cassert>

namespace Locus
{

const float EarClipper::EXPERIMENTAL_TOLERANCE = 1.0f;

static const float COLLINEAR_TOLERANCE = 1.0f;

EarClipper::EarClipper(const Polygon2D_t& polygon)
{
   if (polygon.IsWellDefined())
   {
      for (std::size_t pointIndex = 0, numPoints = polygon.NumPoints(); pointIndex < numPoints; ++pointIndex)
      {
         vertices.push_back( Vertex(&polygon[pointIndex]) );
      }

      polygonWinding = polygon.GetWinding(Vector3::ZAxis());
   }
}

//{CodeReview:Triangulation}
EarClipper::EarClipper(const Polygon2D_t& polygon, const std::vector<const Polygon2D_t*>& innerPolygons)
{
   if (polygon.IsWellDefined())
   {
      Vector3 checkNormal = Vector3::ZAxis();

      polygonWinding = polygon.GetWinding(checkNormal);

      for (std::size_t pointIndex = 0, numPoints = polygon.NumPoints(); pointIndex < numPoints; ++pointIndex)
      {
         vertices.push_back( Vertex(&polygon[pointIndex]) );
      }

      MakeSimple(innerPolygons, checkNormal);
   }
}

//{CodeReview:Triangulation}
void EarClipper::Triangulate(std::vector<const Vector2*>& triangles)
{
   triangles.reserve( triangles.size() + (3 * (vertices.size() - 2)) );

   RemoveCollinearPointsFromConsideration();

   if (vertices.size() >= 3)
   {
      VertexListIteratorList ears;

      VertexList::iterator verticesEnd = vertices.end();

      for (VertexList::iterator vertexIterator = vertices.begin(); vertexIterator != verticesEnd; ++vertexIterator)
      {
         vertexIterator->type = GetConvexOrReflexVertexType(vertexIterator);
      }

      for (VertexList::iterator vertexIterator = vertices.begin(); vertexIterator != verticesEnd; ++vertexIterator)
      {
         CheckForEarAndUpdateVertexType(vertexIterator);

         if (vertexIterator->type == VertexType::Ear)
         {
            ears.push_front(vertexIterator);
         }
      }

      Triangulate_R(ears, triangles);
   }
}

const Polygon2D_t* EarClipper::FindMaxInteriorPointInListAndRemovePolygonFromConsideration(std::forward_list<const Polygon2D_t*>& innerPolygons, std::size_t& maxInteriorPointIndex)
{
   std::forward_list<const Polygon2D_t*>::iterator polygonIteratorToErase = innerPolygons.before_begin();

   const Polygon2D_t* polygonWithMaxX = innerPolygons.front();
   float maxX = (*polygonWithMaxX)[0].x;
   maxInteriorPointIndex = 0;

   for (std::forward_list<const Polygon2D_t*>::iterator polygonIter = innerPolygons.begin(), previousIter = innerPolygons.before_begin(), end = innerPolygons.end(); polygonIter != end; ++polygonIter, ++previousIter)
   {
      const Polygon2D_t* polygon = *polygonIter;

      std::size_t holeSize = polygon->NumPoints();

      for (std::size_t holeVertexIndex = 0; holeVertexIndex < holeSize; ++holeVertexIndex)
      {
         if ((*polygon)[holeVertexIndex].x > maxX)
         {
            polygonWithMaxX = polygon;
            polygonIteratorToErase = previousIter;
            maxX = (*polygon)[holeVertexIndex].x;
            maxInteriorPointIndex = holeVertexIndex;
         }
      }
   }

   innerPolygons.erase_after(polygonIteratorToErase);

   return polygonWithMaxX;
}

EarClipper::VertexList::iterator EarClipper::RayCastFromMaxInteriorPointToOuterPolygon(const Vector2& maxInteriorPoint, Vector2& intersectionPointOnEdge, VertexList::iterator& pointOnEdgeWithMaximumXIter)
{
   //a) Call the vertex on the hole with the maximum x coordinate P. Call the ray { P, (1,0) } R. Intersect R
   //   with the edges of the outer polygon. Find the edge whose intersection is closest to P. Call this edge E.
   //   If R goes through E entirely, then the mutually visible vertex is the endpoint on E closest to P. Otherwise,
   //   if R goes through one of the endpoints of E, then the mutually visible vertex is this endpoint. Otherwise,
   //   R intersects the line segment at a point other than the endpoints (see b)

   VertexList::iterator verticesEnd = vertices.end();

   VertexList::iterator mutuallyVisibleVertexIter = verticesEnd;

   VertexList::iterator possibleMutuallyVisibleVertex = verticesEnd;

   Line2D_t rayFromMaxVertex(maxInteriorPoint, Vector2::XAxis(), true);

   float minDistance = std::numeric_limits<float>::max();

   LineSegment2D_t lineSegmentOnOuterPolygon;
   Vector2 intersectionPoint1, intersectionPoint2;
   VertexList::iterator pointOnThisEdgeWithMaximumXIter = verticesEnd;

   for (VertexList::iterator vertexIterator = vertices.begin(); vertexIterator != verticesEnd; ++vertexIterator)
   {
      VertexList::iterator nextVertexIterator = vertexIterator.next();

      lineSegmentOnOuterPolygon.P1 = *(vertexIterator->point);
      lineSegmentOnOuterPolygon.P2 = *(nextVertexIterator->point);

      IntersectionType intersection = rayFromMaxVertex.LineSegmentIntersection(lineSegmentOnOuterPolygon, intersectionPoint1, intersectionPoint2, EXPERIMENTAL_TOLERANCE);

      if ((intersection == IntersectionType::LineSegment) || (intersection == IntersectionType::Point))
      {
         possibleMutuallyVisibleVertex = vertexIterator;
         bool rayHitEndPointOfEdge = true;

         if (lineSegmentOnOuterPolygon.P1.x > lineSegmentOnOuterPolygon.P2.x)
         {
            pointOnThisEdgeWithMaximumXIter = vertexIterator;
         }
         else
         {
            pointOnThisEdgeWithMaximumXIter = nextVertexIterator;
         }

         if (intersection == IntersectionType::LineSegment)
         {
            if (lineSegmentOnOuterPolygon.P1.x < lineSegmentOnOuterPolygon.P2.x)
            {
               possibleMutuallyVisibleVertex = vertexIterator;
            }
            else
            {
               possibleMutuallyVisibleVertex = nextVertexIterator;
            }
         }
         else if (intersection == IntersectionType::Point)
         {
            if (intersectionPoint1.PreciselyEqualTo(lineSegmentOnOuterPolygon.P1))
            {
               possibleMutuallyVisibleVertex = vertexIterator;
            }
            else if (intersectionPoint1.PreciselyEqualTo(lineSegmentOnOuterPolygon.P2))
            {
               possibleMutuallyVisibleVertex = nextVertexIterator;
            }
            else
            {
               rayHitEndPointOfEdge = false;
            }
         }

         float distance = rayHitEndPointOfEdge ? (possibleMutuallyVisibleVertex->point->x - maxInteriorPoint.x) : (intersectionPoint1.x - maxInteriorPoint.x);

         bool sameishDistance = Float::Equal(distance, minDistance);

         if ((distance < minDistance) || sameishDistance)
         {
            bool takeThisVertex = true;

            if (sameishDistance)
            {
               const Vector2* beforeMutuallyVisibleVertex = possibleMutuallyVisibleVertex.previous()->point;

               takeThisVertex = (GetConvexOrReflexVertexType(beforeMutuallyVisibleVertex, possibleMutuallyVisibleVertex->point, &maxInteriorPoint) == VertexType::Convex);
            }

            if (takeThisVertex)
            {
               if (rayHitEndPointOfEdge)
               {
                  mutuallyVisibleVertexIter = possibleMutuallyVisibleVertex;
               }
               else
               {
                  mutuallyVisibleVertexIter = verticesEnd;
                  pointOnEdgeWithMaximumXIter = pointOnThisEdgeWithMaximumXIter;
                  intersectionPointOnEdge = intersectionPoint1;
               }

               minDistance = distance;
            }
         }
      }
   }

   return mutuallyVisibleVertexIter;
}

EarClipper::VertexList::iterator EarClipper::DetermineMutuallyVisibleVertexFromRayCastResult(const Vector2& maxInteriorPoint, const Vector2& intersectionPointOnEdge, VertexList::iterator pointOnEdgeWithMaximumXIter)
{
   VertexList::iterator verticesEnd = vertices.end();

   VertexList::iterator mutuallyVisibleVertexIter = verticesEnd;

   // Form a triangle with P (see explanation of a) above) (P1), the intersection point on
   // the edge (P2) and the point on the edge with maximum X (P3).
   //
   // If no points on the outer polygon fall on this triangle, then P3 is the visible vertex.
   //
   // Otherwise, the reflex vertex of the outer polygon that falls into this triangle and
   // minimizes the angle to the ray (i.e. x axis) is the mutually visible vertex

   Triangle2D_t checkTriangle(maxInteriorPoint, intersectionPointOnEdge, *(pointOnEdgeWithMaximumXIter->point));

   VertexListIteratorList reflexVerticesOnTriangle;
   bool hasAnyReflexVerticesOnTriangle = false;

   for (VertexList::iterator vertexIterator = vertices.begin(); vertexIterator != verticesEnd; ++vertexIterator)
   {
      if (checkTriangle.PointIsOnPolygon(*(vertexIterator->point), EXPERIMENTAL_TOLERANCE))
      {
         if (GetConvexOrReflexVertexType(vertexIterator) == VertexType::Reflex)
         {
            reflexVerticesOnTriangle.push_front(vertexIterator);
            hasAnyReflexVerticesOnTriangle = true;
         }
      }
   }

   if (hasAnyReflexVerticesOnTriangle)
   {
      mutuallyVisibleVertexIter = reflexVerticesOnTriangle.front();
      reflexVerticesOnTriangle.pop_front();

      float minDistance = mutuallyVisibleVertexIter->point->distanceTo(maxInteriorPoint);
      float minAngle = (*(mutuallyVisibleVertexIter->point) - maxInteriorPoint).angleBetweenRadians( Vector2::XAxis() );

      for (const VertexList::iterator& reflexVertexIter : reflexVerticesOnTriangle)
      {
         float angle = (*(reflexVertexIter->point) - maxInteriorPoint).angleBetweenRadians( Vector2::XAxis() );
         float distance = reflexVertexIter->point->distanceTo(maxInteriorPoint);

         if (angle < minAngle)
         {
            minAngle = angle;
            mutuallyVisibleVertexIter = reflexVertexIter;

            minDistance = distance;
         }
         else if (angle == minAngle)
         {
            if (distance < minDistance)
            {
               minAngle = angle;
               mutuallyVisibleVertexIter = reflexVertexIter;

               minDistance = distance;
            }
         }
      }
   }
   else
   {
      mutuallyVisibleVertexIter = pointOnEdgeWithMaximumXIter;
   }

   return mutuallyVisibleVertexIter;
}

EarClipper::VertexList::iterator EarClipper::FindMutuallyVisibleVertex(const Vector2& maxInteriorPoint)
{
   // Find a vertex on the outer polygon visible to that vertex

   Vector2 intersectionPointOnEdge;
   VertexList::iterator pointOnEdgeWithMaximumXIter = vertices.end();

   VertexList::iterator mutuallyVisibleVertex = RayCastFromMaxInteriorPointToOuterPolygon(maxInteriorPoint, intersectionPointOnEdge, pointOnEdgeWithMaximumXIter);

   if (mutuallyVisibleVertex == vertices.end())
   {
      mutuallyVisibleVertex = DetermineMutuallyVisibleVertexFromRayCastResult(maxInteriorPoint, intersectionPointOnEdge, pointOnEdgeWithMaximumXIter);
   }

   return mutuallyVisibleVertex;
}

void EarClipper::StitchOuterAndInnerPolygons(const Polygon2D_t& hole, std::size_t maxInteriorPointIndex, VertexList::iterator mutuallyVisibleVertexIter)
{
   // Attach hole to outer vertices through the mutually visible vertex

   VertexList::iterator insertIterator = mutuallyVisibleVertexIter.next();

   for (std::size_t holeVertex = 1, holeVertexIndex = maxInteriorPointIndex, holeSize = hole.NumPoints(); holeVertex <= holeSize + 1; ++holeVertex, holeVertexIndex = (holeVertexIndex + 1) % holeSize)
   {
      vertices.insert(insertIterator, Vertex(&hole[holeVertexIndex]));
   }

   vertices.insert(insertIterator, Vertex(mutuallyVisibleVertexIter->point));
}

void EarClipper::MakeSimple(const std::vector<const Polygon2D_t*>& innerPolygons, const Vector3& checkNormal)
{
   std::forward_list<const Polygon2D_t*> innerPolygonsRemaining;
   for (const Polygon2D_t* innerPolygon : innerPolygons)
   {
      if ((innerPolygon == nullptr) || !innerPolygon->IsWellDefined() || (innerPolygon->GetWinding(checkNormal) == polygonWinding))
      {
         //If any inner polygons are not well defined or are not the appropriate
         //winding, then only the outer polygon will be triangulated.
         return;
      }

      innerPolygonsRemaining.push_front(innerPolygon);
   }

   for (std::size_t innerPolygonIndex = 0, numInnerPolygons = innerPolygons.size(); innerPolygonIndex < numInnerPolygons; ++innerPolygonIndex)
   {
      std::size_t maxInteriorPointIndex;
      const Polygon2D_t* innerPolygon = FindMaxInteriorPointInListAndRemovePolygonFromConsideration(innerPolygonsRemaining, maxInteriorPointIndex);

      VertexList::iterator mutuallyVisibleVertex = FindMutuallyVisibleVertex((*innerPolygon)[maxInteriorPointIndex]);

      assert(mutuallyVisibleVertex != vertices.end());

      StitchOuterAndInnerPolygons(*innerPolygon, maxInteriorPointIndex, mutuallyVisibleVertex);
   }
}

void EarClipper::MigrateCollinearPoints(VertexList::iterator& to, const VertexList::iterator& from)
{
   for (const Vector2* collinearPoint : from->collinearPoints)
   {
      to->collinearPoints.push_back(collinearPoint);
   }
}

void EarClipper::RemoveCollinearPointsFromConsideration()
{
   if (vertices.size() > 3)
   {
      VertexList::iterator first = vertices.begin();
      VertexList::iterator second = first.next();

      Line2D_t ray(*(first->point), (*(second->point) - *(first->point)).normVector(), true); 

      VertexList::iterator currentSafe = first;
      VertexList::iterator possibleCollinear = second;
      VertexList::iterator currentCheck = second.next();

      do
      {
         if (ray.IsPointOnLine(*(currentCheck->point), COLLINEAR_TOLERANCE))
         {
            currentSafe->collinearPoints.push_back(possibleCollinear->point);
            vertices.erase(possibleCollinear);
         }
         else
         {
            currentSafe = possibleCollinear;

            ray.P = *(currentSafe->point);
            ray.V = (*(currentCheck->point) - ray.P).normVector();
         }

         possibleCollinear = currentCheck;
         currentCheck = currentCheck.next();
      } while (possibleCollinear != vertices.begin());

      ray.P = *(currentSafe->point);
      ray.V = (*(possibleCollinear->point) - ray.P).normVector();

      if (ray.IsPointOnLine(*(currentCheck->point), COLLINEAR_TOLERANCE))
      {
         currentSafe->collinearPoints.push_back(possibleCollinear->point);

         MigrateCollinearPoints(currentSafe, possibleCollinear);

         vertices.erase(possibleCollinear);
      }
   }
}

void EarClipper::GetPointsStraddlingVertex(VertexList::const_iterator vertexIterator, const Vector2*& pointBefore, const Vector2*& pointAtVertex, const Vector2*& pointAfter)
{
   pointBefore = (vertexIterator.previous())->point;
   pointAtVertex = vertexIterator->point;
   pointAfter = (vertexIterator.next())->point;
}

EarClipper::VertexType EarClipper::GetConvexOrReflexVertexType(const Vector2* pointBefore, const Vector2* pointOfInterest, const Vector2* pointAfter)
{
   Vector3 cross = (*pointOfInterest - *pointBefore).cross(*pointAfter - *pointOfInterest);

   if (cross.z == 0.0f)
   {
      return VertexType::Convex;
   }
   else if (cross.z > 0.0f)
   {
      return (polygonWinding == PolygonWinding::CounterClockwise) ? VertexType::Convex : VertexType::Reflex;
   }
   else
   {
      return (polygonWinding == PolygonWinding::CounterClockwise) ? VertexType::Reflex : VertexType::Convex;
   }
}

EarClipper::VertexType EarClipper::GetConvexOrReflexVertexType(VertexList::const_iterator vertexIterator)
{
   const Vector2 *pointBefore, *pointAtVertex, *pointAfter;
   GetPointsStraddlingVertex(vertexIterator, pointBefore, pointAtVertex, pointAfter);

   return GetConvexOrReflexVertexType(pointBefore, pointAtVertex, pointAfter);
}

void EarClipper::CheckForEarAndUpdateVertexType(VertexList::iterator vertexIterator)
{
   if (vertexIterator->type != VertexType::Reflex)
   {
      const Vector2 *firstPoint, *secondPoint, *thirdPoint;
      GetPointsStraddlingVertex(vertexIterator, firstPoint, secondPoint, thirdPoint);

      Triangle2D_t checkTriangle(*firstPoint, *secondPoint, *thirdPoint);

      for (VertexList::iterator checkVertexIterator = vertices.begin(), end = vertices.end(); checkVertexIterator != end; ++checkVertexIterator)
      {
         if ( (checkVertexIterator->point != firstPoint) && (checkVertexIterator->point != secondPoint) && (checkVertexIterator->point != thirdPoint) )
         {
            if (checkVertexIterator->type == VertexType::Reflex)
            {
               if (checkTriangle.PointIsOnPolygon(*(checkVertexIterator->point), EXPERIMENTAL_TOLERANCE))
               {
                  vertexIterator->type = VertexType::Convex;

                  return;
               }
            } 
         }
      }

      vertexIterator->type = VertexType::Ear;
   }
}

void EarClipper::RemoveEar(const Vector2* point, VertexListIteratorList& ears)
{
   for (VertexList::iterator& earIterator : ears)
   {
      if (earIterator->point == point)
      {
         ears.remove(earIterator);
         break;
      }
   }
}

void EarClipper::ReclassifyVertex(VertexList::iterator vertexIterator, VertexListIteratorList& ears)
{
   VertexType beforeType = vertexIterator->type;

   if (beforeType == VertexType::Reflex)
   {
      vertexIterator->type = GetConvexOrReflexVertexType(vertexIterator);
   }

   CheckForEarAndUpdateVertexType(vertexIterator);

   if (vertexIterator->type == VertexType::Ear)
   {
      if (beforeType != VertexType::Ear)
      {
         ears.push_front(vertexIterator);
      }
   }
   else if (beforeType == VertexType::Ear)
   {
      RemoveEar(vertexIterator->point, ears);
   }
}

void EarClipper::AddTriangle_R(std::vector<const Vector2*>& triangles,
                               const Vector2* trianglePoint1, std::list<const Vector2*>& collinearPoints1, 
                               const Vector2* trianglePoint2, std::list<const Vector2*>& collinearPoints2,
                               const Vector2* trianglePoint3, std::list<const Vector2*>& collinearPoints3)
{
   if (collinearPoints1.empty() && collinearPoints2.empty() && collinearPoints3.empty())
   {
      triangles.push_back(trianglePoint1);
      triangles.push_back(trianglePoint2);
      triangles.push_back(trianglePoint3);
   }
   else
   {
      if (!collinearPoints1.empty())
      {
         bool point3HasCollinearVertices = !collinearPoints3.empty();

         const Vector2* anchorPoint = point3HasCollinearVertices ? collinearPoints3.back() : trianglePoint3;

         const Vector2* lastPoint = trianglePoint1;

         for (const Vector2* collinearPoint : collinearPoints1)
         {
            triangles.push_back(lastPoint);
            triangles.push_back(collinearPoint);
            triangles.push_back(anchorPoint);

            lastPoint = collinearPoint;
         }

         std::list<const Vector2*> emptyList;

         if (point3HasCollinearVertices)
         {
            collinearPoints3.pop_back();

            if (collinearPoints3.empty())
            {
               triangles.push_back(anchorPoint);
               triangles.push_back(lastPoint);
               triangles.push_back(trianglePoint3);

               AddTriangle_R(triangles, trianglePoint2, collinearPoints2, trianglePoint3, collinearPoints3, lastPoint, emptyList);
            }
            else
            {
               triangles.push_back(lastPoint);
               triangles.push_back(trianglePoint2);
               triangles.push_back(anchorPoint);

               AddTriangle_R(triangles, trianglePoint3, collinearPoints3, anchorPoint, emptyList, trianglePoint2, collinearPoints2);
            }
         }
         else
         {
            AddTriangle_R(triangles, trianglePoint2, collinearPoints2, trianglePoint3, collinearPoints3, lastPoint, emptyList);
         }
      }
      else if (!collinearPoints2.empty())
      {
         AddTriangle_R(triangles, trianglePoint2, collinearPoints2, trianglePoint3, collinearPoints3, trianglePoint1, collinearPoints1);
      }
      else
      {
         AddTriangle_R(triangles, trianglePoint3, collinearPoints3, trianglePoint1, collinearPoints1, trianglePoint2, collinearPoints2);
      }
   }
}

void EarClipper::AddTriangle(std::vector<const Vector2*>& triangles, const VertexList::iterator& ear, bool last)
{
   VertexList::iterator previous = ear.previous();
   VertexList::iterator next = ear.next();

   const Vector2 *trianglePoint1, *trianglePoint2, *trianglePoint3;
   GetPointsStraddlingVertex(ear, trianglePoint1, trianglePoint2, trianglePoint3);

   if ( Triangle2D_t::IsValidTriangle(*trianglePoint1, *trianglePoint2, *trianglePoint3, EXPERIMENTAL_TOLERANCE) )
   {
      std::list<const Vector2*> emptyList;

      EarClipper::AddTriangle_R(triangles, trianglePoint1, previous->collinearPoints, trianglePoint2, ear->collinearPoints, trianglePoint3, last ? next->collinearPoints : emptyList);
   }
}

void EarClipper::AddRemainingTriangles(std::vector<const Vector2*>& triangles)
{
   std::size_t numVerticesRemaining = vertices.size();

   assert( numVerticesRemaining > 0 );

   std::list<const Vector2*> emptyList;

   VertexList::iterator first = vertices.begin();

   if (numVerticesRemaining == 2)
   {
      VertexList::iterator second = first.next();

      assert( (first->collinearPoints.size() == 1) || (second->collinearPoints.size() == 1) );

      if (first->collinearPoints.size() == 1)
      {
         EarClipper::AddTriangle_R(triangles, first->point, emptyList, first->collinearPoints.front(), emptyList, second->point, emptyList);
      }
      else if (second->collinearPoints.size() == 1)
      {
         EarClipper::AddTriangle_R(triangles, first->point, emptyList, second->point, emptyList, second->collinearPoints.front(), emptyList);
      }
   }
   else
   {
      assert( first->collinearPoints.size() == 2 );
            
      EarClipper::AddTriangle_R(triangles, first->point, emptyList, first->collinearPoints.front(), emptyList, first->collinearPoints.back(), emptyList);
   }
}

void EarClipper::AdjustForPossibleResultingCollinearity(VertexListIteratorList& ears, VertexList::iterator& beforeEar, VertexList::iterator& afterEar)
{
   beforeEar->collinearPoints.clear();

   if (vertices.size() > 3)
   {
      Vector2 beforeToAfter = (*(afterEar->point) - *(beforeEar->point)).normVector();

      Line2D_t rayBefore(*(beforeEar->point), -beforeToAfter, true);
      Line2D_t rayAfter(*(afterEar->point), beforeToAfter, true);

      VertexList::iterator twiceBeforeEar = beforeEar.previous();
      VertexList::iterator twiceAfterEar = afterEar.next();

      bool removeBeforeEar = rayBefore.IsPointOnLine(*(twiceBeforeEar->point), COLLINEAR_TOLERANCE);
      bool removeAfterEar = rayAfter.IsPointOnLine(*(twiceAfterEar->point), COLLINEAR_TOLERANCE);

      if (removeBeforeEar && removeAfterEar)
      {
         twiceBeforeEar->collinearPoints.push_back(beforeEar->point);
         twiceBeforeEar->collinearPoints.push_back(afterEar->point);

         MigrateCollinearPoints(twiceBeforeEar, afterEar);
      }
      else if (removeBeforeEar)
      {
         twiceBeforeEar->collinearPoints.push_back(beforeEar->point);
      }
      else if (removeAfterEar)
      {
         beforeEar->collinearPoints.push_back(afterEar->point);

         MigrateCollinearPoints(beforeEar, afterEar);
      }

      if (removeBeforeEar)
      {
         if (beforeEar->type == VertexType::Ear)
         {
            RemoveEar(beforeEar->point, ears);
         }

         vertices.erase(beforeEar);
      }

      if (removeAfterEar)
      {
         if (afterEar->type == VertexType::Ear)
         {
            RemoveEar(afterEar->point, ears);
         }

         vertices.erase(afterEar);
      }
   }
}

//{CodeReview:Triangulation}
void EarClipper::Triangulate_R(VertexListIteratorList& ears, std::vector<const Vector2*>& triangles)
{
   if (vertices.size() == 3)
   {
      AddTriangle(triangles, vertices.begin(), true);
   }
   else if (vertices.size() < 3)
   {
      AddRemainingTriangles(triangles);
   }
   else
   {
      assert( !ears.empty() );

      VertexList::iterator ear = ears.front();
      ears.pop_front();

      VertexList::iterator firstAdjacent = ear.previous();
      VertexList::iterator secondAdjacent = ear.next();

      AddTriangle(triangles, ear, false);

      vertices.erase(ear);

      ReclassifyVertex(firstAdjacent, ears);
      ReclassifyVertex(secondAdjacent, ears);

      AdjustForPossibleResultingCollinearity(ears, firstAdjacent, secondAdjacent);

      Triangulate_R(ears, triangles);
   }
}

}