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

#include "Locus/Geometry/OrientedBox.h"
#include "Locus/Geometry/Moveable.h"
#include "Locus/Geometry/Triangle.h"
#include "Locus/Geometry/Sphere.h"

#include "Locus/Common/Float.h"

#include <limits>

namespace Locus
{

OrientedBox::OrientedBox()
{
}

OrientedBox::OrientedBox(const Vector3& centroid, float xLength, float yLength, float zLength)
   : centroid(centroid)
{
   min.set(-xLength / 2, -yLength / 2, -zLength / 2);
   max.set(xLength / 2, yLength / 2, zLength / 2);
}

OrientedBox::OrientedBox(const std::vector<Vector3>& points)
{
   std::size_t numPoints = points.size();

   if (numPoints > 1)
   {
      centroid.set(0.0f, 0.0f, 0.0f);

      float xxExpectedValue = 0.0f;
      float yyExpectedValue = 0.0f;
      float zzExpectedValue = 0.0f;
      float xyExpectedValue = 0.0f;
      float xzExpectedValue = 0.0f;
      float yzExpectedValue = 0.0f;

      for (const Vector3& singlePoint : points)
      {
         centroid += singlePoint;

         xxExpectedValue += (singlePoint.x * singlePoint.x);
         yyExpectedValue += (singlePoint.y * singlePoint.y);
         zzExpectedValue += (singlePoint.z * singlePoint.z);

         xyExpectedValue += (singlePoint.x * singlePoint.y);
         xzExpectedValue += (singlePoint.x * singlePoint.z);
         yzExpectedValue += (singlePoint.y * singlePoint.z);
      }

      centroid /= static_cast<float>(numPoints);

      xyExpectedValue /= numPoints;
      xzExpectedValue /= numPoints;
      yzExpectedValue /= numPoints;

      float xxEntry = xxExpectedValue - (centroid.x * centroid.x);
      float yyEntry = yyExpectedValue - (centroid.y * centroid.y);
      float zzEntry = zzExpectedValue - (centroid.z * centroid.z);
      float xyEntry = xyExpectedValue - (centroid.x * centroid.y);
      float xzEntry = xzExpectedValue - (centroid.x * centroid.z);
      float yzEntry = yzExpectedValue - (centroid.y * centroid.z);

      SquareMatrix<float> covarianceMatrix(3, { xxEntry, xyEntry, xzEntry,
                                                xyEntry, yyEntry, yzEntry,
                                                xzEntry, yzEntry, zzEntry });

      Transformation rotation;

      std::vector<std::vector<float>> eigenvectors;

      if (covarianceMatrix.SolveEigenvectors(eigenvectors))
      {
         Vector3 columnInRotationMatrix(eigenvectors[0][0], eigenvectors[0][1], eigenvectors[0][2]);
         columnInRotationMatrix.normalize();

         rotation(0, 0) = columnInRotationMatrix.x;
         rotation(1, 0) = columnInRotationMatrix.y;
         rotation(2, 0) = columnInRotationMatrix.z;

         columnInRotationMatrix.set(eigenvectors[1][0], eigenvectors[1][1], eigenvectors[1][2]);
         columnInRotationMatrix.normalize();

         rotation(0, 1) = columnInRotationMatrix.x;
         rotation(1, 1) = columnInRotationMatrix.y;
         rotation(2, 1) = columnInRotationMatrix.z;

         columnInRotationMatrix.set(eigenvectors[2][0], eigenvectors[2][1], eigenvectors[2][2]);
         columnInRotationMatrix.normalize();

         rotation(0, 2) = columnInRotationMatrix.x;
         rotation(1, 2) = columnInRotationMatrix.y;
         rotation(2, 2) = columnInRotationMatrix.z;
      }

      rotationInverse = rotation.TransposedMatrix();

      Vector3 rotatedXAxis = rotation.MultVector(Vector3::XAxis());
      Vector3 rotatedYAxis = rotation.MultVector(Vector3::YAxis());
      Vector3 rotatedZAxis = rotation.MultVector(Vector3::ZAxis());

      float maxFloat = std::numeric_limits<float>::max();

      Vector3 minProjections(maxFloat, maxFloat, maxFloat);
      Vector3 maxProjections(-maxFloat, -maxFloat, -maxFloat);

      for (const Vector3& singlePoint : points)
      {
         float projected = rotatedXAxis.dot(singlePoint - centroid);

         if (projected < minProjections.x)
         {
            minProjections.x = projected;
         }
         if (projected > maxProjections.x)
         {
            maxProjections.x = projected;
         }

         projected = rotatedYAxis.dot(singlePoint - centroid);

         if (projected < minProjections.y)
         {
            minProjections.y = projected;
         }
         if (projected > maxProjections.y)
         {
            maxProjections.y = projected;
         }

         projected = rotatedZAxis.dot(singlePoint - centroid);

         if (projected < minProjections.z)
         {
            minProjections.z = projected;
         }
         if (projected > maxProjections.z)
         {
            maxProjections.z = projected;
         }
      }

      min = minProjections;
      max = maxProjections;
   }
}

OrientedBox::OrientedBox(const AxisAlignedBox& axisAlignedBox)
{
   std::array<float, 3> extents;
   axisAlignedBox.Extents(extents);

   centroid = axisAlignedBox.Centroid();

   min.set(-extents[Vector3::Coordinate_X] / 2, -extents[Vector3::Coordinate_Y] / 2, -extents[Vector3::Coordinate_Z] / 2);
   max.set(extents[Vector3::Coordinate_X] / 2, extents[Vector3::Coordinate_Y] / 2, extents[Vector3::Coordinate_Z] / 2);
}

void OrientedBox::SetRotationInverse(const Transformation& rotationInverse)
{
   this->rotationInverse = rotationInverse;
}

void OrientedBox::SetZLength(float zLength)
{
   min.z = -zLength / 2;
   max.z = zLength / 2;
}

bool OrientedBox::IsOutside(const Triangle3D_t& triangle) const
{
   Triangle3D_t triangleInAxisAlignedSpace( rotationInverse.MultVertex(triangle[0] - centroid),
                                            rotationInverse.MultVertex(triangle[1] - centroid),
                                            rotationInverse.MultVertex(triangle[2] - centroid) );

   return AxisAlignedBox::IsOutside(triangleInAxisAlignedSpace);
}

bool OrientedBox::Intersects(const Triangle3D_t& triangle) const
{
   return !IsOutside(triangle);
}

void OrientedBox::AxesAndRotation(std::array<Vector3, 3>& axes, Transformation& rotation) const
{
   rotation = rotationInverse;
   rotation.Transpose();

   axes[Vector3::Coordinate_X] = rotation.MultVector(Vector3::XAxis());
   axes[Vector3::Coordinate_Y] = rotation.MultVector(Vector3::YAxis());
   axes[Vector3::Coordinate_Z] = rotation.MultVector(Vector3::ZAxis());
}

Plane OrientedBox::MaxSplitPlane() const
{
   std::array<Vector3, 3> axes;
   Transformation rotation;

   AxesAndRotation(axes, rotation);

   std::array<float, 3> extents;

   Extents(extents);

   Vector3::Coordinate maxExtentsCoordinate = Vector3::Coordinate_X;
   float maxExtent = extents[Vector3::Coordinate_X];

   for (Vector3::Coordinate coordinate = Vector3::Coordinate_Y; coordinate <= Vector3::Coordinate_Z; coordinate = static_cast<Vector3::Coordinate>(coordinate + 1))
   {
      float thisExtent = extents[coordinate];

      if (thisExtent > maxExtent)
      {
         maxExtent = thisExtent;
         maxExtentsCoordinate = coordinate;
      }
   }

   return Plane(centroid, axes[maxExtentsCoordinate]);
}

float OrientedBox::DiagonalLength() const
{
   return AxisAlignedBox::DiagonalLength();
}

void OrientedBox::TransformBy(const Moveable& moveable)
{
   centroid = moveable.CurrentModelTransformation().MultVertex(centroid);

   rotationInverse = (rotationInverse * moveable.CurrentRotation().TransposedMatrix());

   float scale = moveable.CurrentScale().x;

   min *= scale;
   max *= scale;
}

bool OrientedBox::Intersects(const OrientedBox& box) const
{
   //http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=5

   Transformation thisRotation;
   std::array<Vector3, 3> thisAxes;
   AxesAndRotation(thisAxes, thisRotation);

   Transformation thatRotation;
   std::array<Vector3, 3> thatAxes;
   box.AxesAndRotation(thatAxes, thatRotation);

   std::array<float, 3> thisExtents;
   Extents(thisExtents);

   std::array<float, 3> thatExtents;
   box.Extents(thatExtents);

   float dotProducts[3][3];

   for (Vector3::Coordinate thisCoordinate = Vector3::Coordinate_X; thisCoordinate <= Vector3::Coordinate_Z; thisCoordinate = static_cast<Vector3::Coordinate>(thisCoordinate + 1))
   {
      for (Vector3::Coordinate thatCoordinate = Vector3::Coordinate_X; thatCoordinate <= Vector3::Coordinate_Z; thatCoordinate = static_cast<Vector3::Coordinate>(thatCoordinate + 1))
      {
         dotProducts[thisCoordinate][thatCoordinate] = thisAxes[thisCoordinate].dot(thatAxes[thatCoordinate]);
      }
   }

   Vector3 parentFrameTranslation = box.centroid - centroid;

   Vector3 thisFrameTranslation( parentFrameTranslation.dot(thisAxes[0]),
                                    parentFrameTranslation.dot(thisAxes[1]),
                                    parentFrameTranslation.dot(thisAxes[2]) );

   float thisRadiusProjection = 0.0f;
   float thatRadiusProjection = 0.0f;
   float projectionThreshold = 0.0f;

   //projections onto this axes
   for (Vector3::Coordinate thisCoordinate = Vector3::Coordinate_X; thisCoordinate <= Vector3::Coordinate_Z; thisCoordinate = static_cast<Vector3::Coordinate>(thisCoordinate + 1))
   {
      thisRadiusProjection = thisExtents[thisCoordinate];

      thatRadiusProjection = (thatExtents[0] * fabs(dotProducts[thisCoordinate][0])) +
                              (thatExtents[1] * fabs(dotProducts[thisCoordinate][1])) +
                              (thatExtents[2] * fabs(dotProducts[thisCoordinate][2]));

      projectionThreshold = fabs( thisFrameTranslation[thisCoordinate] );

      if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
      {
         return false;
      }
   }

   //projections onto that axes
   for (Vector3::Coordinate thatCoordinate = Vector3::Coordinate_X; thatCoordinate <= Vector3::Coordinate_Z; thatCoordinate = static_cast<Vector3::Coordinate>(thatCoordinate + 1))
   {
      thisRadiusProjection = (thisExtents[0] * fabs(dotProducts[0][thatCoordinate])) +
                              (thisExtents[1] * fabs(dotProducts[1][thatCoordinate])) +
                              (thisExtents[2] * fabs(dotProducts[2][thatCoordinate]));

      thatRadiusProjection = thatExtents[thatCoordinate];

      if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
      {
         return false;
      }
   }

   //9 cross products

   //this_XAxis x that_XAxis
   thisRadiusProjection = thisExtents[1] * fabs(dotProducts[2][0]) + thisExtents[2] * fabs(dotProducts[1][0]);
   thatRadiusProjection = thatExtents[1] * fabs(dotProducts[0][2]) + thatExtents[2] * fabs(dotProducts[0][1]);
   projectionThreshold = fabs( (thisFrameTranslation[Vector3::Coordinate_Z] * dotProducts[1][0]) - (thisFrameTranslation[Vector3::Coordinate_Y] * dotProducts[2][0]) );

   if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
   {
      return false;
   }

   //this_XAxis x that_YAxis
   thisRadiusProjection = thisExtents[1] * fabs(dotProducts[2][1]) + thisExtents[2] * fabs(dotProducts[1][1]);
   thatRadiusProjection = thatExtents[0] * fabs(dotProducts[0][2]) + thatExtents[2] * fabs(dotProducts[0][0]);
   projectionThreshold = fabs( (thisFrameTranslation[Vector3::Coordinate_Z] * dotProducts[1][1]) - (thisFrameTranslation[Vector3::Coordinate_Y] * dotProducts[2][1]) );

   if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
   {
      return false;
   }

   //this_XAxis x that_ZAxis
   thisRadiusProjection = thisExtents[1] * fabs(dotProducts[2][2]) + thisExtents[2] * fabs(dotProducts[1][2]);
   thatRadiusProjection = thatExtents[0] * fabs(dotProducts[0][1]) + thatExtents[1] * fabs(dotProducts[0][0]);
   projectionThreshold = fabs( thisFrameTranslation[Vector3::Coordinate_Z] * dotProducts[1][2] - thisFrameTranslation[Vector3::Coordinate_Y] * dotProducts[2][2] );

   if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
   {
      return false;
   }

   //this_YAxis x that_XAxis
   thisRadiusProjection = thisExtents[0] * fabs(dotProducts[2][0]) + thisExtents[2] * fabs(dotProducts[0][0]);
   thatRadiusProjection = thatExtents[1] * fabs(dotProducts[1][2]) + thatExtents[2] * fabs(dotProducts[1][1]);
   projectionThreshold = fabs( thisFrameTranslation[Vector3::Coordinate_X] * dotProducts[2][0] - thisFrameTranslation[Vector3::Coordinate_Z]*dotProducts[0][0] );

   if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
   {
      return false;
   }

   //this_YAxis x that_YAxis
   thisRadiusProjection = thisExtents[0] * fabs(dotProducts[2][1]) + thisExtents[2] * fabs(dotProducts[0][1]);
   thatRadiusProjection = thatExtents[0] * fabs(dotProducts[1][2]) + thatExtents[2] * fabs(dotProducts[1][0]);
   projectionThreshold = fabs( thisFrameTranslation[Vector3::Coordinate_X] * dotProducts[2][1] - thisFrameTranslation[Vector3::Coordinate_Z] * dotProducts[0][1] );

   if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
   {
      return false;
   }

   //this_YAxis x that_ZAxis
   thisRadiusProjection = thisExtents[0] * fabs(dotProducts[2][2]) + thisExtents[2] * fabs(dotProducts[0][2]);
   thatRadiusProjection = thatExtents[0] * fabs(dotProducts[1][1]) + thatExtents[1] * fabs(dotProducts[1][0]);
   projectionThreshold = fabs( thisFrameTranslation[Vector3::Coordinate_X] * dotProducts[2][2] - thisFrameTranslation[Vector3::Coordinate_Z] * dotProducts[0][2] );

   if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
   {
      return false;
   }

   //this_ZAxis x that_XAxis
   thisRadiusProjection = thisExtents[0] * fabs(dotProducts[1][0]) + thisExtents[1] * fabs(dotProducts[0][0]);
   thatRadiusProjection = thatExtents[1] * fabs(dotProducts[2][2]) + thatExtents[2] * fabs(dotProducts[2][1]);
   projectionThreshold = fabs( thisFrameTranslation[Vector3::Coordinate_Y] * dotProducts[0][0] - thisFrameTranslation[Vector3::Coordinate_X] * dotProducts[1][0] );

   if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
   {
      return false;
   }

   //this_ZAxis x that_YAxis
   thisRadiusProjection = thisExtents[0] * fabs(dotProducts[1][1]) + thisExtents[1] * fabs(dotProducts[0][1]);
   thatRadiusProjection = thatExtents[0] * fabs(dotProducts[2][2]) + thatExtents[2] * fabs(dotProducts[2][0]);
   projectionThreshold = fabs( thisFrameTranslation[Vector3::Coordinate_Y] * dotProducts[0][1] - thisFrameTranslation[Vector3::Coordinate_X] * dotProducts[1][1] );

   if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
   {
      return false;
   }

   //this_ZAxis x that_ZAxis
   thisRadiusProjection = thisExtents[0] * fabs(dotProducts[1][2]) + thisExtents[1] * fabs(dotProducts[0][2]);
   thatRadiusProjection = thatExtents[0] * fabs(dotProducts[2][1]) + thatExtents[1] * fabs(dotProducts[2][0]);
   projectionThreshold = fabs( thisFrameTranslation[Vector3::Coordinate_Y] * dotProducts[0][2] - thisFrameTranslation[Vector3::Coordinate_X] * dotProducts[1][2] );

   if (projectionThreshold > (thisRadiusProjection + thatRadiusProjection))
   {
      return false;
   }

   return true;
}

bool OrientedBox::Intersects(const Sphere& sphere, const Moveable& sphereMoveable) const
{
   Sphere sphereInBoxSpace(sphereMoveable.CurrentModelTransformation().MultVertex(sphere.center), sphereMoveable.CurrentScale().x * sphere.radius);

   sphereInBoxSpace.center = rotationInverse.MultVertex(sphereInBoxSpace.center - centroid);

   return AxisAlignedBox::Intersects(sphereInBoxSpace);
}

bool OrientedBox::Intersects(const Moveable& thisMoveable, const OrientedBox& other, const Moveable& otherMoveable) const
{
   OrientedBox thisTransformed(*this);
   thisTransformed.TransformBy(thisMoveable);

   OrientedBox otherTransformed(other);
   otherTransformed.TransformBy(otherMoveable);

   return thisTransformed.Intersects(otherTransformed);
}

bool OrientedBox::Intersects(const AxisAlignedBox& box, const Moveable& boxMoveable) const
{
   OrientedBox boxOriented(box);

   boxOriented.TransformBy(boxMoveable);

   return Intersects(boxOriented);
}

bool OrientedBox::Intersects(const OrientedBox& box, const Moveable& boxMoveable) const
{
   OrientedBox transformedBox(box);

   transformedBox.TransformBy(boxMoveable);

   return Intersects(transformedBox);
}

}