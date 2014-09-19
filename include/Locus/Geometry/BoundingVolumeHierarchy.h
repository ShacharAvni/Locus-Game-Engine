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

#include <cmath>

namespace Locus
{

class Moveable;
class OrientedBox;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

// Used internally by BoundingVolumeHierarchy
typedef std::unordered_map<std::size_t, Triangle3D_t> TriangleInputMap_t;

//{CodeReview:NarrowPhaseCollisions}
/*!
 * \brief A bounding volume hierarchy made from the octree
 * decomposition of a given model or triangles.
 *
 * \details Three template parameters are possible: (1)
 * Sphere means the hierarchy will be a Sphere Tree. (2)
 * AxisAlignedBox means the hierarchy will be an Axis-Aligned
 * Bounding Box Tree. (3) OrientedBox means the hierarchy will
 * be an Oriented Bounding Box Tree.
 */
template <class BoundingVolume>
class LOCUS_GEOMETRY_API BoundingVolumeHierarchy
{
public:
   static_assert(std::is_same<BoundingVolume, Sphere>::value || std::is_same<BoundingVolume, AxisAlignedBox>::value || std::is_same<BoundingVolume, OrientedBox>::value, "BoundingVolume must be Sphere or AxisAlignedBox or OrientedBox");

   /*!
    * \brief The max number of children each node in the
    * tree could have. This is eight, meaning
    * BoundingVolumeHierarchies are octrees.
    */
   static const std::size_t NUM_TREE_CHILDREN = 8;

