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

#include "Drawable.h"

#include <memory>

namespace Locus
{

class GPUVertexData;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API SingleDrawable : public Drawable
{
public:
   virtual ~SingleDrawable();

   virtual void DeleteGPUVertexData() override;

   virtual void Draw(RenderingState& renderingState) const override;

protected:
   std::unique_ptr<GPUVertexData> gpuVertexData;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}