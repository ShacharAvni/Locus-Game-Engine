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
#include "TriangleFwd.h"

#include <vector>
#include <array>

namespace Locus
{

class Sphere;
class Moveable;

class LOCUS_GEOMETRY_API AxisAlignedBox
{
public:
   AxisAlignedBox();
   AxisAlignedBox(const Vector3& min, const Vector3& max);
   AxisAlignedBox(const Vector3& center, float diagonalLength);
   AxisAlignedBox(const std::vector<Vector3>& points, bool tight);
   virtual ~AxisAlignedBox();

   bool Contains(const Vector3& point) const;
   bool IsOutside(const Triangle3D_t& triangle) const;
   bool Intersects(const Triangle3D_t& triangle) const;

   bool Intersects(const AxisAlignedBox& box) const;
   bool Intersects(const Moveable& thisMoveable, const AxisAlignedBox& other, const Moveable& otherMoveable) const;
   bool Intersects(const Sphere& sphere) const;

   void TransformBy(const Moveable& moveable);

   void Split(AxisAlignedBox& minBox, AxisAlignedBox& maxBox, Vector3::Coordinate whichCoordinat) const;
   void OctantSplit(std::array<AxisAlignedBox, 8>& octants) const;

   void Extents(std::array<float, 3>& extents) const;

   float DiagonalLength() const;
   Vector3 Centroid() const;

protected:
   Vector3 min;
   Vector3 max;
};

}