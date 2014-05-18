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

struct LOCUS_RENDERING_API GPUVertexDataStorage
{
   float position[3];
   float normal[3];
   float texCoord[2];
   unsigned char color[4];
};

LOCUS_RENDERING_API void ZeroFill(GPUVertexDataStorage& gpuVertexDataStorage);

struct LOCUS_RENDERING_API GPUVertexDataTransferInfo
{
   GPUVertexDataTransferInfo()
      : sendPositions(true), sendNormals(true), sendTexCoords(true), sendColors(true)
   {
   }

   bool sendPositions;
   bool sendNormals;
   bool sendTexCoords;
   bool sendColors;
};

}