 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusGeometryAPI.h"

#include "PolygonFwd.h"
#include "PolygonWinding.h"
#include "Vector2.h"
#include "Vector3.h"

#include <vector>
#include <list>
#include <memory>
#include <type_traits>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

template <class PolygonType>
class LOCUS_GEOMETRY_API PolygonHierarchy
{
public:
   static_assert(std::is_same<PolygonType, Polygon2D_t>::value || std::is_same<PolygonType, Polygon3D_t>::value, "PolygonType must be Polygon3D_t or Polygon2D_t");

   PolygonHierarchy(std::vector<PolygonType*>& polygons, PolygonWinding winding, const Vector3& normal, float toleranceFactor);

   struct Node
   {
      PolygonType* polygon;
      std::vector< std::unique_ptr<Node> > children;
   };

   const Node* Root() const;

private:
   std::unique_ptr<Node> root;

   static void DetermineTopLevelPolygonsAndTheirChildren(std::vector<PolygonType*>& polygons, float toleranceFactor, std::list<PolygonType*>& topLevelPolygons, std::list< std::vector<PolygonType*> >& children);
   static void FormHierarchyAtNode_R(std::unique_ptr<Node>& node, std::vector<PolygonType*>& polygons, PolygonWinding winding, const Vector3& normal, float toleranceFactor);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

#ifdef LOCUS_GEOMETRY_SHARED_IMPORTS

extern template class LOCUS_SHARED_IMPORTS PolygonHierarchy<Polygon2D_t>;
extern template class LOCUS_SHARED_IMPORTS PolygonHierarchy<Polygon3D_t>;

#endif

}