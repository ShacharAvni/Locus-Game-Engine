/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

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

            children[octantIndex] = std::make_unique<Node>(octantTriangles, octants[octantIndex], leafTriangles, currentDepth + 1, maxDepth);
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

      FVector3 min(reallyBigNumber, reallyBigNumber, reallyBigNumber);
      FVector3 max(reallySmallNumber, reallySmallNumber, reallySmallNumber);

      for (std::size_t triangleIndex = 0; triangleIndex < numTriangles; ++triangleIndex)
      {
         for (std::size_t vertexIndex = 0; vertexIndex < Triangle3D_t::NumPointsOnATriangle; ++vertexIndex)
         {
            FVector3 checkVertex = triangles[triangleIndex][vertexIndex];

            for (unsigned int coordinate = 0; coordinate < 3; ++coordinate)
            {
               min[coordinate] = std::min(min[coordinate], checkVertex[coordinate]);
               max[coordinate] = std::max(max[coordinate], checkVertex[coordinate]);
            }
         }
      }

      //turn extents into a cube

      //find biggest extent
      unsigned int biggestCoordinate = 0;
      float biggestExtent = max[biggestCoordinate] - min[biggestCoordinate];

      for (unsigned int coordinate = 1; coordinate < 3; ++coordinate)
      {
         float extent = max[coordinate] - min[coordinate];

         if (FGreater<float>(extent, biggestExtent))
         {
            biggestCoordinate = coordinate;
            biggestExtent = extent;
         }
      }

      //stretch the other two extents
      for (unsigned int coordinate = 0; coordinate < 3; ++coordinate)
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

      root = std::make_unique<Node>(rootTriangles, AxisAlignedBox(min, max), leafTriangles, 0, maxDepth);
   }
}

const Octree::Node* Octree::Root() const
{
   return root.get();
}

}