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

#include "Locus/Math/Vectors.h"

#include "AxisAlignedBox.h"
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
   OrientedBox(const FVector3& centroid, float xLength, float yLength, float zLength);
   OrientedBox(const std::vector<FVector3>& points);
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

   void AxesAndRotation(std::array<FVector3, 3>& axes, Transformation& rotation) const;

   Plane MaxSplitPlane() const;

   float DiagonalLength() const;

   FVector3 centroid;

private:
   Transformation rotationInverse;
};

}