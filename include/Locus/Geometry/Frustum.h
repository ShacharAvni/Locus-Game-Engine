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

#include "Plane.h"
#include "Vector3.h"

namespace Locus
{

class LOCUS_GEOMETRY_API Frustum
{
public:
   Frustum(const Vector3& point, const Vector3& forward, const Vector3& up, float horizField, float vertField, float distNear, float distFar);

   bool Within(const Vector3& point) const;
   bool Within(const Vector3& centerOfSphere, float sphereRadius) const;

private:
   enum PlaneLocations
   {
      Near = 0,
      Far = 1,
      Left = 2,
      Right = 3,
      Bottom = 4,
      Top = 5,
      NUM_PLANES
   };

   Plane planes[NUM_PLANES];
   Vector3 viewPoint;
   Vector3 forwardVector;
   Vector3 upVector;
   float horizontalField; //in degrees
   float verticalField; //in degrees
   float nearDistance;
   float farDistance;
};

}