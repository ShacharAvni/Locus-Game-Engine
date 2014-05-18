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

class RenderingState;

class LOCUS_RENDERING_API DrawUtility
{
public:
   static void BeginDrawing2D(RenderingState& renderingState, int resolutionX, int resolutionY);
   static void EndDrawing2D(RenderingState& renderingState);
};

}