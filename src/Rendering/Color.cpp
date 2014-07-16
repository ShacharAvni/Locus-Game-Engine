/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/Color.h"

namespace Locus
{

Color::Color()
   : r(0), g(0), b(0), a(255)
{
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
   : r(r), g(g), b(b), a(a)
{
}

const Color& Color::Black()
{
   static Color black(0, 0, 0, 255);
   return black;
}

const Color& Color::White()
{
   static Color white(255, 255, 255, 255);
   return white;
}

const Color& Color::Red()
{
   static Color red(255, 0, 0, 255);
   return red;
}

const Color& Color::Blue()
{
   static Color blue(0, 0, 255, 255);
   return blue;
}

const Color& Color::Green()
{
   static Color green(0, 255, 0, 255);
   return green;
}

const Color& Color::Yellow()
{
   static Color yellow(255, 255,  0 , 255);
   return yellow;
}


}