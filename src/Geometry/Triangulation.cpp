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
#include "Locus/Geometry/Polygon.h"
#include "Locus/Geometry/PolygonHierarchy.h"
#include "Locus/Geometry/Triangle.h"
#include "Locus/Geometry/LineSegment.h"

#include "Locus/Common/CircularList.h"
#include "Locus/Common/Float.h"

#include <forward_list>
#include <list>
#include <limits>
#include <queue>
#include <algorithm>

#include <assert.h>

namespace Locus
{

namespace EarClipping
{

enum class VertexType
{
   Convex,
   Reflex,
   Ear
};

struct AugmentedVertex
{
   AugmentedVertex(const Vector2* vertex)
      : vertex(vertex), vertexType(VertexType::Reflex)
   {
   }

   const Vector2* vertex;
   VertexType vertexType;
   std::list<const Vector2*> collinearVertices;
};

typedef CircularList<AugmentedVertex> AugmentedVertexList;

static const Polygon2D_t* FindMaxInteriorPointInListAndRemovePolygonFromConsideration(std::forward_list<const Polygon2D_t*>& innerPolygons, std::size_t& maxInteriorPointIndex);

static void StitchOuterAndInnerPolygons(AugmentedVertexList& augmentedVertices, const Polygon2D_t& hole, std::size_t maxInteriorPointIndex, AugmentedVertexList::iterator mutuallyVisibleVertexIter);
static AugmentedVertexList::iterator DetermineMutuallyVisibleVertexFromRayCastResult(AugmentedVertexList& augmentedVertices, const Vector2& maxInteriorPoint, const Vector2& intersectionPointOnEdge, AugmentedVertexList::iterator pointOnEdgeWithMaximumXIter, PolygonWinding winding);
static AugmentedVertexList::iterator FindMutuallyVisibleVertex(AugmentedVertexList& augmentedVertices, const Vector2& maxInteriorPoint, PolygonWinding winding);
static AugmentedVertexList::iterator RayCastFromMaxInteriorPointToOuterPolygon(AugmentedVertexList& augmentedVertices, const Vector2& maxInteriorPoint, Vector2& intersectionPointOnEdge, AugmentedVertexList::iterator& pointOnEdgeWithMaximumXIter, PolygonWinding winding);

static void MakeSimple(AugmentedVertexList& augmentedVertices, const std::vector<const Polygon2D_t*>& innerPolygons, PolygonWinding winding);

static void RemoveCollinearVerticesFromConsideration(AugmentedVertexList& augmentedVertices);
static void AdjustForPossibleResultingCollinearity(AugmentedVertexList& augmentedVertices, std::forward_list< AugmentedVertexList::iterator >& ears, AugmentedVertexList::iterator& beforeEar, AugmentedVertexList::iterator& afterEar);
static void MigrateCollinearVertices(AugmentedVertexList::iterator& to, const AugmentedVertexList::iterator& from);

static VertexType GetConvexOrReflexVertexType(const Vector2* firstVertex, const Vector2* secondVertex, const Vector2* thirdVertex, PolygonWinding winding);
static VertexType GetConvexOrReflexVertexType(AugmentedVertexList::const_iterator augmentedVertexIterator, PolygonWinding winding);
static void CheckForEarAndUpdateVertexType(AugmentedVertexList& augmentedVertices, AugmentedVertexList::iterator augmentedVertexIterator);
static void RemoveEar(const Vector2* vertex, std::forward_list< AugmentedVertexList::iterator >& ears);
static void ReclassifyVertex(AugmentedVertexList& augmentedVertices, AugmentedVertexList::iterator augmentedVertexIterator, PolygonWinding winding, std::forward_list< AugmentedVertexList::iterator >& ears);

static void Triangulate(AugmentedVertexList& augmentedVertices, PolygonWinding winding, std::vector<const Vector2*>& triangles);
static void Triangulate_R(AugmentedVertexList& augmentedVertices, std::forward_list< AugmentedVertexList::iterator >& ears, PolygonWinding winding, std::vector<const Vector2*>& triangles);

static void AddTriangle(std::vector<const Vector2*>& triangles, const AugmentedVertexList::iterator& ear, bool last);
static void AddRemainingTriangles(std::vector<const Vector2*>& triangles, AugmentedVertexList& augmentedVertices);
static void AddTriangle_R(std::vector<const Vector2*>& triangles, const Vector2* trianglePoint1, std::list<const Vector2*>& collinearVertices1, 
                          const Vector2* trianglePoint2, std::list<const Vector2*>& collinearVertices2, const Vector2* trianglePoint3, std::list<const Vector2*>& collinearVertices3);

static const float EXPERIMENTAL_TOLERANCE = 1.0f;
static const float COLLINEAR_TOLERANCE = 1.0f;

void Triangulate(const Polygon2D_t& polygon, std::vector<const Vector2*>& triangles)
{
   if (polygon.IsWellDefined())
   {
      AugmentedVertexList augmentedVertices;

      for (std::size_t pointIndex = 0, numPoints = polygon.NumPoints(); pointIndex < numPoints; ++pointIndex)
      {
         augmentedVertices.push_back( AugmentedVertex(&polygon[pointIndex]) );
      }

      Triangulate(augmentedVertices, polygon.GetWinding(Vector3::ZAxis()), triangles);
   }
}

//{CodeReview:Triangulation}
void Triangulate(const Polygon2D_t& polygon, const std::vector<const Polygon2D_t*>& innerPolygons, std::vector<const Vector2*>& triangles)
{
   if (innerPolygons.size() == 0)
   {
      Triangulate(polygon, triangles);
   }
   else
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

         AugmentedVertexList augmentedVertices;

         for (std::size_t pointIndex = 0, numPoints = polygon.NumPoints(); pointIndex < numPoints; ++pointIndex)
         {
            augmentedVertices.push_back( AugmentedVertex(&polygon[pointIndex]) );
         }

         MakeSimple(augmentedVertices, innerPolygons, polygonWinding);

         Triangulate(augmentedVertices, polygonWinding, triangles);
      }
   }
}

