 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusGeometryAPI.h"

#include "Vector3.h"

namespace Locus
{

typedef unsigned int ID_t;

class LOCUS_GEOMETRY_API Collidable
{
   friend class CollisionManager;

public:
   Collidable();
   virtual ~Collidable();

   Collidable& operator=(const Collidable& other);

   unsigned int GetCollidableType() const;

   virtual void UpdateBroadCollisionExtent() = 0;

   virtual bool CollidesWith(Collidable& collidable) const;

   virtual void ResolveCollision(Collidable& collidable) = 0;
   virtual void HandleCollision(Collidable& collidable);

private:
   Vector3 broadCollisionExtentMin;
   Vector3 broadCollisionExtentMax;

   ID_t collidableID;

protected:
   unsigned int collidableType;

   void UpdateBroadCollisionExtent(const Vector3& centroid, float radius);
};

}