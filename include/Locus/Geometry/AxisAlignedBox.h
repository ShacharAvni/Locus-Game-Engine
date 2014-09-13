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
#include "TriangleFwd.h"

#include <vector>
#include <array>

namespace Locus
{

class Sphere;
class Moveable;

/// An axis-aligned box
class LOCUS_GEOMETRY_API AxisAlignedBox
{
public:
   /*!
    * \details the minimum corner is set to (-1, -1, -1)
    * and the maximum corner is set to (1, 1, 1)
    */
   AxisAlignedBox();

   /*!
    * \param min the absolute coordinates of the corner
    * of the box with minimum x, y, and z.
    * \param max the absolute coordinates of the corner
    * of the box with maximum x, y, and z.
    */
   AxisAlignedBox(const Vector3& min, const Vector3& max);

   /*!
    * \brief Constructs an axis-aligned cube.
    *
    * \param center the absolute coordinates of the center
    * of the box.
    * \param diagonalLength the length of a space diagonal
    * of the box.
    *
    * \sa DiagonalLength
    */
   AxisAlignedBox(const Vector3& center, float diagonalLength);

   /*!
    * \param points the points in absolute coordinates whose
    * bounding box will be used to create the box.
    * \param tight if true, then the box will be constructed as the
    * axis-aligned bounding box of the input points. If false,
    * then the center of the box will be the centroid of the input
    * points and the length of the space diagonal will be the diameter
    * of the smallest sphere that encloses all the input points and is
    * centered at the centroid.
    */
   AxisAlignedBox(const std::vector<Vector3>& points, bool tight);

   /// \return true if the given point is within the box or is within a tolerance of one of the boundaries.
   bool Contains(const Vector3& point) const;

   /// \return true if the triangle does not intersect the box in any fashion.
   bool IsOutside(const Triangle3D_t& triangle) const;

   /// \return true if the triangle intersects the box, fully or partially.
   bool Intersects(const Triangle3D_t& triangle) const;

   /// \return true if this box intersects the given box, fully or partially.
   bool Intersects(const AxisAlignedBox& box) const;

   /*!
    * \brief Determines if this box intersects another box fully or partially
    * after transformations are applied to both boxes.
    *
    * \param thisMoveable contains the model transformation that will be applied
    * to this box.
    * \param other the box that we're querying for potential intersection with
    * this box.
    * \param otherMoveable contains the model transformation that will be applied
    * to the other box.
    *
    * \sa TransformBy
    */
   bool Intersects(const Moveable& thisMoveable, const AxisAlignedBox& other, const Moveable& otherMoveable) const;

   /// \return true if this box intersects the given sphere fully or partially.
   bool Intersects(const Sphere& sphere) const;

   /*!
    * \brief Transforms this box using the model transformation of
    * the given Moveable.
    *
    * \details The centroid of the box is first scaled, then rotated,
    * then translated according to the scale, rotation, and translation
    * components of the model transformation of the given Moveable. The
    * extents of the box are also scaled given the scale component of the
    * model transformation. Note that the box is not truly rotated or else
    * it would no longer be axis-aligned after the transformation.
    */
   void TransformBy(const Moveable& moveable);

   /*!
    * \brief Gets the two halves of this axis-aligned box that would
    * be made after cutting this box in two.
    *
    * \param minBox the axis-aligned box on the side that contains
    * the minimum corner of this box.
    * \param maxBox the axis-aligned box on the side that contains
    * the maximum corner of this box.
    * \param whichCoordinate the coordinate that will be split. For
    * instance, say that this box goes from -2 to +4 along X and that
    * X is passed as whichCoordinate. Then, minBox would be this box
    * between X = -2 and X = 1 inclusively, and maxBox would be this
    * box between X = 1 and X = 4 inclusively.
    */
   void Split(AxisAlignedBox& minBox, AxisAlignedBox& maxBox, Vector3::Coordinate whichCoordinate) const;

   /*!
    * \brief Gets the eight axis-aligned boxes of equal size that
    * would be made after cutting this box three times down the
    * middle (once along each dimension).
    *
    * \details octants[0] has the corner (min x, min y, min z),
    * octants[1] has the corner (min x, min y, max z),
    * octants[2] has the corner (min x, max y, min z),
    * octants[3] has the corner (min x, max y, max z),
    * octants[4] has the corner (max x, min y, min z),
    * octants[5] has the corner (max x, min y, max z),
    * octants[6] has the corner (max x, max y, min z),
    * octants[7] has the corner (max x, max y, max z),
    */
   void OctantSplit(std::array<AxisAlignedBox, 8>& octants) const;

   /*!
    * \brief Gets the lengths of this box along each dimension.
    *
    * \details extents[0] gets the length along X, extents[1]
    * gets the length along Y, and extents[2] gets the length
    * along Z.
    */
   void Extents(std::array<float, 3>& extents) const;

   /*!
    * \return the length of a space diagonal of this box.
    * That is, a line segment that passes through the center
    * of the box and that joins two opposite corners.
    */
   float DiagonalLength() const;

   /// \return the centroid of the box.
   Vector3 Centroid() const;

protected:
   Vector3 min;
   Vector3 max;
};

}