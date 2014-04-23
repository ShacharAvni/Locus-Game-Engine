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