   /*!
    * \return The first power of eight that is greater
    * than or equal to the number of faces in the given
    * model.
    *
    * \param[in] maxDepthClamp If this is non-zero then
    * the return value will be clamped to this value if
    * the return value is greater than it. If this is
    * zero then the return value is not touched.
    */
   template <class VertexIndexerType, class VertexType>
   static std::size_t GetMaxTreeDepth(const Model<VertexIndexerType, VertexType>& model, std::size_t maxDepthClamp = 0)
   {
      std::size_t numFaces = model.NumFaces();

      std::size_t maxDepth = 1;
      while (std::pow(NUM_TREE_CHILDREN, maxDepth) < numFaces)
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

   /*!
    * \param[in] maxDepthClamp Clamps the maximum depth of the resulting
    * tree. The max depth is determined by calling GetMaxTreeDepth on the
    * given model.
    *
    * \details The BoundingVolumeHierarchy is constructed the same
    * as BoundingVolumeHierarchy(const std::vector<Triangle3D_t>&, std::size_t, std::size_t)
    * given this model's identity face triangles.
    *
    * \note If the model's triangles change, then the BoundingVolumeHierarchy
    * should be recreated.
    *
    * \sa Model::GetIdentityFaceTriangles GetMaxTreeDepth BoundingVolumeHierarchy(const std::vector<Triangle3D_t>&, std::size_t, std::size_t)
    */
   template <class VertexIndexerType, class VertexType>
   BoundingVolumeHierarchy(const Model<VertexIndexerType, VertexType>& model, std::size_t leafTriangles, std::size_t maxDepthClamp = 0) 
   {
      std::vector<Triangle3D_t> identityFaceTriangles = model.GetIdentityFaceTriangles();

      ConstructFromTriangles(identityFaceTriangles, leafTriangles, BoundingVolumeHierarchy::GetMaxTreeDepth(model, maxDepthClamp));
   }

   /*!
    * \param[in] triangles The triangles to be contained in the
    * BoundingVolumeHierarchy.
    *
    * \param[in] leafTriangles If a bounding volume is encountered
    * with a number of triangles less than or equal to this value,
    * then that bounding volume is considered to be a leaf of the
    * tree.
    *
    * \param[in] maxDepth Bounding volumes are not subdivided
    * further if the decomposition reaches this depth.
    *
    * \details First a bounding volume is placed centered around
    * the centroid of the given triangles. This bounding volume
    * is set as the root of the tree. Next, this bounding volume
    * is split into eight axis-aligned octants. A triangle intersecting
    * an octant in any fashion is placed into that octant for further
    * processing. For each octant, a bounding volume is placed centered
    * around the centroid of the triangles falling into that octant. The
    * process continues recursively. If at any time the number of
    * triangles is less than or equal to the leafTriangles parameter,
    * then that node is considered to be a leaf and processing for that
    * branch terminates. The process along a branch terminates also if
    * the depth of that branch is equal to the maxDepth parameter.
    *
    * \note There is no attempt at optimal balancing.
    *
    * \note The resulting tree is not guaranteed to be a heap.
    */
   BoundingVolumeHierarchy(const std::vector<Triangle3D_t>& triangles, std::size_t leafTriangles, std::size_t maxDepth);

   BoundingVolumeHierarchy(const BoundingVolumeHierarchy& otherBoundingVolumeHierarchy);

   //{CodeReview:NarrowPhaseCollisions}
   /*!
    * \brief Gets the intersection of the triangles in this BoundingVolumeHierarchy
    * with the triangles in another BoundingVolumeHierarchy after transformations
    * are applied to both BoundingVoxlumeHierarchies.
    *
    * \param[in] thisMoveable Contains the model transformation that will be applied
    * to this BoundingVolumeHierarchy.
    *
    * \param[in] otherBoundingVolumeHierarchy The box BoundingVolumeHierarchy that
    * we're querying for potential intersection with this BoundingVolumeHierarchy.
    *
    * \param[in] otherMoveable Contains the model transformation that will be applied
    * to the other BoundingVolumeHierarchy.
    *
    * \param[out] thisIntersectionSet The indices of the triangles in this
    * BoundingVolumeHierarchy that intersected with any triangle of the other
    * BoundingVolumeHierarchy.
    *
    * \param[out] otherIntersectionSet The indices of the triangles in the other
    * BoundingVolumeHierarchy that intersected with any triangle of this
    * BoundingVolumeHierarchy.
    *
    * \note If the BoundingVolumeHierarchy has been constructed from a vector of
    * triangles, then the indices given in the intersection set correspond to the
    * indices of that vector. If the BoundingVolumeHierarchy has been constructed
    * from a model, then the indices given in the intersection set correspond to
    * the vector returned from Model::GetIdentityFaceTriangles assuming the triangles
    * were transformed with the model transformation of the Moveable provided. Also,
    * in the latter case, the indices correspond to those passed to Model::GetFaceTriangle
    * assuming that the model transformation of the Moveable provided is the same
    * as the model transformation of the model used in construction.
    */
   void GetIntersection(const Moveable& thisMoveable, const BoundingVolumeHierarchy<BoundingVolume>& otherBoundingVolumeHierarchy, const Moveable& otherMoveable, std::unordered_set<std::size_t>& thisIntersectionSet, std::unordered_set<std::size_t>& otherIntersectionSet) const;

   //{CodeReview:NarrowPhaseCollisions}
   /*!
    * \brief Gets the intersection of the triangles in this BoundingVolumeHierarchy
    * with the given OrientedBox.
    *
    * \param[in] thisMoveable Contains the model transformation that will be applied
    * to this BoundingVolumeHierarchy.
    *
    * \param[in] orientedBox The OrientedBox that we're querying for potential
    * intersection with this BoundingVolumeHierarchy.
    *
    * \param[out] thisIntersectionSet The indices of the triangles in this
    * BoundingVolumeHierarchy that intersected with the given OrientedBox in
    * any fashion.
    *
    * \note For a description of the indices given in the intersection set, see
    * the description of GetIntersection(const Moveable&, const BoundingVolumeHierarchy<BoundingVolume>&, const Moveable&, std::unordered_set<std::size_t>&, std::unordered_set<std::size_t>&) const
    *
    * \sa GetIntersection(const Moveable&, const BoundingVolumeHierarchy<BoundingVolume>&, const Moveable&, std::unordered_set<std::size_t>&, std::unordered_set<std::size_t>&) const
    */
   void GetIntersection(const Moveable& thisMoveable, const OrientedBox& orientedBox, std::unordered_set<std::size_t>& thisIntersectionSet) const;

private:
   struct Node
   {
      //{CodeReview:NarrowPhaseCollisions}
      Node(const TriangleInputMap_t& triangles, const BoundingVolume& boundingVolume, std::size_t leafTriangles, std::size_t currentDepth, std::size_t maxDepth);
      Node(const Node& otherNode);

      BoundingVolume boundingVolume;
      bool isLeaf;

      std::vector<std::size_t> containedTriangles;
      std::array<std::unique_ptr<Node>, NUM_TREE_CHILDREN> children;
   };

   void ConstructFromTriangles(const std::vector<Triangle3D_t>& triangles, std::size_t leafTriangles, std::size_t maxDepth);

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