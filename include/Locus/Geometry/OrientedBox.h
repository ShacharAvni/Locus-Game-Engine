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

#pragma once

#include "LocusGeometryAPI.h"

#include "AxisAlignedBox.h"
#include "Vector3.h"
#include "Transformation.h"
#include "TriangleFwd.h"
#include "Plane.h"

#include <vector>

namespace Locus
{

class Sphere;
class Moveable;

class LOCUS_GEOMETRY_API OrientedBox : private AxisAlignedBox
{
public:
   OrientedBox();
   OrientedBox(const Vector3& centroid, float xLength, float yLength, float zLength);
   OrientedBox(const std::vector<Vector3>& points);
   OrientedBox(const AxisAlignedBox& axisAlignedBox);

   void SetRotationInverse(const Transformation& rotationInverse);

   void SetZLength(float zLength);

   bool IsOutside(const Triangle3D_t& triangle) const;
   bool Intersects(const Triangle3D_t& triangle) const;

   void TransformBy(const Moveable& moveable);

   bool Intersects(const OrientedBox& box) const;

   bool Intersects(const Sphere& sphere, const Moveable& sphereMoveable) const;
   bool Intersects(const AxisAlignedBox& box, const Moveable& boxMoveable) const;
   bool Intersects(const OrientedBox& box, const Moveable& boxMoveable) const;

   bool Intersects(const Moveable& thisMoveable, const OrientedBox& other, const Moveable& otherMoveable) const;

   void AxesAndRotation(std::array<Vector3, 3>& axes, Transformation& rotation) const;

   Plane MaxSplitPlane() const;

   float DiagonalLength() const;

   Vector3 centroid;

private:
   Transformation rotationInverse;
};

}