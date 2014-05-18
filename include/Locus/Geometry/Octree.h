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

#include "AxisAlignedBox.h"
#include "Triangle.h"

#include <vector>
#include <array>
#include <unordered_map>
#include <memory>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_GEOMETRY_API Octree
{
public:
   struct Node
   {
      Node(const std::unordered_map<std::size_t, Triangle3D_t>& triangles, const AxisAlignedBox& box, std::size_t leafTriangles, std::size_t currentDepth, std::size_t maxDepth);

      AxisAlignedBox box;
      std::vector<std::size_t> containedTriangles;
      std::array<std::unique_ptr<Node>, 8> children;
      bool isLeaf;
   };

   Octree(const std::vector<Triangle3D_t>& triangles, std::size_t leafTriangles, std::size_t maxDepth);

   const Node* Root() const;

private:
   std::unique_ptr<Node> root;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}