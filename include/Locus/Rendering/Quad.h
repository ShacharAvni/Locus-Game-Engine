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

#include "Mesh.h"

namespace Locus
{

class Color;

class LOCUS_RENDERING_API Quad : public Mesh
{
public:
   Quad();
   void Set(const FVector3& lowerLeftCorner, const FVector3& widthDirection, const FVector3& heightDirection, float width, float height, const Color& color);
};

}