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

#include "Vector3.h"
#include "Triangle.h"
#include "Model.h"
#include "Plane.h"

#include "Sphere.h"
#include "AxisAlignedBox.h"
#include "OrientedBox.h"

#include <vector>
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <type_traits>

#include <math.h>

namespace Locus
{

typedef std::unordered_map<std::size_t, Triangle3D_t> TriangleInputMap_t;

LOCUS_GEOMETRY_API std::vector<Vector3> GetUniquePointsFromTriangles(const std::vector<Triangle3D_t>& triangles);
LOCUS_GEOMETRY_API std::vector<Vector3> GetUniquePointsFromTriangles(const TriangleInputMap_t& triangles);

LOCUS_GEOMETRY_API TriangleInputMap_t MakeTriangleInputMap(const std::vector<Triangle3D_t>& triangles);

class Moveable;
class OrientedBox;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

//{CodeReview:NarrowPhaseCollisions}
template <class BoundingVolume>
class LOCUS_GEOMETRY_API BoundingVolumeHierarchy
{
public:
   static_assert(std::is_same<BoundingVolume, Sphere>::value || std::is_same<BoundingVolume, AxisAlignedBox>::value || std::is_same<BoundingVolume, OrientedBox>::value, "BoundingVolume must be Sphere or AxisAlignedBox or OrientedBox");

   static const std::size_t Num_Tree_Children = 8;

   template <class VertexIndexerType, class VertexType>
   static std::size_t GetMaxTreeDepth(const Model<VertexIndexerType, VertexType>& model, std::size_t maxDepthClamp = 0)
   {
      std::size_t numFaces = model.NumFaces();

      std::size_t maxDepth = 1;
      while (pow(Num_Tree_Children, maxDepth) < numFaces)
      {
         ++maxDepth;
      }

      if (maxDepthClamp == 0)
      {
         return maxDepth;
      }
      else
      {
         return std::min(maxDepth, maxDepthClamp);
      }
   }

   struct LOCUS_GEOMETRY_API Node
   {
      Node(const TriangleInputMap_t& triangles, const BoundingVolume& boundingVolume, std::size_t leafTriangles, std::size_t currentDepth, std::size_t maxDepth);
      Node(const Node& boundingVolumeHierarchyNode);

      BoundingVolume boundingVolume;
      bool isLeaf;

      std::vector<std::size_t> containedTriangles;
      std::array<std::unique_ptr<Node>, Num_Tree_Children> children;
   };

   template <class VertexIndexerType, class VertexType>
   BoundingVolumeHierarchy(const Model<VertexIndexerType, VertexType>& model, std::size_t leafTriangles, std::size_t maxDepthClamp = 0) 
   {
      std::vector<Triangle3D_t> identityFaceTriangles = model.GetIdentityFaceTriangles();

      TriangleInputMap_t triangleInputMap = MakeTriangleInputMap(identityFaceTriangles);
      std::vector<Vector3> rootPoints = GetUniquePointsFromTriangles(identityFaceTriangles);

      root = std::make_unique<Node>(triangleInputMap, BoundingVolumeHierarchy::InstantiateBoundingVolumeFromPoints(rootPoints), leafTriangles, 0, BoundingVolumeHierarchy::GetMaxTreeDepth(model, maxDepthClamp));
   }

   BoundingVolumeHierarchy(const std::vector<Triangle3D_t>& triangles, std::size_t leafTriangles, std::size_t maxDepth);
   BoundingVolumeHierarchy(const BoundingVolumeHierarchy& otherBoundingVolumeHierarchy);

   void GetIntersection(const Moveable& thisMoveable, const BoundingVolumeHierarchy<BoundingVolume>& otherBoundingVolumeHierarchy, const Moveable& otherMoveable, std::unordered_set<std::size_t>& thisIntersectionSet, std::unordered_set<std::size_t>& otherIntersectionSet) const;
   void GetIntersection(const Moveable& thisMoveable, const OrientedBox& orientedBox, std::unordered_set<std::size_t>& thisIntersectionSet) const;

private:
   static BoundingVolume InstantiateBoundingVolumeFromPoints(const std::vector<Vector3>& points);

   static void GatherIntersectionsFromCheckList(const Node* node, const std::vector<Node*>& checkList, const Moveable& moveable, const Moveable& checkMoveable, std::vector<Node*>& hitList, std::unordered_set<std::size_t>& intersectionSet);

   static void FinalizeIntersection(const Moveable& thisMoveable, const Moveable& otherMoveable, const std::vector<Node*>& thisRemainingNodes, const std::vector<Node*>& otherCheckList, std::unordered_set<std::size_t>& thisIntersectionSet);

   std::unique_ptr<Node> root;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

#ifdef LOCUS_GEOMETRY_SHARED_IMPORTS

extern template class LOCUS_SHARED_IMPORTS BoundingVolumeHierarchy<Sphere>;
extern template class LOCUS_SHARED_IMPORTS BoundingVolumeHierarchy<AxisAlignedBox>;
extern template class LOCUS_SHARED_IMPORTS BoundingVolumeHierarchy<OrientedBox>;

#endif

typedef BoundingVolumeHierarchy<Sphere> SphereTree_t;
typedef BoundingVolumeHierarchy<AxisAlignedBox> AABBTree_t;
typedef BoundingVolumeHierarchy<OrientedBox> OBBTree_t;

}