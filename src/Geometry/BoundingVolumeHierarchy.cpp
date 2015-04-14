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

static std::vector<FVector3> GetUniquePointsFromTriangles(const std::vector<Triangle3D_t>& triangles)
{
   std::vector<FVector3> uniquePoints;

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

      std::vector<FVector3>::iterator uniqueEnd = std::unique(uniquePoints.begin(), uniquePoints.end(), [](const FVector3& first, const FVector3& second)->bool
      {
         return (first == second);
      });

      uniquePoints.resize( std::distance(uniquePoints.begin(), uniqueEnd) );
   }

   return uniquePoints;
}

static std::vector<FVector3> GetUniquePointsFromTriangles(const TriangleInputMap_t& triangles)
{
   std::vector<Triangle3D_t> trianglesAsVector;
   trianglesAsVector.reserve(triangles.size());

   for (const TriangleInputMap_t::value_type& triangle : triangles)
   {
      trianglesAsVector.push_back(triangle.second);
   }

   return GetUniquePointsFromTriangles(trianglesAsVector);
}

static TriangleInputMap_t MakeTriangleInputMap(const std::vector<Triangle3D_t>& triangles)
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

static FVector3 GetCenterOfBoundingVolume(const Sphere& boundingVolume)
{
   return boundingVolume.center;
}

static FVector3 GetCenterOfBoundingVolume(const AxisAlignedBox& boundingVolume)
{
   return boundingVolume.Centroid();
}

static FVector3 GetCenterOfBoundingVolume(const OrientedBox& boundingVolume)
{
   return boundingVolume.centroid;
}

template <class BoundingVolume>
static BoundingVolume InstantiateBoundingVolumeFromPoints(const std::vector<FVector3>& points)
{
   return BoundingVolume(points);
}

template <>
AxisAlignedBox InstantiateBoundingVolumeFromPoints(const std::vector<FVector3>& points)
{
   return AxisAlignedBox(points, false);
}

//{CodeReview:NarrowPhaseCollisions}
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

         const FVector3 center = GetCenterOfBoundingVolume(boundingVolume);

         const std::array<Plane, 3> splitPlanes = { Plane(center, Vec3D::XAxis()), Plane(center, Vec3D::YAxis()), Plane(center, Vec3D::ZAxis()) };
         std::array<Plane::IntersectionQuery, 3> intersectionQueries;

         std::array<bool, NUM_TREE_CHILDREN> inOctants;
         std::fill(inOctants.begin(), inOctants.end(), true);

         for (const std::unordered_map<std::size_t, Triangle3D_t>::value_type& containedTriangle : triangles)
         {
            for (unsigned int coordinate = 0; coordinate < 3; ++coordinate)
            {
               intersectionQueries[coordinate] = splitPlanes[coordinate].triangleIntersectionTest(containedTriangle.second);
            }

            DetermineOctants(intersectionQueries[0], intersectionQueries[1], intersectionQueries[2], inOctants);

            for (std::size_t childIndex = 0; childIndex < NUM_TREE_CHILDREN; ++childIndex)
            {
               if (inOctants[childIndex])
               {
                  trianglesInOctants[childIndex][containedTriangle.first] = containedTriangle.second;
                  break;
               }
            }
         }

         for (std::size_t childIndex = 0; childIndex < NUM_TREE_CHILDREN; ++childIndex)
         {
            if (!trianglesInOctants[childIndex].empty())
            {
               children[childIndex] = std::make_unique<Node>(trianglesInOctants[childIndex], InstantiateBoundingVolumeFromPoints<BoundingVolume>(GetUniquePointsFromTriangles(trianglesInOctants[childIndex])), leafTriangles, currentDepth + 1, maxDepth);
            }
         }
      }  
   }
}

template <class BoundingVolume>
BoundingVolumeHierarchy<BoundingVolume>::Node::Node(const typename BoundingVolumeHierarchy<BoundingVolume>::Node& otherNode)
      : boundingVolume(otherNode.boundingVolume), isLeaf(otherNode.isLeaf), containedTriangles(otherNode.containedTriangles)
{
   for (std::size_t octantIndex = 0; octantIndex < NUM_TREE_CHILDREN; ++octantIndex)
   {
      if (otherNode.children[octantIndex] != nullptr)
      {
         children[octantIndex] = std::make_unique<Node>(*otherNode.children[octantIndex]);
      }
   }
}

template <class BoundingVolume>
BoundingVolumeHierarchy<BoundingVolume>::BoundingVolumeHierarchy(const std::vector<Triangle3D_t>& triangles, std::size_t leafTriangles, std::size_t maxDepth)
{
   ConstructFromTriangles(triangles, leafTriangles, maxDepth);
}

template <class BoundingVolume>
BoundingVolumeHierarchy<BoundingVolume>::BoundingVolumeHierarchy(const BoundingVolumeHierarchy<BoundingVolume>& otherBoundingVolumeHierarchy)
   : root( std::make_unique<Node>(*otherBoundingVolumeHierarchy.root) )
{
}

