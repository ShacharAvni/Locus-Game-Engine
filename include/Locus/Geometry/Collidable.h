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

namespace Locus
{

class LOCUS_GEOMETRY_API Collidable
{
public:
   Collidable();
   virtual ~Collidable();

   unsigned int GetCollidableType() const;

   const Vector3& GetBroadCollisionExtentMin() const;
   const Vector3& GetBroadCollisionExtentMax() const;

   virtual void UpdateBroadCollisionExtent() = 0;

   virtual bool CollidesWith(Collidable& collidable) const;

   virtual void ResolveCollision(Collidable& collidable) = 0;

private:
   Vector3 broadCollisionExtentMin;
   Vector3 broadCollisionExtentMax;

protected:
   unsigned int collidableType;

   void UpdateBroadCollisionExtent(const Vector3& centroid, float radius);
};

}