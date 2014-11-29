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

#include "GPUVertexData.h"
#include "GPUVertexDataStorage.h"

namespace Locus
{

class LOCUS_RENDERING_API DefaultGPUVertexData : public GPUVertexData
{
public:
   DefaultGPUVertexData();

   virtual void SetAttributes(ShaderController& shaderController) const override;
   virtual void SetClientState() const override;

   GPUVertexDataTransferInfo transferInfo;
};

}