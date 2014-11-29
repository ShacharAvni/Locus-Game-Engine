/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/DefaultSingleDrawable.h"
#include "Locus/Rendering/DefaultGPUVertexData.h"

namespace Locus
{

DefaultSingleDrawable::DefaultSingleDrawable()
{
   defaultGPUVertexData = nullptr;
}

void DefaultSingleDrawable::CreateGPUVertexData()
{
   defaultGPUVertexData = new DefaultGPUVertexData();
   gpuVertexData.reset(defaultGPUVertexData);
}

void DefaultSingleDrawable::DeleteGPUVertexData()
{
   SingleDrawable::DeleteGPUVertexData();
   defaultGPUVertexData = nullptr;
}

}