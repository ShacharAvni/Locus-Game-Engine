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

class LOCUS_RENDERING_API Color
{
public:
   Color();
   Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
   Color(unsigned char r, unsigned char g, unsigned char b);

   unsigned char r;
   unsigned char g;
   unsigned char b;
   unsigned char a;

   static const Color& Black();
   static const Color& White();
   static const Color& Red();
   static const Color& Blue();
   static const Color& Green();
   static const Color& Yellow();
};

//for sorting within the Mesh class
LOCUS_RENDERING_API bool operator==(const Color& color1, const Color& color2);
LOCUS_RENDERING_API bool operator <(const Color& color1, const Color& color2);

}