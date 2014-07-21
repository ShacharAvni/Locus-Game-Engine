/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Collidable.h"

namespace Locus
{

Collidable::Collidable()
{
}

Collidable::~Collidable()
{
}

unsigned int Collidable::GetCollidableType() const
{
   return collidableType;
}

const Vector3& Collidable::GetBroadCollisionExtentMin() const
{
   return broadCollisionExtentMin;
}

const Vector3& Collidable::GetBroadCollisionExtentMax() const
{
   return broadCollisionExtentMax;
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

}