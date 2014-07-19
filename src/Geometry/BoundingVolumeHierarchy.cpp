/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/BoundingVolumeHierarchy.h"

#include <queue>

namespace Locus
{

static void EliminateOctants(const std::array<std::size_t, 4>& octantsToEliminate, std::array<bool, 8>& octants)
{
   for (std::size_t eliminatedOctant : octantsToEliminate)
   {
      octants[eliminatedOctant] = false;
   }
}

static void DetermineOctants(Plane::IntersectionQuery intersectionQueryX, Plane::IntersectionQuery intersectionQueryY, Plane::IntersectionQuery intersectionQueryZ, std::array<bool, 8>& octants)
{
   static const std::array<std::size_t, 4> positiveXOctants = {0, 1, 2, 3};
   static const std::array<std::size_t, 4> positiveYOctants = {2, 3, 6, 7};
   static const std::array<std::size_t, 4> positiveZOctants = {0, 2, 4, 6};

   static const std::array<std::size_t, 4> negativeXOctants = {4, 5, 6, 7};
   static const std::array<std::size_t, 4> negativeYOctants = {0, 1, 4, 5};
   static const std::array<std::size_t, 4> negativeZOctants = {1, 3, 5, 7};

   std::fill(octants.begin(), octants.end(), true);

   if (intersectionQueryX == Plane::IntersectionQuery::Positive)
   {
      EliminateOctants(negativeXOctants, octants);
   }
   else if (intersectionQueryX == Plane::IntersectionQuery::Negative)
   {
      EliminateOctants(positiveXOctants, octants);
   }

   if (intersectionQueryY == Plane::IntersectionQuery::Positive)
   {
      EliminateOctants(negativeYOctants, octants);
   }
   else if (intersectionQueryY == Plane::IntersectionQuery::Negative)
   {
      EliminateOctants(positiveYOctants, octants);
   }

   if (intersectionQueryZ == Plane::IntersectionQuery::Positive)
   {
      EliminateOctants(negativeZOctants, octants);
   }
   else if (intersectionQueryZ == Plane::IntersectionQuery::Negative)
   {
      EliminateOctants(positiveZOctants, octants);
   }
}

std::vector<Vector3> GetUniquePointsFromTriangles(const std::vector<Triangle3D_t>& triangles)
{
   std::vector<Vector3> uniquePoints;

   std::size_t numTriangles = triangles.size();

   if (numTriangles > 0)
   {
      uniquePoints.reserve(numTriangles * Triangle3D_t::NumPointsOnATriangle);

      for (const Triangle3D_t& triangle : triangles)
      {
         uniquePoints.push_back(triangle[0]);
         uniquePoints.push_back(triangle[1]);
         uniquePoints.push_back(triangle[2]);
      }

      std::sort(uniquePoints.begin(), uniquePoints.end());

      std::vector<Vector3>::iterator uniqueEnd = std::unique(uniquePoints.begin(), uniquePoints.end(), [](const Vector3& first, const Vector3& second)->bool
      {
         return first.PreciselyEqualTo(second);
      });

      uniquePoints.resize( std::distance(uniquePoints.begin(), uniqueEnd) );
   }

   return uniquePoints;
}

std::vector<Vector3> GetUniquePointsFromTriangles(const TriangleInputMap_t& triangles)
{
   std::vector<Triangle3D_t> trianglesAsVector;
   trianglesAsVector.reserve(triangles.size());

   for (const TriangleInputMap_t::value_type& triangle : triangles)
   {
      trianglesAsVector.push_back(triangle.second);
   }

   return GetUniquePointsFromTriangles(trianglesAsVector);
}

TriangleInputMap_t MakeTriangleInputMap(const std::vector<Triangle3D_t>& triangles)
{
   TriangleInputMap_t triangleInputMap;

   std::size_t triangleIndex = 0;
   for (const Triangle3D_t& singleTriangle : triangles)
   {
      triangleInputMap.emplace(triangleIndex, singleTriangle);
      ++triangleIndex;
   }

   return triangleInputMap;
}

static Vector3 GetCenterOfBoundingVolume(const Sphere& boundingVolume)
{
   return boundingVolume.center;
}

static Vector3 GetCenterOfBoundingVolume(const AxisAlignedBox& boundingVolume)
{
   return boundingVolume.Centroid();
}

static Vector3 GetCenterOfBoundingVolume(const OrientedBox& boundingVolume)
{
   return boundingVolume.centroid;
}

template <class BoundingVolume>
BoundingVolumeHierarchy<BoundingVolume>::Node::Node(const TriangleInputMap_t& triangles, const BoundingVolume& boundingVolume, std::size_t leafTriangles, std::size_t currentDepth, std::size_t maxDepth)
   : boundingVolume(boundingVolume)
{
   isLeaf = true;

   std::size_t numContainedTriangles = triangles.size();

   if (numContainedTriangles > 0)
   {
      containedTriangles.reserve(numContainedTriangles);
      for (const TriangleInputMap_t::value_type& containedTriangle : triangles)
      {
         containedTriangles.push_back(containedTriangle.first);
      }

      if ((numContainedTriangles > leafTriangles) && (currentDepth < maxDepth))
      {
         isLeaf = false;

         std::array<TriangleInputMap_t, 8> trianglesInOctants;

         const Vector3 center = GetCenterOfBoundingVolume(boundingVolume);

         const std::array<Plane, 3> splitPlanes = { Plane(center, Vector3::XAxis()), Plane(center, Vector3::YAxis()), Plane(center, Vector3::ZAxis()) };
         std::array<Plane::IntersectionQuery, 3> intersectionQueries;

         std::array<bool, Num_Tree_Children> inOctants = {true};

         for (const std::unordered_map<std::size_t, Triangle3D_t>::value_type& containedTriangle : triangles)
         {
            for (Vector3::Coordinate coordinate = Vector3::Coordinate_X; coordinate <= Vector3::Coordinate_Z; coordinate = static_cast<Vector3::Coordinate>(coordinate + 1))
            {
               intersectionQueries[coordinate] = splitPlanes[coordinate].triangleIntersectionTest(containedTriangle.second);
            }

            DetermineOctants(intersectionQueries[Vector3::Coordinate_X], intersectionQueries[Vector3::Coordinate_Y], intersectionQueries[Vector3::Coordinate_Z], inOctants);

            for (std::size_t childIndex = 0; childIndex < Num_Tree_Children; ++childIndex)
            {
               if (inOctants[childIndex])
               {
                  trianglesInOctants[childIndex][containedTriangle.first] = containedTriangle.second;
                  break;
               }
            }
         }

         for (std::size_t childIndex = 0; childIndex < Num_Tree_Children; ++childIndex)
         {
            if (trianglesInOctants[childIndex].size() > 0)
            {
               children[childIndex] = std::make_unique<Node>(trianglesInOctants[childIndex], BoundingVolumeHierarchy::InstantiateBoundingVolumeFromPoints(GetUniquePointsFromTriangles(trianglesInOctants[childIndex])), leafTriangles, currentDepth + 1, maxDepth);
            }
         }
      }  
   }
}

template <class BoundingVolume>
BoundingVolumeHierarchy<BoundingVolume>::Node::Node(const typename BoundingVolumeHierarchy<BoundingVolume>::Node& boundingVolumeHierarchyNode)
   : boundingVolume(boundingVolumeHierarchyNode.boundingVolume), isLeaf(boundingVolumeHierarchyNode.isLeaf), containedTriangles(boundingVolumeHierarchyNode.containedTriangles), children()
{
   for (std::size_t octantIndex = 0; octantIndex < Num_Tree_Children; ++octantIndex)
   {
      if (boundingVolumeHierarchyNode.children[octantIndex] != nullptr)
      {
         children[octantIndex] = std::make_unique<Node>(*boundingVolumeHierarchyNode.children[octantIndex]);
      }
   }
}

template <class BoundingVolume>
BoundingVolumeHierarchy<BoundingVolume>::BoundingVolumeHierarchy(const std::vector<Triangle3D_t>& triangles, std::size_t leafTriangles, std::size_t maxDepth)
{
   TriangleInputMap_t triangleInputMap = MakeTriangleInputMap(triangles);
   std::vector<Vector3> rootPoints = GetUniquePointsFromTriangles(triangles);

   root = std::make_unique<Node>(triangleInputMap, BoundingVolumeHierarchy::InstantiateBoundingVolumeFromPoints(rootPoints), leafTriangles, 0, maxDepth);
}

template <class BoundingVolume>
BoundingVolumeHierarchy<BoundingVolume>::BoundingVolumeHierarchy(const BoundingVolumeHierarchy<BoundingVolume>& otherBoundingVolumeHierarchy)
   : root( std::make_unique<Node>(*otherBoundingVolumeHierarchy.root) )
{
}

template <class BoundingVolume>
BoundingVolume BoundingVolumeHierarchy<BoundingVolume>::InstantiateBoundingVolumeFromPoints(const std::vector<Vector3>& points)
{
   return BoundingVolume(points);
}

template <>
AxisAlignedBox BoundingVolumeHierarchy<AxisAlignedBox>::InstantiateBoundingVolumeFromPoints(const std::vector<Vector3>& points)
{
   return AxisAlignedBox(points, false);
}

template <class BoundingVolume>
void BoundingVolumeHierarchy<BoundingVolume>::FinalizeIntersection(const Moveable& thisMoveable, const Moveable& otherMoveable, std::vector<Node*>& thisRemainingNodes, const std::vector<Node*>& otherCheckList, std::unordered_set<std::size_t>& thisIntersectionSet)
{
   std::queue<Node*> remainingNodes;
   for (Node* node : thisRemainingNodes)
   {
      remainingNodes.push(node);
   }

   do
   {
      Node* checkNode = remainingNodes.front();
      remainingNodes.pop();

      if (checkNode->isLeaf)
      {
         for (std::size_t containedTriangle : checkNode->containedTriangles)
         {
            thisIntersectionSet.insert(containedTriangle);
         }
      }
      else
      {
         bool intersectsOne = false;
         for (const Node* node : otherCheckList)
         {
            if (checkNode->boundingVolume.Intersects(thisMoveable, node->boundingVolume, otherMoveable))
            {
               intersectsOne = true;
               break;
            }
         }

         if (intersectsOne)
         {
            for (std::size_t childIndex = 0; childIndex < Num_Tree_Children; ++childIndex)
            {
               if (checkNode->children[childIndex] != nullptr)
               {
                  remainingNodes.push(checkNode->children[childIndex].get());
               }
            }
         }
      }
   } while (remainingNodes.size() > 0);
}

template <class BoundingVolume>
void BoundingVolumeHierarchy<BoundingVolume>::GetIntersection(const Moveable& thisMoveable, const BoundingVolumeHierarchy<BoundingVolume>& otherBoundingVolumeHierarchy, const Moveable& otherMoveable, std::unordered_set<std::size_t>& thisIntersectionSet, std::unordered_set<std::size_t>& otherIntersectionSet) const
{
   if (root->boundingVolume.Intersects(thisMoveable, otherBoundingVolumeHierarchy.root->boundingVolume, otherMoveable))
   {
      std::vector<Node*> thisCurrentCheckList(1, root.get());
      std::vector<Node*> otherCurrentCheckList(1, otherBoundingVolumeHierarchy.root.get());

      std::vector<Node*> thisHitList;
      std::vector<Node*> otherHitList;

      while ((thisCurrentCheckList.size() > 0) && (otherCurrentCheckList.size() > 0))
      {
         thisHitList.clear();
         thisHitList.reserve(thisCurrentCheckList.size() * Num_Tree_Children);

         otherHitList.clear();
         otherHitList.reserve(otherCurrentCheckList.size() * Num_Tree_Children);

         for (const Node* node : thisCurrentCheckList)
         {
            if (node->isLeaf)
            {
               for (std::size_t containedTriangle : node->containedTriangles)
               {
                  thisIntersectionSet.insert(containedTriangle);
               }
            }
            else
            {
               for (std::size_t childIndex = 0; childIndex < Num_Tree_Children; ++childIndex)
               {
                  if (node->children[childIndex] != nullptr)
                  {
                     for (const Node* otherNode : otherCurrentCheckList)
                     {
                        if (!otherNode->isLeaf)
                        {
                           if (node->children[childIndex]->boundingVolume.Intersects(thisMoveable, otherNode->boundingVolume, otherMoveable))
                           {
                              if (node->children[childIndex]->isLeaf)
                              {
                                 for (std::size_t containedTriangle : node->children[childIndex]->containedTriangles)
                                 {
                                    thisIntersectionSet.insert(containedTriangle);
                                 }
                              }
                              else
                              {
                                 thisHitList.push_back(node->children[childIndex].get());
                              }
                           }
                        }
                     }
                  }
               }
            }
         }

         for (const Node* node : otherCurrentCheckList)
         {
            if (node->isLeaf)
            {
               for (std::size_t containedTriangle : node->containedTriangles)
               {
                  otherIntersectionSet.insert(containedTriangle);
               }
            }
            else
            {
               for (std::size_t childIndex = 0; childIndex < Num_Tree_Children; ++childIndex)
               {
                  if (node->children[childIndex] != nullptr)
                  {
                     for (const Node* thisNode : thisCurrentCheckList)
                     {
                        if (!thisNode->isLeaf)
                        {
                           if (node->children[childIndex]->boundingVolume.Intersects(otherMoveable, thisNode->boundingVolume, thisMoveable))
                           {
                              if (node->children[childIndex]->isLeaf)
                              {
                                 for (std::size_t containedTriangle : node->children[childIndex]->containedTriangles)
                                 {
                                    otherIntersectionSet.insert(containedTriangle);
                                 }
                              }
                              else
                              {
                                 otherHitList.push_back(node->children[childIndex].get());
                              }
                           }
                        }
                     }
                  }
               }
            }
         }

         if ( (thisHitList.size() == 0) && (otherHitList.size() > 0) )
         {
            FinalizeIntersection(otherMoveable, thisMoveable, otherHitList, thisCurrentCheckList, otherIntersectionSet);
            break;
         }
         else if ( (otherHitList.size() == 0) && (thisHitList.size() > 0) )
         {
            FinalizeIntersection(thisMoveable, otherMoveable, thisHitList, otherCurrentCheckList, thisIntersectionSet);
            break;
         }

         thisCurrentCheckList = std::move(thisHitList);
         otherCurrentCheckList = std::move(otherHitList);
      }
   }
}

template <class BoundingVolume>
void BoundingVolumeHierarchy<BoundingVolume>::GetIntersection(const Moveable& thisMoveable, const OrientedBox& orientedBox, std::unordered_set<std::size_t>& thisIntersectionSet) const
{
   if (orientedBox.Intersects(root->boundingVolume, thisMoveable))
   {
      std::vector<Node*> thisCurrentCheckList(1, root.get());

      while (thisCurrentCheckList.size() > 0)
      {
         std::vector<Node*> thisHitList;
         thisHitList.reserve(thisCurrentCheckList.size() * 8);

         for (const Node* node : thisCurrentCheckList)
         {
            if (node->isLeaf)
            {
               for (std::size_t containedTriangle : node->containedTriangles)
               {
                  thisIntersectionSet.insert(containedTriangle);
               }
            }
            else
            {
               for (std::size_t childIndex = 0; childIndex < Num_Tree_Children; ++childIndex)
               {
                  if (node->children[childIndex] != nullptr)
                  {
                     if (orientedBox.Intersects(node->children[childIndex]->boundingVolume, thisMoveable))
                     {
                        if (node->children[childIndex]->isLeaf)
                        {
                           for (std::size_t containedTriangle : node->children[childIndex]->containedTriangles)
                           {
                              thisIntersectionSet.insert(containedTriangle);
                           }
                        }
                        else
                        {
                           thisHitList.push_back(node->children[childIndex].get());
                        }
                     }
                  }
               }
            }
         }

         thisCurrentCheckList = std::move(thisHitList);
      }
   }
}

template class LOCUS_GEOMETRY_API_AT_DEFINITION BoundingVolumeHierarchy<Sphere>;
template class LOCUS_GEOMETRY_API_AT_DEFINITION BoundingVolumeHierarchy<AxisAlignedBox>;
template class LOCUS_GEOMETRY_API_AT_DEFINITION BoundingVolumeHierarchy<OrientedBox>;

}