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

#include "Locus/Math/VectorsFwd.h"

namespace Locus
{

template <class PointType>
class LOCUS_GEOMETRY_API Triangle;

typedef Triangle<FVector3> Triangle3D_t;
typedef Triangle<FVector2> Triangle2D_t;

}