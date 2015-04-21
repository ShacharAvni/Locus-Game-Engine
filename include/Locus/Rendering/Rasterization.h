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

#include "LocusRenderingAPI.h"

#include "Locus/Math/VectorsFwd.h"

#include <vector>

namespace Locus
{

LOCUS_RENDERING_API void Bresenham2D(const IVector2& pixelDifference, std::vector<IVector2>& pixelOffsets, std::size_t& numPixelsWritten);

}