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
   Util::ClearAndShrink(positions);
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

      if (Float::FGreater<float>(distance, maxDistanceToCenter))
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