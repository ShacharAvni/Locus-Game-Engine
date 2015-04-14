/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "CollidableMesh.h"

#include "Locus/Geometry/Geometry.h"
#include "Locus/Geometry/Plane.h"
#include "Locus/Geometry/Triangle.h"

#include "Locus/Rendering/RenderingState.h"

namespace Locus
{

namespace Examples
{

CollidableMesh::CollidableMesh()
   : lastCollision(nullptr)
{
   collidableType = My_Collidable_Type;
}

CollidableMesh::CollidableMesh(const CollidableMesh& other)
   :
   boundingVolumeHierarchy( std::make_unique<Locus::SphereTree_t>(*other.boundingVolumeHierarchy) ),
   lastCollision(other.lastCollision)
{
}

CollidableMesh& CollidableMesh::operator=(const CollidableMesh& other)
{
   if (this != &other)
   {
      boundingVolumeHierarchy = std::make_unique<Locus::SphereTree_t>(*other.boundingVolumeHierarchy);

      lastCollision = other.lastCollision;
      lastCollisionTime = other.lastCollisionTime;
   }

   return *this;
}

const Locus::SphereTree_t& CollidableMesh::GetBoundingVolumeHierarchy() const
{
   return *boundingVolumeHierarchy;
}

void CollidableMesh::GrabMesh(const Locus::Mesh& mesh)
{
   Mesh::CopyFrom(mesh);
}

void CollidableMesh::GrabMeshAndCollidable(const CollidableMesh& other)
{
   GrabMesh(other);
   Collidable::operator=(other);

   boundingVolumeHierarchy = std::make_unique<Locus::SphereTree_t>(*other.boundingVolumeHierarchy);
}

void CollidableMesh::UpdateBroadCollisionExtent()
{
   Collidable::UpdateBroadCollisionExtent(centroid, maxDistanceToCenter);
}

void CollidableMesh::CreateBoundingVolumeHierarchy()
{
   boundingVolumeHierarchy = std::make_unique<Locus::SphereTree_t>(*this, 6);
}

bool CollidableMesh::GetCollidableMeshIntersection(CollidableMesh& other,  Locus::Triangle3D_t& intersectingTriangle1, Locus::Triangle3D_t& intersectingTriangle2)
{
   std::unordered_set<std::size_t> thisIntersectionSet;
   std::unordered_set<std::size_t> otherIntersectionSet;

   boundingVolumeHierarchy->GetIntersection(*this, *other.boundingVolumeHierarchy, other, thisIntersectionSet, otherIntersectionSet);

   if ((thisIntersectionSet.size() > 0) && (otherIntersectionSet.size() > 0))
   {
      return GetResolvedCollision(other, CurrentModelTransformation(), other.CurrentModelTransformation(), thisIntersectionSet, otherIntersectionSet, intersectingTriangle1, intersectingTriangle2);
   }
   
   return false;
}

void CollidableMesh::ResolveCollision(Collidable& collidable)
{
   if (collidable.GetCollidableType() == CollidableMesh::My_Collidable_Type)
   {
      ResolveCollision( dynamic_cast<CollidableMesh&>(collidable) );
   }
}

void CollidableMesh::ResolveCollision(CollidableMesh& otherCollidableMesh)
{
   std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

   if (lastCollision == &otherCollidableMesh)
   {
      std::chrono::high_resolution_clock::duration diff = now - lastCollisionTime;
      if (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() < 500)
      {
         return;
      }
   }

   if (otherCollidableMesh.lastCollision == this)
   {
      std::chrono::high_resolution_clock::duration diff = now - otherCollidableMesh.lastCollisionTime;
      if (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() < 500)
      {
         return;
      }
   }

   Locus::Triangle3D_t intersectingTriangle1, intersectingTriangle2;

   if ( GetCollidableMeshIntersection(otherCollidableMesh, intersectingTriangle1, intersectingTriangle2) )
   {
      Locus::FVector3 collisionPoint = Locus::Triangle3D_t::ComputeCentroid(intersectingTriangle1, intersectingTriangle2);

      Locus::FVector3 impulseDirection = NormVector(intersectingTriangle1.Normal());

      Locus::ResolveCollision(1.0f, BoundingSphere(), otherCollidableMesh.BoundingSphere(), collisionPoint, impulseDirection,
                               motionProperties, otherCollidableMesh.motionProperties);

      lastCollision = &otherCollidableMesh;
      otherCollidableMesh.lastCollision = this;

      now = std::chrono::high_resolution_clock::now();

      lastCollisionTime = now;
      otherCollidableMesh.lastCollisionTime = now;
   }
}

void CollidableMesh::Tick(double DT)
{
   Translate((motionProperties.speed * motionProperties.direction) * static_cast<float>(DT));
   Rotate(motionProperties.angularSpeed * static_cast<float>(DT) * motionProperties.rotation);
}

}

}