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

namespace Locus
{

class Moveable;

class LOCUS_GEOMETRY_API Sphere
{
public:
   Sphere();
   Sphere(const FVector3& center, float radius);
   Sphere(const std::vector<FVector3>& points);

   bool Intersects(const Moveable& thisMoveable, const Sphere& other, const Moveable& otherMoveable) const;

   float Volume() const;

   FVector3 center;
   float radius;
};

}