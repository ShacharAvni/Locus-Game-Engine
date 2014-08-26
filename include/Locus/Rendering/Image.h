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

#include <string>
#include <vector>

namespace Locus
{

struct MountedFilePath;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API Image
{
public:
   Image(const std::string& filePath);
   Image(const MountedFilePath& mountedFilePath);
   Image(const unsigned char* pixelData, unsigned int width, unsigned int height, unsigned int numPixelComponents);

   unsigned int Width() const;
   unsigned int Height() const;
   unsigned int NumPixelComponents() const;
   const unsigned char* PixelData() const;

   void ClearPixelData();

   void FlipVertically();
   void Scale(unsigned int newWidth, unsigned int newHeight);

private:
   std::vector<unsigned char> pixelData;

   unsigned int numPixelComponents;
   unsigned int width;
   unsigned int height;

   void FinishLoad(unsigned char* pixels, unsigned int width, unsigned int height, unsigned int numPixelComponents);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}