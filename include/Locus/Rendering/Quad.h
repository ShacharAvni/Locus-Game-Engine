 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusRenderingAPI.h"

#include "Mesh.h"

namespace Locus
{

class Vector3;
class Color;

class LOCUS_RENDERING_API Quad : public Mesh
{
public:
   Quad();
   void Set(const Vector3& lowerLeftCorner, const Vector3& widthDirection, const Vector3& heightDirection, float width, float height, const Color& color);
};

}