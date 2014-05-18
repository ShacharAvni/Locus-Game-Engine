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

namespace Locus
{

template <class PointType>
class LOCUS_GEOMETRY_API Polygon;

class Vector3;
class Vector2;

typedef Polygon<Vector3> Polygon3D_t;
typedef Polygon<Vector2> Polygon2D_t;

}