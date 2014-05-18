/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/PolygonWinding.h"

namespace Locus
{

PolygonWinding ReversePolygonWinding(PolygonWinding winding)
{
   if (winding == PolygonWinding::CounterClockwise)
   {
      return PolygonWinding::Clockwise;
   }
   else
   {
      return PolygonWinding::CounterClockwise;
   }
}

}