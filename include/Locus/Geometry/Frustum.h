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

#include "Plane.h"

namespace Locus
{

/// A geometrical view frustum.
class LOCUS_GEOMETRY_API Frustum
{
public:
   /*!
    * \param[in] point The viewpoint which is where the
    * tip of the frustum pyramid would be.
    *
    * \param[in] forward A vector pointing from the viewpoint
    * into the frustum.
    *
    * \param[in] up A vector pointing up from the viewpoint.
    *
    * \param[in] horizField Angle in degrees defining the
    * horizontal field of view. This angle would be at the
    * tip of the pyramid if looking straight down at
    * a 2D projection of the frustum.
    *
    * \param[in] vertField Angle in degrees defining the
    * vertical field of view. This angle would be at the
    * tip of the pyramid if looking straight from the side
    * at a 2D projection of the frustum.
    *
    * \param[in] distNear The distance from the viewpoint to
    * the near plane.
    *
    * \param[in] distFar The distance from the viewpoint to
    * the far plane.
    */
   Frustum(const FVector3& point, const FVector3& forward, const FVector3& up, float horizField, float vertField, float distNear, float distFar);

   /// \return true if the point is within the frustum.
   bool Within(const FVector3& point) const;

   /// \return true if the sphere given by its center and radius is completely within the frustum.
   bool Within(const FVector3& centerOfSphere, float sphereRadius) const;

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
   FVector3 viewPoint;
   FVector3 forwardVector;
   FVector3 upVector;
   float horizontalField; //in degrees
   float verticalField; //in degrees
   float nearDistance;
   float farDistance;
};

}