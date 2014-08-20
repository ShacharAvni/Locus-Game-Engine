/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/PointCloud.h"

#include "Locus/Common/Float.h"
#include "Locus/Common/Util.h"

namespace Locus
{

PointCloud::PointCloud()
   : maxDistanceToCenter(0.0f)
{
}

PointCloud::PointCloud(const std::vector<Vector3>& positions)
   : positions(positions), maxDistanceToCenter(0.0f)
{
}

PointCloud::~PointCloud()
{
}

const std::vector<Vector3>& PointCloud::GetPositions() const
{
   return positions;
}

std::vector<Vector3> PointCloud::GetTransformedPositions(const Transformation* transformation) const
{
   if (transformation == nullptr)
   {
      return GetTransformedPositions(CurrentModelTransformation());
   }
   else
   {
      return GetTransformedPositions(*transformation);
   }
}

std::vector<Vector3> PointCloud::GetTransformedPositions(const Transformation& transformation) const
{
   std::vector<Vector3> transformedPositions;
      
   if (positions.size() > 0)
   {
      transformedPositions.reserve(positions.size());

      for (const Vector3& position : positions)
      {
         transformedPositions.push_back(transformation.MultVertex(position));
      }
   }

   return transformedPositions;
}

void PointCloud::AddPosition(const Vector3& v)
{
   positions.push_back(v);
}

void PointCloud::AddPositions(const std::vector<Vector3>& positionsToAdd)
{
   for (const Vector3& position : positionsToAdd)
   {
      AddPosition(position);
   }
}

void PointCloud::Clear()
{
   ClearAndShrink(positions);
   centroid = Vector3::ZeroVector();
}

void PointCloud::Translate(const Vector3& translation)
{
   Moveable::Translate(translation);
   centroid += translation;
}

Vector3 PointCloud::ComputeCentroid(const std::vector<Vector3>& points)
{
   Vector3 centroidComputed(0, 0, 0);

   for (const Vector3& point : points)
   {
      centroidComputed += point;
   }
   centroidComputed = centroidComputed/static_cast<float>(points.size());

   return centroidComputed;
}

void PointCloud::ComputeCentroid()
{
   centroid = ComputeCentroid(positions);
}

float PointCloud::GetMaxDistanceToCenter() const
{
   return maxDistanceToCenter;
}

void PointCloud::UpdateMaxDistanceToCenter()
{
   maxDistanceToCenter = 0;

   Transformation modelTransformation = CurrentModelTransformation();

   for (const Vector3& vertexPosition : positions)
   {
      Vector3 worldVertex = modelTransformation.MultVertex(vertexPosition);
      float distance = (worldVertex - centroid).norm();

      if (FGreater<float>(distance, maxDistanceToCenter))
      {
         maxDistanceToCenter = distance;
      }
   }
}

Sphere PointCloud::BoundingSphere() const
{
   return Sphere(centroid, maxDistanceToCenter);
}

}