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

#include "Moveable.h"
#include "Collidable.h"
#include "Sphere.h"

#include "Locus/Math/Vectors.h"

#include <vector>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_GEOMETRY_API PointCloud : public Moveable
{
public:
   PointCloud();
   PointCloud(const std::vector<FVector3>& positions);
   virtual ~PointCloud();

   const std::vector<FVector3>& GetPositions() const;
   std::vector<FVector3> GetTransformedPositions(const Transformation* transformation = nullptr) const;

   virtual void AddPosition(const FVector3& v);
   void AddPositions(const std::vector<FVector3>& positionsToAdd);

   virtual void Clear();

   virtual void Translate(const FVector3& translation);

   static FVector3 ComputeCentroid(const std::vector<FVector3>& points); 
   void ComputeCentroid();

   float GetMaxDistanceToCenter() const;
   void UpdateMaxDistanceToCenter();

   Sphere BoundingSphere() const;

   FVector3 centroid;

protected:
   std::vector<FVector3> positions;
   float maxDistanceToCenter;

   std::vector<FVector3> GetTransformedPositions(const Transformation& transformation) const;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}