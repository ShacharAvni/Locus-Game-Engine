 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Rendering/SingleDrawable.h"
#include "Locus/Rendering/GPUVertexData.h"
#include "Locus/Rendering/RenderingState.h"

namespace Locus
{

SingleDrawable::SingleDrawable()
{
}

SingleDrawable::SingleDrawable(const SingleDrawable& /*singleDrawable*/)
{
}

SingleDrawable::~SingleDrawable()
{
}

SingleDrawable& SingleDrawable::operator=(const SingleDrawable& /*singleDrawable*/)
{
   return *this;
}

void SingleDrawable::CreateGPUVertexData()
{
   gpuVertexData.reset(new GPUVertexData());
}

void SingleDrawable::DeleteGPUVertexData()
{
   gpuVertexData.reset();
}

void SingleDrawable::Draw(RenderingState& renderingState) const
{
   if (gpuVertexData != nullptr)
   {
      gpuVertexData->Draw(renderingState.shaderController);
   }
}

}