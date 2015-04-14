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

namespace Locus
{

class RenderingState;
class Transformation;

class LOCUS_RENDERING_API DrawUtility
{
public:
   static void BeginDrawing2D(RenderingState& renderingState, int resolutionX, int resolutionY);
   static void EndDrawing2D(RenderingState& renderingState);

   static bool Unproject(float windowCoordX,
                         float windowCoordY,
                         float windowCoordZ,
                         const Transformation& modelView,
                         const Transformation& projection,
                         int resolutionX,
                         int resolutionY,
                         FVector3& worldCoordinate);
};

}