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

namespace EarClipping
{

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

void MakeSimple(VertexList& vertices, const std::vector<const Polygon2D_t*>& innerPolygons, PolygonWinding winding);

VertexList::iterator FindMutuallyVisibleVertex(VertexList& vertices, const Vector2& maxInteriorPoint, PolygonWinding winding);
const Polygon2D_t* FindMaxInteriorPointInListAndRemovePolygonFromConsideration(std::forward_list<const Polygon2D_t*>& innerPolygons, std::size_t& maxInteriorPointIndex);
VertexList::iterator DetermineMutuallyVisibleVertexFromRayCastResult(VertexList& vertices, const Vector2& maxInteriorPoint, const Vector2& intersectionPointOnEdge, VertexList::iterator pointOnEdgeWithMaximumXIter, PolygonWinding winding);
VertexList::iterator RayCastFromMaxInteriorPointToOuterPolygon(VertexList& vertices, const Vector2& maxInteriorPoint, Vector2& intersectionPointOnEdge, VertexList::iterator& pointOnEdgeWithMaximumXIter, PolygonWinding winding);
void StitchOuterAndInnerPolygons(VertexList& vertices, const Polygon2D_t& hole, std::size_t maxInteriorPointIndex, VertexList::iterator mutuallyVisibleVertexIter);

void RemoveCollinearPointsFromConsideration(VertexList& vertices);
void AdjustForPossibleResultingCollinearity(VertexList& vertices, VertexListIteratorList& ears, VertexList::iterator& beforeEar, VertexList::iterator& afterEar);
void MigrateCollinearPoints(VertexList::iterator& to, const VertexList::iterator& from);

void GetPointsStraddlingVertex(VertexList::const_iterator vertexIterator, const Vector2*& pointBefore, const Vector2*& pointAtVertex, const Vector2*& pointAfter);
VertexType GetConvexOrReflexVertexType(const Vector2* pointBefore, const Vector2* pointOfInterest, const Vector2* pointAfter, PolygonWinding winding);
VertexType GetConvexOrReflexVertexType(VertexList::const_iterator vertexIterator, PolygonWinding winding);
void CheckForEarAndUpdateVertexType(VertexList& vertices, VertexList::iterator vertexIterator);
void RemoveEar(const Vector2* point, VertexListIteratorList& ears);
void ReclassifyVertex(VertexList& vertices, VertexList::iterator vertexIterator, PolygonWinding winding, VertexListIteratorList& ears);

void InternalTriangulate(VertexList& vertices, PolygonWinding winding, std::vector<const Vector2*>& triangles);
void InternalTriangulate_R(VertexList& vertices, VertexListIteratorList& ears, PolygonWinding winding, std::vector<const Vector2*>& triangles);

void AddTriangle(std::vector<const Vector2*>& triangles, const VertexList::iterator& ear, bool last);
void AddRemainingTriangles(std::vector<const Vector2*>& triangles, VertexList& vertices);
void AddTriangle_R(std::vector<const Vector2*>& triangles, const Vector2* trianglePoint1, std::list<const Vector2*>& collinearPoints1, 
                   const Vector2* trianglePoint2, std::list<const Vector2*>& collinearPoints2, const Vector2* trianglePoint3, std::list<const Vector2*>& collinearPoints3);

}

}
