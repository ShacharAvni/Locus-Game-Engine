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
   : collidableType(0)
{
}

Collidable::~Collidable()
{
}

unsigned int Collidable::GetCollidableType() const
{
   return collidableType;
}

const FVector3& Collidable::GetBroadCollisionExtentMin() const
{
   return broadCollisionExtentMin;
}

const FVector3& Collidable::GetBroadCollisionExtentMax() const
{
   return broadCollisionExtentMax;
}

void Collidable::UpdateBroadCollisionExtent(const FVector3& centroid, float halfLength)
{
   broadCollisionExtentMin.x = centroid.x - halfLength;
   broadCollisionExtentMax.x = centroid.x + halfLength;

   broadCollisionExtentMin.y = centroid.y - halfLength;
   broadCollisionExtentMax.y = centroid.y + halfLength;

   broadCollisionExtentMin.z = centroid.z - halfLength;
   broadCollisionExtentMax.z = centroid.z + halfLength;
}

bool Collidable::CollidesWith(Collidable& /*collidable*/) const
{
   return true;
}

}