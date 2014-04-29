 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Geometry/Collidable.h"

#include "Locus/Common/IDGenerator.h"

namespace Locus
{

Collidable::Collidable()
   : collidableID(BAD_ID)
{
}

Collidable::~Collidable()
{
}

unsigned int Collidable::GetCollidableType() const
{
   return collidableType;
}

Collidable& Collidable::operator=(const Collidable& other)
{
   if (this != &other)
   {
      collidableType = other.collidableType;
      broadCollisionExtentMin = other.broadCollisionExtentMin;
      broadCollisionExtentMax = other.broadCollisionExtentMax;
   }

   return *this;
}
   
void Collidable::UpdateBroadCollisionExtent(const Vector3& centroid, float radius)
{
   broadCollisionExtentMin.x = centroid.x - radius;
   broadCollisionExtentMax.x = centroid.x + radius;

   broadCollisionExtentMin.y = centroid.y - radius;
   broadCollisionExtentMax.y = centroid.y + radius;

   broadCollisionExtentMin.z = centroid.z - radius;
   broadCollisionExtentMax.z = centroid.z + radius;
}

bool Collidable::CollidesWith(Collidable& /*collidable*/) const
{
   return true;
}

void Collidable::HandleCollision(Collidable& collidable)
{
   if (CollidesWith(collidable))
   {
      ResolveCollision(collidable);
   }
}

}