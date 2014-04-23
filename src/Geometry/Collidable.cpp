 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

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