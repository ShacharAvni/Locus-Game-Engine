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
class LOCUS_GEOMETRY_API Line;

typedef Line<FVector3> Line3D_t;
typedef Line<FVector2> Line2D_t;

}