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
#include "Moveable.h"
#include "Collidable.h"
#include "Sphere.h"

#include <vector>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_GEOMETRY_API PointCloud : public Moveable
{
public:
   PointCloud();
   PointCloud(const std::vector<Vector3>& positions);
   virtual ~PointCloud();

   const std::vector<Vector3>& GetPositions() const;
   std::vector<Vector3> GetTransformedPositions(const Transformation* transformation = nullptr) const;

   virtual void AddPosition(const Vector3& v);
   void AddPositions(const std::vector<Vector3>& positionsToAdd);

   virtual void Clear();

   virtual void Translate(const Vector3& translation);

   static Vector3 ComputeCentroid(const std::vector<Vector3>& points); 
   void ComputeCentroid();

   float GetMaxDistanceToCenter() const;
   void UpdateMaxDistanceToCenter();

   Sphere BoundingSphere() const;

   Vector3 centroid;

protected:
   std::vector<Vector3> positions;
   float maxDistanceToCenter;

   std::vector<Vector3> GetTransformedPositions(const Transformation& transformation) const;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}