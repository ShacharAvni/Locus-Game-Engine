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

namespace Locus
{

struct LOCUS_RENDERING_API TextureCoordinate
{
   TextureCoordinate();
   TextureCoordinate(float x, float y);

   float x;
   float y;
};

//for sorting within the Mesh class
LOCUS_RENDERING_API bool operator==(const TextureCoordinate& coord1, const TextureCoordinate& coord2);
LOCUS_RENDERING_API bool operator <(const TextureCoordinate& coord1, const TextureCoordinate& coord2);

}