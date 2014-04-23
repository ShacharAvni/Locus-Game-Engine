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

#include "Locus/Geometry/Sphere.h"
#include "Locus/Geometry/Moveable.h"
#include "Locus/Geometry/Geometry.h"

#include "Locus/Common/Util.h"

#include <unordered_set>
#include <algorithm>

#include <math.h>

namespace Locus
{

Sphere::Sphere()
   : radius(0.01f)
{
}

Sphere::Sphere(const Vector3& center, float radius)
   : center(center), radius(radius)
{
}

Sphere::Sphere(const std::vector<Vector3>& points)
   : radius(0.01f)
{
   std::size_t numPoints = points.size();

   if (numPoints > 0)
   {
      center = Vector3::ZeroVector();

      for (const Vector3& singlePoint : points)
      {
         center += singlePoint;
      }

      center /= static_cast<float>(numPoints);

      radius = 0;

      for (const Vector3& singlePoint : points)
      {
         radius = std::max(radius, (singlePoint - center).squaredNorm());
      }

      radius = sqrt(radius);
   }
}

bool Sphere::Intersects(const Moveable& thisMoveable, const Sphere& other, const Moveable& otherMoveable) const
{
   float radiiSum = (radius * thisMoveable.CurrentScale().x) + (other.radius * otherMoveable.CurrentScale().x);

   return (otherMoveable.CurrentModelTransformation().MultVertex(other.center) - thisMoveable.CurrentModelTransformation().MultVertex(center)).squaredNorm() <= (radiiSum * radiiSum);
}

float Sphere::Volume() const
{
   return ( (4.0f / 3.0f) * PI * radius * radius * radius );
}

}