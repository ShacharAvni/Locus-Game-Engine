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

#include "SingleDrawable.h"

namespace Locus
{

class DefaultGPUVertexData;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API DefaultSingleDrawable : public SingleDrawable
{
public:
   DefaultSingleDrawable();

   virtual void CreateGPUVertexData() override;
   virtual void DeleteGPUVertexData() override;

protected:
   DefaultGPUVertexData* defaultGPUVertexData;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}