template <class BoundingVolume>
void BoundingVolumeHierarchy<BoundingVolume>::ConstructFromTriangles(const std::vector<Triangle3D_t>& triangles, std::size_t leafTriangles, std::size_t maxDepth)
{
   TriangleInputMap_t triangleInputMap = MakeTriangleInputMap(triangles);
   std::vector<FVector3> rootPoints = GetUniquePointsFromTriangles(triangles);

   root = std::make_unique<Node>(triangleInputMap, InstantiateBoundingVolumeFromPoints<BoundingVolume>(rootPoints), leafTriangles, 0, maxDepth);
}

template <class BoundingVolume>
void BoundingVolumeHierarchy<BoundingVolume>::FinalizeIntersection(const Moveable& thisMoveable, const Moveable& otherMoveable, const std::vector<Node*>& thisRemainingNodes, const std::vector<Node*>& otherCheckList, std::unordered_set<std::size_t>& thisIntersectionSet)
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
         for (const Node* node : otherCheckList)
         {
            if (checkNode->boundingVolume.Intersects(thisMoveable, node->boundingVolume, otherMoveable))
            {
               for (std::unique_ptr<Node>& child : checkNode->children)
               {
                  if (child != nullptr)
                  {
                     remainingNodes.push(child.get());
                  }
               }

               break;
            }
         }
      }
   } while (!remainingNodes.empty());
}

template <class BoundingVolume>
void BoundingVolumeHierarchy<BoundingVolume>::GatherIntersectionsFromCheckList(const Node* node, const std::vector<Node*>& checkList, const Moveable& moveable, const Moveable& checkMoveable, std::vector<Node*>& hitList, std::unordered_set<std::size_t>& intersectionSet)
{
   for (const std::unique_ptr<Node>& child : node->children)
   {
      if (child != nullptr)
      {
         for (const Node* otherNode : checkList)
         {
            if (!otherNode->isLeaf)
            {
               if (child->boundingVolume.Intersects(moveable, otherNode->boundingVolume, checkMoveable))
               {
                  if (child->isLeaf)
                  {
                     for (std::size_t containedTriangle : child->containedTriangles)
                     {
                        intersectionSet.insert(containedTriangle);
                     }
                  }
                  else
                  {
                     hitList.push_back(child.get());
                  }
               }
            }
         }
      }
   }
}

//{CodeReview:NarrowPhaseCollisions}
template <class BoundingVolume>
void BoundingVolumeHierarchy<BoundingVolume>::GetIntersection(const Moveable& thisMoveable, const BoundingVolumeHierarchy<BoundingVolume>& otherBoundingVolumeHierarchy, const Moveable& otherMoveable, std::unordered_set<std::size_t>& thisIntersectionSet, std::unordered_set<std::size_t>& otherIntersectionSet) const
{
   if (root->boundingVolume.Intersects(thisMoveable, otherBoundingVolumeHierarchy.root->boundingVolume, otherMoveable))
   {
      std::vector<Node*> thisCurrentCheckList(1, root.get());
      std::vector<Node*> otherCurrentCheckList(1, otherBoundingVolumeHierarchy.root.get());

      std::vector<Node*> thisHitList;
      std::vector<Node*> otherHitList;

      while (!thisCurrentCheckList.empty() && !otherCurrentCheckList.empty())
      {
         thisHitList.clear();
         thisHitList.reserve(thisCurrentCheckList.size() * NUM_TREE_CHILDREN);

         otherHitList.clear();
         otherHitList.reserve(otherCurrentCheckList.size() * NUM_TREE_CHILDREN);

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
               GatherIntersectionsFromCheckList(node, otherCurrentCheckList, thisMoveable, otherMoveable, thisHitList, thisIntersectionSet);
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
               GatherIntersectionsFromCheckList(node, thisCurrentCheckList, otherMoveable, thisMoveable, otherHitList, otherIntersectionSet);
            }
         }

         if (thisHitList.empty() && !otherHitList.empty())
         {
            FinalizeIntersection(otherMoveable, thisMoveable, otherHitList, thisCurrentCheckList, otherIntersectionSet);
            break;
         }
         else if (otherHitList.empty() && !thisHitList.empty())
         {
            FinalizeIntersection(thisMoveable, otherMoveable, thisHitList, otherCurrentCheckList, thisIntersectionSet);
            break;
         }

         thisCurrentCheckList = std::move(thisHitList);
         otherCurrentCheckList = std::move(otherHitList);
      }
   }
}

//{CodeReview:NarrowPhaseCollisions}
template <class BoundingVolume>
void BoundingVolumeHierarchy<BoundingVolume>::GetIntersection(const Moveable& thisMoveable, const OrientedBox& orientedBox, std::unordered_set<std::size_t>& thisIntersectionSet) const
{
   if (orientedBox.Intersects(root->boundingVolume, thisMoveable))
   {
      std::vector<Node*> thisCurrentCheckList(1, root.get());

      while (!thisCurrentCheckList.empty())
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
               for (const std::unique_ptr<Node>& child : node->children)
               {
                  if (child != nullptr)
                  {
                     if (orientedBox.Intersects(child->boundingVolume, thisMoveable))
                     {
                        if (child->isLeaf)
                        {
                           for (std::size_t containedTriangle : child->containedTriangles)
                           {
                              thisIntersectionSet.insert(containedTriangle);
                           }
                        }
                        else
                        {
                           thisHitList.push_back(child.get());
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