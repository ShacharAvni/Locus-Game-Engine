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

enum class PolygonWinding
{
   CounterClockwise,
   Clockwise,
   Undefined
};

LOCUS_GEOMETRY_API PolygonWinding ReversePolygonWinding(PolygonWinding winding);

}