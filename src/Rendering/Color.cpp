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

#include <cassert>

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

Color::Color(unsigned char r, unsigned char g, unsigned char b)
   : r(r), g(g), b(b), a(255)
{
}

void Color::SerializeTo(unsigned char* destination) const
{
   assert(destination != nullptr);

   destination[0] = r;
   destination[1] = g;
   destination[2] = b;
   destination[3] = a;
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

bool operator==(const Color& color1, const Color& color2)
{
   return ( (color1.r == color2.r) && (color1.g == color2.g) && (color1.b == color2.b) && (color1.a == color2.a) );
}

bool operator <(const Color& color1, const Color& color2)
{
   if (color1.r == color2.r)
   {
      if (color1.g == color2.g)
      {
         if (color1.b == color2.b)
         {
            return (color1.a < color2.a);
         }
         else
         {
            return (color1.b < color2.b);
         }
      }
      else
      {
         return (color1.g < color2.g);
      }
   }
   else
   {
      return (color1.r < color2.r);
   }
}

}