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

void MakeSimple(AugmentedVertexList& augmentedVertices, const std::vector<const Polygon2D_t*>& innerPolygons, PolygonWinding winding);

AugmentedVertexList::iterator FindMutuallyVisibleVertex(AugmentedVertexList& augmentedVertices, const Vector2& maxInteriorPoint, PolygonWinding winding);
const Polygon2D_t* FindMaxInteriorPointInListAndRemovePolygonFromConsideration(std::forward_list<const Polygon2D_t*>& innerPolygons, std::size_t& maxInteriorPointIndex);
AugmentedVertexList::iterator DetermineMutuallyVisibleVertexFromRayCastResult(AugmentedVertexList& augmentedVertices, const Vector2& maxInteriorPoint, const Vector2& intersectionPointOnEdge, AugmentedVertexList::iterator pointOnEdgeWithMaximumXIter, PolygonWinding winding);
AugmentedVertexList::iterator RayCastFromMaxInteriorPointToOuterPolygon(AugmentedVertexList& augmentedVertices, const Vector2& maxInteriorPoint, Vector2& intersectionPointOnEdge, AugmentedVertexList::iterator& pointOnEdgeWithMaximumXIter, PolygonWinding winding);
void StitchOuterAndInnerPolygons(AugmentedVertexList& augmentedVertices, const Polygon2D_t& hole, std::size_t maxInteriorPointIndex, AugmentedVertexList::iterator mutuallyVisibleVertexIter);

void RemoveCollinearVerticesFromConsideration(AugmentedVertexList& augmentedVertices);
void AdjustForPossibleResultingCollinearity(AugmentedVertexList& augmentedVertices, std::forward_list< AugmentedVertexList::iterator >& ears, AugmentedVertexList::iterator& beforeEar, AugmentedVertexList::iterator& afterEar);
void MigrateCollinearVertices(AugmentedVertexList::iterator& to, const AugmentedVertexList::iterator& from);

VertexType GetConvexOrReflexVertexType(const Vector2* firstVertex, const Vector2* secondVertex, const Vector2* thirdVertex, PolygonWinding winding);
VertexType GetConvexOrReflexVertexType(AugmentedVertexList::const_iterator augmentedVertexIterator, PolygonWinding winding);
void CheckForEarAndUpdateVertexType(AugmentedVertexList& augmentedVertices, AugmentedVertexList::iterator augmentedVertexIterator);
void RemoveEar(const Vector2* vertex, std::forward_list< AugmentedVertexList::iterator >& ears);
void ReclassifyVertex(AugmentedVertexList& augmentedVertices, AugmentedVertexList::iterator augmentedVertexIterator, PolygonWinding winding, std::forward_list< AugmentedVertexList::iterator >& ears);

void InternalTriangulate(AugmentedVertexList& augmentedVertices, PolygonWinding winding, std::vector<const Vector2*>& triangles);
void InternalTriangulate_R(AugmentedVertexList& augmentedVertices, std::forward_list< AugmentedVertexList::iterator >& ears, PolygonWinding winding, std::vector<const Vector2*>& triangles);

void AddTriangle(std::vector<const Vector2*>& triangles, const AugmentedVertexList::iterator& ear, bool last);
void AddRemainingTriangles(std::vector<const Vector2*>& triangles, AugmentedVertexList& augmentedVertices);
void AddTriangle_R(std::vector<const Vector2*>& triangles, const Vector2* trianglePoint1, std::list<const Vector2*>& collinearVertices1, 
                   const Vector2* trianglePoint2, std::list<const Vector2*>& collinearVertices2, const Vector2* trianglePoint3, std::list<const Vector2*>& collinearVertices3);

}

}
