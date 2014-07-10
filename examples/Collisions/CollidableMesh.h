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

#include "Locus/Geometry/Moveable.h"
#include "Locus/Geometry/Collidable.h"
#include "Locus/Geometry/MotionProperties.h"
#include "Locus/Geometry/TriangleFwd.h"

#include "Locus/Rendering/Mesh.h"

#include "Locus/Geometry/BoundingVolumeHierarchy.h"

#include <chrono>

namespace Locus
{

class RenderingState;
class Texture;
class SoundEffect;

namespace Examples
{

class CollidableMesh : public Locus::Mesh, public Locus::Collidable
{
public:
   CollidableMesh();
   CollidableMesh(const CollidableMesh& other);
   CollidableMesh& operator=(const CollidableMesh& other);

   const Locus::SphereTree_t& GetBoundingVolumeHierarchy() const;

   void GrabMesh(const Locus::Mesh& mesh);
   void GrabMeshAndCollidable(const CollidableMesh& other);

   virtual void UpdateBroadCollisionExtent();

   void CreateBoundingVolumeHierarchy();

   bool GetCollidableMeshIntersection(CollidableMesh& other, Locus::Triangle3D_t& intersectingTriangle1, Locus::Triangle3D_t& intersectingTriangle2);

   virtual void ResolveCollision(Collidable& collidable) override;
   void ResolveCollision(CollidableMesh& otherCollidableMesh);

   void Tick(double DT);

   Locus::MotionProperties motionProperties;

private:
   std::unique_ptr< Locus::SphereTree_t > boundingVolumeHierarchy;

   //HACK: avoiding interpenetration
   Locus::Collidable* lastCollision;
   std::chrono::high_resolution_clock::time_point lastCollisionTime;

   static const unsigned int My_Collidable_Type = 0;
};

}

}