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

#include "Locus/Geometry/Octree.h"

#include "Locus/Common/Float.h"

#include <limits>
#include <algorithm>

namespace Locus
{

Octree::Node::Node(const std::unordered_map<std::size_t, Triangle3D_t>& triangles, const AxisAlignedBox& box, std::size_t leafTriangles, std::size_t currentDepth, std::size_t maxDepth)
   : box(box), isLeaf(false)
{
   std::size_t numContainedTriangles = triangles.size();

   if (numContainedTriangles > 0)
   {
      containedTriangles.reserve(numContainedTriangles);
      for (const std::unordered_map<std::size_t, Triangle3D_t>::value_type& containedTriangle : triangles)
      {
         containedTriangles.push_back(containedTriangle.first);
      }

      if ((numContainedTriangles > leafTriangles) && (currentDepth < maxDepth))
      {
         std::array<AxisAlignedBox, 8> octants;
         box.OctantSplit(octants);

         for (int octantIndex = 0; octantIndex < 8; ++octantIndex)
         {
            std::unordered_map<std::size_t, Triangle3D_t> octantTriangles;

            for (const std::unordered_map<std::size_t, Triangle3D_t>::value_type& containedTriangle : triangles)
            {
               if (octants[octantIndex].Intersects(containedTriangle.second))
               {
                  octantTriangles[containedTriangle.first] = containedTriangle.second;
               }
            }

            children[octantIndex].reset(  new Node(octantTriangles, octants[octantIndex], leafTriangles, currentDepth + 1, maxDepth) );
         }
      }
      else
      {
         isLeaf = true;
      }
   }
   else
   {
      isLeaf = true;
   }
}

Octree::Octree(const std::vector<Triangle3D_t>& triangles, std::size_t leafTriangles, std::size_t maxDepth)
{
   std::size_t numTriangles = triangles.size();

   if (numTriangles > 0)
   {
      //find extents
      const float reallyBigNumber = std::numeric_limits<float>::max();
      const float reallySmallNumber = -reallyBigNumber;

      Vector3 min(reallyBigNumber, reallyBigNumber, reallyBigNumber);
      Vector3 max(reallySmallNumber, reallySmallNumber, reallySmallNumber);

      for (std::size_t triangleIndex = 0; triangleIndex < numTriangles; ++triangleIndex)
      {
         for (std::size_t vertexIndex = 0; vertexIndex < Triangle3D_t::NumPointsOnATriangle; ++vertexIndex)
         {
            Vector3 checkVertex = triangles[triangleIndex][vertexIndex];

            for (Vector3::Coordinate coordinate = Vector3::Coordinate_X; coordinate <= Vector3::Coordinate_Z; coordinate = static_cast<Vector3::Coordinate>(coordinate + 1))
            {
               min[coordinate] = Float::FMin<float>(min[coordinate], checkVertex[coordinate]);
               max[coordinate] = Float::FMax<float>(max[coordinate], checkVertex[coordinate]);
            }
         }
      }

      //turn extents into a cube

      //find biggest extent
      Vector3::Coordinate biggestCoordinate = Vector3::Coordinate_X;
      float biggestExtent = max[Vector3::Coordinate_X] - min[Vector3::Coordinate_X];

      for (Vector3::Coordinate coordinate = Vector3::Coordinate_Y; coordinate <= Vector3::Coordinate_Z; coordinate = static_cast<Vector3::Coordinate>(coordinate + 1))
      {
         float extent = max[coordinate] - min[coordinate];

         if (Float::FGreater<float>(extent, biggestExtent))
         {
            biggestCoordinate = coordinate;
            biggestExtent = extent;
         }
      }

      //stretch the other two extents
      for (Vector3::Coordinate coordinate = Vector3::Coordinate_X; coordinate <= Vector3::Coordinate_Z; coordinate = static_cast<Vector3::Coordinate>(coordinate + 1))
      {
         if (coordinate != biggestCoordinate)
         {
            float difference = biggestExtent - (max[coordinate] - min[coordinate]);
            min[coordinate] = min[coordinate] - (difference / 2);
            max[coordinate] = max[coordinate] + (difference / 2);
         }
      }

      //make octree root

      std::unordered_map<std::size_t, Triangle3D_t> rootTriangles;
      for (std::size_t triangleIndex = 0; triangleIndex < numTriangles; ++triangleIndex)
      {
         rootTriangles[triangleIndex] = triangles[triangleIndex];
      }

      root.reset( new Node(rootTriangles, AxisAlignedBox(min, max), leafTriangles, 0, maxDepth) );
   }
}

const Octree::Node* Octree::Root() const
{
   return root.get();
}

}