//{CodeReview:Triangulation}
void Triangulate(std::vector<Polygon2D_t>& polygons, PolygonWinding winding, std::vector<const Vector2*>& triangles)
{
   std::vector<Polygon2D_t*> polygonsForHierarchy;
   polygonsForHierarchy.reserve(polygons.size());

   for (Polygon2D_t& polygon : polygons)
   {
      polygonsForHierarchy.push_back(&polygon);
   }

   PolygonHierarchy<Polygon2D_t> polygonHierarchy(polygonsForHierarchy, winding, Vector3::ZAxis(), EXPERIMENTAL_TOLERANCE);

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

         Triangulate(*(node->polygon), innerPolygons, triangles);
      }
   }
}

static const Polygon2D_t* FindMaxInteriorPointInListAndRemovePolygonFromConsideration(std::forward_list<const Polygon2D_t*>& innerPolygons, std::size_t& maxInteriorPointIndex)
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

static AugmentedVertexList::iterator RayCastFromMaxInteriorPointToOuterPolygon(AugmentedVertexList& augmentedVertices, const Vector2& maxInteriorPoint, Vector2& intersectionPointOnEdge, AugmentedVertexList::iterator& pointOnEdgeWithMaximumXIter, PolygonWinding winding)
{
   //a) Call the vertex on the hole with the maximum x coordinate P. Call the ray { P, (1,0) } R. Intersect R
   //   with the edges of the outer polygon. Find the edge whose intersection is closest to P. Call this edge E.
   //   If R goes through E entirely, then the mutually visible vertex is the endpoint on E closest to P. Otherwise,
   //   if R goes through one of the endpoints of E, then the mutually visible vertex is this endpoint. Otherwise,
   //   R intersects the line segment at a point other than the endpoints (see b)

   AugmentedVertexList::iterator augmentedVerticesEnd = augmentedVertices.end();

   AugmentedVertexList::iterator mutuallyVisibleVertexIter = augmentedVerticesEnd;

   AugmentedVertexList::iterator possibleMutuallyVisibleVertex = augmentedVerticesEnd;

   Line2D_t rayFromMaxVertex(maxInteriorPoint, Vector2::XAxis(), true);

   float minDistance = std::numeric_limits<float>::max();

   LineSegment2D_t lineSegmentOnOuterPolygon;
   Vector2 intersectionPoint1, intersectionPoint2;
   AugmentedVertexList::iterator pointOnThisEdgeWithMaximumXIter = augmentedVerticesEnd;

   for (AugmentedVertexList::iterator augmentedVertexIter = augmentedVertices.begin(); augmentedVertexIter != augmentedVerticesEnd; ++augmentedVertexIter)
   {
      AugmentedVertexList::iterator nextVertexIter = augmentedVertexIter.next();

      lineSegmentOnOuterPolygon.P1 = *(augmentedVertexIter->vertex);
      lineSegmentOnOuterPolygon.P2 = *(nextVertexIter->vertex);

      IntersectionType intersection = rayFromMaxVertex.LineSegmentIntersection(lineSegmentOnOuterPolygon, intersectionPoint1, intersectionPoint2, EXPERIMENTAL_TOLERANCE);

      if ((intersection == IntersectionType::LineSegment) || (intersection == IntersectionType::Point))
      {
         possibleMutuallyVisibleVertex = augmentedVertexIter;
         bool rayHitEndPointOfEdge = true;

         if (lineSegmentOnOuterPolygon.P1.x > lineSegmentOnOuterPolygon.P2.x)
         {
            pointOnThisEdgeWithMaximumXIter = augmentedVertexIter;
         }
         else
         {
            pointOnThisEdgeWithMaximumXIter = nextVertexIter;
         }

         if (intersection == IntersectionType::LineSegment)
         {
            if (lineSegmentOnOuterPolygon.P1.x < lineSegmentOnOuterPolygon.P2.x)
            {
               possibleMutuallyVisibleVertex = augmentedVertexIter;
            }
            else
            {
               possibleMutuallyVisibleVertex = nextVertexIter;
            }
         }
         else if (intersection == IntersectionType::Point)
         {
            if (intersectionPoint1.PreciselyEqualTo(lineSegmentOnOuterPolygon.P1))
            {
               possibleMutuallyVisibleVertex = augmentedVertexIter;
            }
            else if (intersectionPoint1.PreciselyEqualTo(lineSegmentOnOuterPolygon.P2))
            {
               possibleMutuallyVisibleVertex = nextVertexIter;
            }
            else
            {
               rayHitEndPointOfEdge = false;
            }
         }

         float distance = rayHitEndPointOfEdge ? (possibleMutuallyVisibleVertex->vertex->x - maxInteriorPoint.x) : (intersectionPoint1.x - maxInteriorPoint.x);

         bool sameishDistance = Float::FEqual(distance, minDistance);

         if ((distance < minDistance) || sameishDistance)
         {
            bool takeThisVertex = true;

            if (sameishDistance)
            {
               const Vector2* beforeMutuallyVisibleVertex = possibleMutuallyVisibleVertex.previous()->vertex;

               takeThisVertex = (GetConvexOrReflexVertexType(beforeMutuallyVisibleVertex, possibleMutuallyVisibleVertex->vertex, &maxInteriorPoint, winding) == VertexType::Convex);
            }

            if (takeThisVertex)
            {
               if (rayHitEndPointOfEdge)
               {
                  mutuallyVisibleVertexIter = possibleMutuallyVisibleVertex;
               }
               else
               {
                  mutuallyVisibleVertexIter = augmentedVerticesEnd;
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

static AugmentedVertexList::iterator DetermineMutuallyVisibleVertexFromRayCastResult(AugmentedVertexList& augmentedVertices, const Vector2& maxInteriorPoint, const Vector2& intersectionPointOnEdge, AugmentedVertexList::iterator pointOnEdgeWithMaximumXIter, PolygonWinding winding)
{
   AugmentedVertexList::iterator augmentedVerticesEnd = augmentedVertices.end();

   AugmentedVertexList::iterator mutuallyVisibleVertexIter = augmentedVerticesEnd;

   // Form a triangle with P (see explanation of a) above) (P1), the intersection point on
   // the edge (P2) and the point on the edge with maximum X (P3).
   //
   // If no points on the outer polygon fall on this triangle, then P3 is the visible vertex.
   //
   // Otherwise, the reflex vertex of the outer polygon that falls into this triangle and
   // minimizes the angle to the ray (i.e. x axis) is the mutually visible vertex

   Triangle2D_t checkTriangle(maxInteriorPoint, intersectionPointOnEdge, *(pointOnEdgeWithMaximumXIter->vertex));

   std::forward_list<AugmentedVertexList::iterator> reflexPointsOnTriangle;
   bool hasAnyReflexPointsOnTriangle = false;

   for (AugmentedVertexList::iterator augmentedVertexIterator = augmentedVertices.begin(); augmentedVertexIterator != augmentedVerticesEnd; ++augmentedVertexIterator)
   {
      if (checkTriangle.PointIsOnPolygon(*(augmentedVertexIterator->vertex), EXPERIMENTAL_TOLERANCE))
      {
         if (GetConvexOrReflexVertexType(augmentedVertexIterator, winding) == VertexType::Reflex)
         {
            reflexPointsOnTriangle.push_front(augmentedVertexIterator);
            hasAnyReflexPointsOnTriangle = true;
         }
      }
   }

   if (hasAnyReflexPointsOnTriangle)
   {
      mutuallyVisibleVertexIter = reflexPointsOnTriangle.front();
      reflexPointsOnTriangle.pop_front();

      float minDistance = mutuallyVisibleVertexIter->vertex->distanceTo(maxInteriorPoint);
      float minAngle = (*(mutuallyVisibleVertexIter->vertex) - maxInteriorPoint).angleBetweenRadians( Vector2::XAxis() );

      for (const AugmentedVertexList::iterator& reflexVertexIter : reflexPointsOnTriangle)
      {
         float angle = (*(reflexVertexIter->vertex) - maxInteriorPoint).angleBetweenRadians( Vector2::XAxis() );
         float distance = reflexVertexIter->vertex->distanceTo(maxInteriorPoint);

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

static AugmentedVertexList::iterator FindMutuallyVisibleVertex(AugmentedVertexList& augmentedVertices, const Vector2& maxInteriorPoint, PolygonWinding winding)
{
   // Find a vertex on the outer polygon visible to that vertex

   Vector2 intersectionPointOnEdge;
   AugmentedVertexList::iterator pointOnEdgeWithMaximumXIter = augmentedVertices.end();

   AugmentedVertexList::iterator mutuallyVisibleVertex = RayCastFromMaxInteriorPointToOuterPolygon(augmentedVertices, maxInteriorPoint, intersectionPointOnEdge, pointOnEdgeWithMaximumXIter, winding);

   if (mutuallyVisibleVertex == augmentedVertices.end())
   {
      mutuallyVisibleVertex = DetermineMutuallyVisibleVertexFromRayCastResult(augmentedVertices, maxInteriorPoint, intersectionPointOnEdge, pointOnEdgeWithMaximumXIter, winding);
   }

   return mutuallyVisibleVertex;
}

static void StitchOuterAndInnerPolygons(AugmentedVertexList& augmentedVertices, const Polygon2D_t& hole, std::size_t maxInteriorPointIndex, AugmentedVertexList::iterator mutuallyVisibleVertexIter)
{
   // Attach hole to outer vertices through the mutually visible vertex

   AugmentedVertexList::iterator insertIterator = mutuallyVisibleVertexIter.next();

   for (std::size_t holeVertex = 1, holeVertexIndex = maxInteriorPointIndex, holeSize = hole.NumPoints(); holeVertex <= holeSize + 1; ++holeVertex, holeVertexIndex = (holeVertexIndex + 1) % holeSize)
   {
      augmentedVertices.insert(insertIterator, AugmentedVertex(&hole[holeVertexIndex]));
   }

   augmentedVertices.insert(insertIterator, AugmentedVertex(mutuallyVisibleVertexIter->vertex));
}

static void MakeSimple(AugmentedVertexList& augmentedVertices, const std::vector<const Polygon2D_t*>& innerPolygons, PolygonWinding winding)
{
   std::forward_list<const Polygon2D_t*> innerPolygonsRemaining;
   for (const Polygon2D_t* innerPolygon : innerPolygons)
   {
      innerPolygonsRemaining.push_front(innerPolygon);
   }

   for (std::size_t innerPolygonIndex = 0, numInnerPolygons = innerPolygons.size(); innerPolygonIndex < numInnerPolygons; ++innerPolygonIndex)
   {
      std::size_t maxInteriorPointIndex;
      const Polygon2D_t* innerPolygon = FindMaxInteriorPointInListAndRemovePolygonFromConsideration(innerPolygonsRemaining, maxInteriorPointIndex);

      AugmentedVertexList::iterator mutuallyVisibleVertex = FindMutuallyVisibleVertex(augmentedVertices, (*innerPolygon)[maxInteriorPointIndex], winding);

      assert(mutuallyVisibleVertex != augmentedVertices.end());

      StitchOuterAndInnerPolygons(augmentedVertices, *innerPolygon, maxInteriorPointIndex, mutuallyVisibleVertex);
   }
}

static void MigrateCollinearVertices(AugmentedVertexList::iterator& to, const AugmentedVertexList::iterator& from)
{
   for (const Vector2* collinearVertex : from->collinearVertices)
   {
      to->collinearVertices.push_back(collinearVertex);
   }
}

static void RemoveCollinearVerticesFromConsideration(AugmentedVertexList& augmentedVertices)
{
   if (augmentedVertices.size() > 3)
   {
      AugmentedVertexList::iterator first = augmentedVertices.begin();
      AugmentedVertexList::iterator second = first.next();

      Line2D_t ray(*(first->vertex), (*(second->vertex) - *(first->vertex)).normVector(), true); 

      AugmentedVertexList::iterator currentSafe = first;
      AugmentedVertexList::iterator possibleCollinear = second;
      AugmentedVertexList::iterator currentCheck = second.next();

      do
      {
         if (ray.IsPointOnLine(*(currentCheck->vertex), COLLINEAR_TOLERANCE))
         {
            currentSafe->collinearVertices.push_back(possibleCollinear->vertex);
            augmentedVertices.erase(possibleCollinear);
         }
         else
         {
            currentSafe = possibleCollinear;

            ray.P = *(currentSafe->vertex);
            ray.V = (*(currentCheck->vertex) - ray.P).normVector();
         }

         possibleCollinear = currentCheck;
         currentCheck = currentCheck.next();
      } while (possibleCollinear != augmentedVertices.begin());

      ray.P = *(currentSafe->vertex);
      ray.V = (*(possibleCollinear->vertex) - ray.P).normVector();

      if (ray.IsPointOnLine(*(currentCheck->vertex), COLLINEAR_TOLERANCE))
      {
         currentSafe->collinearVertices.push_back(possibleCollinear->vertex);

         MigrateCollinearVertices(currentSafe, possibleCollinear);

         augmentedVertices.erase(possibleCollinear);
      }
   }
}

static void Triangulate(AugmentedVertexList& augmentedVertices, PolygonWinding winding, std::vector<const Vector2*>& triangles)
{
   triangles.reserve( triangles.size() + (3 * (augmentedVertices.size() - 2)) );

   RemoveCollinearVerticesFromConsideration(augmentedVertices);

   if (augmentedVertices.size() >= 3)
   {
      std::forward_list< AugmentedVertexList::iterator > ears;

      for (AugmentedVertexList::iterator augmentedVertexIterator = augmentedVertices.begin(), end = augmentedVertices.end(); augmentedVertexIterator != end; ++augmentedVertexIterator)
      {
         augmentedVertexIterator->vertexType = GetConvexOrReflexVertexType(augmentedVertexIterator, winding);
      }

      for (AugmentedVertexList::iterator augmentedVertexIterator = augmentedVertices.begin(), end = augmentedVertices.end(); augmentedVertexIterator != end; ++augmentedVertexIterator)
      {
         CheckForEarAndUpdateVertexType(augmentedVertices, augmentedVertexIterator);

         if (augmentedVertexIterator->vertexType == VertexType::Ear)
         {
            ears.push_front(augmentedVertexIterator);
         }
      }

      Triangulate_R(augmentedVertices, ears, winding, triangles);
   }
}

static VertexType GetConvexOrReflexVertexType(const Vector2* firstVertex, const Vector2* secondVertex, const Vector2* thirdVertex, PolygonWinding winding)
{
   Vector3 cross = (*secondVertex - *firstVertex).cross(*thirdVertex - *secondVertex);

   if (cross.z == 0.0f)
   {
      return VertexType::Convex;
   }
   else if (cross.z > 0.0f)
   {
      return (winding == PolygonWinding::CounterClockwise) ? VertexType::Convex : VertexType::Reflex;
   }
   else
   {
      return (winding == PolygonWinding::CounterClockwise) ? VertexType::Reflex : VertexType::Convex;
   }
}

static VertexType GetConvexOrReflexVertexType(AugmentedVertexList::const_iterator augmentedVertexIterator, PolygonWinding winding)
{
   const Vector2* firstVertex = (augmentedVertexIterator.previous())->vertex;
   const Vector2* secondVertex = augmentedVertexIterator->vertex;
   const Vector2* thirdVertex = (augmentedVertexIterator.next())->vertex;

   return GetConvexOrReflexVertexType(firstVertex, secondVertex, thirdVertex, winding);
}

static void CheckForEarAndUpdateVertexType(AugmentedVertexList& augmentedVertices, AugmentedVertexList::iterator augmentedVertexIterator)
{
   if (augmentedVertexIterator->vertexType != VertexType::Reflex)
   {
      const Vector2* firstVertex = (augmentedVertexIterator.previous())->vertex;
      const Vector2* secondVertex = augmentedVertexIterator->vertex;
      const Vector2* thirdVertex = (augmentedVertexIterator.next())->vertex;

      Triangle2D_t checkTriangle(*firstVertex, *secondVertex, *thirdVertex);

      for (AugmentedVertexList::iterator checkVertexIterator = augmentedVertices.begin(), end = augmentedVertices.end(); checkVertexIterator != end; ++checkVertexIterator)
      {
         if ( (checkVertexIterator->vertex != firstVertex) && (checkVertexIterator->vertex != secondVertex) && (checkVertexIterator->vertex != thirdVertex) )
         {
            if (checkVertexIterator->vertexType == VertexType::Reflex)
            {
               if (checkTriangle.PointIsOnPolygon(*(checkVertexIterator->vertex), EXPERIMENTAL_TOLERANCE))
               {
                  augmentedVertexIterator->vertexType = VertexType::Convex;

                  return;
               }
            } 
         }
      }

      augmentedVertexIterator->vertexType = VertexType::Ear;
   }
}

static void RemoveEar(const Vector2* vertex, std::forward_list< AugmentedVertexList::iterator >& ears)
{
   for (AugmentedVertexList::iterator& earIterator : ears)
   {
      if (earIterator->vertex == vertex)
      {
         ears.remove(earIterator);
         break;
      }
   }
}

static void ReclassifyVertex(AugmentedVertexList& augmentedVertices, AugmentedVertexList::iterator augmentedVertexIterator, PolygonWinding winding, std::forward_list< AugmentedVertexList::iterator >& ears)
{
   VertexType beforeType = augmentedVertexIterator->vertexType;

   if (beforeType == VertexType::Reflex)
   {
      augmentedVertexIterator->vertexType = GetConvexOrReflexVertexType(augmentedVertexIterator, winding);
   }

   CheckForEarAndUpdateVertexType(augmentedVertices, augmentedVertexIterator);

   if (augmentedVertexIterator->vertexType == VertexType::Ear)
   {
      if (beforeType != VertexType::Ear)
      {
         ears.push_front(augmentedVertexIterator);
      }
   }
   else if (beforeType == VertexType::Ear)
   {
      RemoveEar(augmentedVertexIterator->vertex, ears);
   }
}

static void AddTriangle_R(std::vector<const Vector2*>& triangles, const Vector2* trianglePoint1, std::list<const Vector2*>& collinearVertices1, 
                           const Vector2* trianglePoint2, std::list<const Vector2*>& collinearVertices2, const Vector2* trianglePoint3, std::list<const Vector2*>& collinearVertices3)
{
   if (collinearVertices1.empty() && collinearVertices2.empty() && collinearVertices3.empty())
   {
      triangles.push_back(trianglePoint1);
      triangles.push_back(trianglePoint2);
      triangles.push_back(trianglePoint3);
   }
   else
   {
      if (!collinearVertices1.empty())
      {
         bool point3HasCollinearVertices = !collinearVertices3.empty();

         const Vector2* anchorPoint = point3HasCollinearVertices ? collinearVertices3.back() : trianglePoint3;

         const Vector2* lastPoint = trianglePoint1;

         for (const Vector2* collinearPoint : collinearVertices1)
         {
            triangles.push_back(lastPoint);
            triangles.push_back(collinearPoint);
            triangles.push_back(anchorPoint);

            lastPoint = collinearPoint;
         }

         std::list<const Vector2*> emptyList;

         if (point3HasCollinearVertices)
         {
            collinearVertices3.pop_back();

            if (collinearVertices3.empty())
            {
               triangles.push_back(anchorPoint);
               triangles.push_back(lastPoint);
               triangles.push_back(trianglePoint3);

               AddTriangle_R(triangles, trianglePoint2, collinearVertices2, trianglePoint3, collinearVertices3, lastPoint, emptyList);
            }
            else
            {
               triangles.push_back(lastPoint);
               triangles.push_back(trianglePoint2);
               triangles.push_back(anchorPoint);

               AddTriangle_R(triangles, trianglePoint3, collinearVertices3, anchorPoint, emptyList, trianglePoint2, collinearVertices2);
            }
         }
         else
         {
            AddTriangle_R(triangles, trianglePoint2, collinearVertices2, trianglePoint3, collinearVertices3, lastPoint, emptyList);
         }
      }
      else if (!collinearVertices2.empty())
      {
         AddTriangle_R(triangles, trianglePoint2, collinearVertices2, trianglePoint3, collinearVertices3, trianglePoint1, collinearVertices1);
      }
      else
      {
         AddTriangle_R(triangles, trianglePoint3, collinearVertices3, trianglePoint1, collinearVertices1, trianglePoint2, collinearVertices2);
      }
   }
}

static void AddTriangle(std::vector<const Vector2*>& triangles, const AugmentedVertexList::iterator& ear, bool last)
{
   AugmentedVertexList::iterator previous = ear.previous();
   AugmentedVertexList::iterator next = ear.next();

   const Vector2* trianglePoint1 = previous->vertex;
   const Vector2* trianglePoint2 = ear->vertex;
   const Vector2* trianglePoint3 = next->vertex;

   if ( Triangle2D_t::IsValidTriangle(*trianglePoint1, *trianglePoint2, *trianglePoint3, EXPERIMENTAL_TOLERANCE) )
   {
      std::list<const Vector2*> emptyList;

      AddTriangle_R(triangles, trianglePoint1, previous->collinearVertices, trianglePoint2, ear->collinearVertices, trianglePoint3, last ? next->collinearVertices : emptyList);
   }
}

static void AddRemainingTriangles(std::vector<const Vector2*>& triangles, AugmentedVertexList& augmentedVertices)
{
   std::size_t numVerticesRemaining = augmentedVertices.size();

   assert( numVerticesRemaining > 0 );

   std::list<const Vector2*> emptyList;

   AugmentedVertexList::iterator first = augmentedVertices.begin();

   if (numVerticesRemaining == 2)
   {
      AugmentedVertexList::iterator second = first.next();

      assert( (first->collinearVertices.size() == 1) || (second->collinearVertices.size() == 1) );

      if (first->collinearVertices.size() == 1)
      {
         AddTriangle_R(triangles, first->vertex, emptyList, first->collinearVertices.front(), emptyList, second->vertex, emptyList);
      }
      else if (second->collinearVertices.size() == 1)
      {
         AddTriangle_R(triangles, first->vertex, emptyList, second->vertex, emptyList, second->collinearVertices.front(), emptyList);
      }
   }
   else
   {
      assert( first->collinearVertices.size() == 2 );
            
      AddTriangle_R(triangles, first->vertex, emptyList, first->collinearVertices.front(), emptyList, first->collinearVertices.back(), emptyList);
   }
}

static void AdjustForPossibleResultingCollinearity(AugmentedVertexList& augmentedVertices, std::forward_list< AugmentedVertexList::iterator >& ears, AugmentedVertexList::iterator& beforeEar, AugmentedVertexList::iterator& afterEar)
{
   beforeEar->collinearVertices.clear();

   if (augmentedVertices.size() > 3)
   {
      Vector2 beforeToAfter = (*(afterEar->vertex) - *(beforeEar->vertex)).normVector();

      Line2D_t rayBefore(*(beforeEar->vertex), -beforeToAfter, true);
      Line2D_t rayAfter(*(afterEar->vertex), beforeToAfter, true);

      AugmentedVertexList::iterator twiceBeforeEar = beforeEar.previous();
      AugmentedVertexList::iterator twiceAfterEar = afterEar.next();

      bool removeBeforeEar = rayBefore.IsPointOnLine(*(twiceBeforeEar->vertex), COLLINEAR_TOLERANCE);
      bool removeAfterEar = rayAfter.IsPointOnLine(*(twiceAfterEar->vertex), COLLINEAR_TOLERANCE);

      if (removeBeforeEar && removeAfterEar)
      {
         twiceBeforeEar->collinearVertices.push_back(beforeEar->vertex);
         twiceBeforeEar->collinearVertices.push_back(afterEar->vertex);

         MigrateCollinearVertices(twiceBeforeEar, afterEar);
      }
      else if (removeBeforeEar)
      {
         twiceBeforeEar->collinearVertices.push_back(beforeEar->vertex);
      }
      else if (removeAfterEar)
      {
         beforeEar->collinearVertices.push_back(afterEar->vertex);

         MigrateCollinearVertices(beforeEar, afterEar);
      }

      if (removeBeforeEar)
      {
         if (beforeEar->vertexType == VertexType::Ear)
         {
            RemoveEar(beforeEar->vertex, ears);
         }

         augmentedVertices.erase(beforeEar);
      }

      if (removeAfterEar)
      {
         if (afterEar->vertexType == VertexType::Ear)
         {
            RemoveEar(afterEar->vertex, ears);
         }

         augmentedVertices.erase(afterEar);
      }
   }
}

//{CodeReview:Triangulation}
static void Triangulate_R(AugmentedVertexList& augmentedVertices, std::forward_list< AugmentedVertexList::iterator >& ears, PolygonWinding winding, std::vector<const Vector2*>& triangles)
{
   if (augmentedVertices.size() == 3)
   {
      AddTriangle(triangles, augmentedVertices.begin(), true);
   }
   else if (augmentedVertices.size() < 3)
   {
      AddRemainingTriangles(triangles, augmentedVertices);
   }
   else
   {
      assert( !ears.empty() );

      AugmentedVertexList::iterator ear = ears.front();
      ears.pop_front();

      AugmentedVertexList::iterator firstAdjacent = ear.previous();
      AugmentedVertexList::iterator secondAdjacent = ear.next();

      AddTriangle(triangles, ear, false);

      augmentedVertices.erase(ear);

      ReclassifyVertex(augmentedVertices, firstAdjacent, winding, ears);
      ReclassifyVertex(augmentedVertices, secondAdjacent, winding, ears);

      AdjustForPossibleResultingCollinearity(augmentedVertices, ears, firstAdjacent, secondAdjacent);

      Triangulate_R(augmentedVertices, ears, winding, triangles);
   }
}

}

}