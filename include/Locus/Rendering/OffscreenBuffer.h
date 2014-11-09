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

#include "GLCommonTypes.h"

#include "Color.h"

namespace Locus
{

class LOCUS_RENDERING_API OffscreenBuffer
{
public:
   OffscreenBuffer(int maxWidth, int maxHeight);
   ~OffscreenBuffer();

   OffscreenBuffer(const OffscreenBuffer&) = delete;
   OffscreenBuffer& operator=(const OffscreenBuffer&) = delete;

   void Bind() const;

   static void Unbind();

   Color ReadPixel(int x, int y);

private:
   GLuint fboID;
   GLuint renderBufferID;
   GLuint depthBufferID;
   GLuint pixelPackBufferID;

   void DestroyBuffers();
};

}