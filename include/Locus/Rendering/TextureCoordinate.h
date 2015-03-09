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

   /*!
    * \brief serializes the TextureCoordinate.
    *
    * \param[in] destination pointer to a pre allocated
    * memory location where the TextureCoordinate data
    * will be placed.
    *
    * \details this function writes to destination (x), and destination + 1
    * (y). Undefined behaviour will occur if any of these locations are
    * invalid.
    */
   void SerializeTo(float* destination) const;
};

LOCUS_RENDERING_API TextureCoordinate operator+(const TextureCoordinate& t1, const TextureCoordinate& t2);
LOCUS_RENDERING_API TextureCoordinate& operator+=(TextureCoordinate& t1, const TextureCoordinate& t2);
LOCUS_RENDERING_API TextureCoordinate operator-(const TextureCoordinate& t1, const TextureCoordinate& t2);
LOCUS_RENDERING_API TextureCoordinate& operator-=(TextureCoordinate& t1, const TextureCoordinate& t2);
LOCUS_RENDERING_API TextureCoordinate operator/(const TextureCoordinate& t1, float k);
LOCUS_RENDERING_API TextureCoordinate& operator/=(TextureCoordinate& t1, float k);
LOCUS_RENDERING_API TextureCoordinate operator*(const TextureCoordinate& t1, float k);
LOCUS_RENDERING_API TextureCoordinate operator*(float k, const TextureCoordinate& t1);
LOCUS_RENDERING_API TextureCoordinate& operator*=(TextureCoordinate& t1, float k);

//for sorting within the Mesh class
LOCUS_RENDERING_API bool operator==(const TextureCoordinate& coord1, const TextureCoordinate& coord2);
LOCUS_RENDERING_API bool operator <(const TextureCoordinate& coord1, const TextureCoordinate& coord2);

}