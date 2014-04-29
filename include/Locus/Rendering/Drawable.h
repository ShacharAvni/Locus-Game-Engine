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

namespace Locus
{

class RenderingState;
class ShaderController;

class LOCUS_RENDERING_API Drawable
{
public:
   Drawable();
   virtual ~Drawable();

   virtual void CreateGPUVertexData() = 0;
   virtual void DeleteGPUVertexData() = 0;
   virtual void UpdateGPUVertexData() = 0;

   virtual void Draw(RenderingState& renderingState) const = 0;
};

}