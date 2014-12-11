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
   Image(unsigned int width, unsigned int height, unsigned int numPixelComponents);
   Image(const std::string& filePath);
   Image(const MountedFilePath& mountedFilePath);
   Image(const unsigned char* pixelData, unsigned int width, unsigned int height, unsigned int numPixelComponents);

   static bool ValidPixelComponents(unsigned int numPixelComponents);

   unsigned int Width() const;
   unsigned int Height() const;
   unsigned int NumPixelComponents() const;
   const unsigned char* PixelData() const;

   const unsigned char* GetPixel(unsigned int x, unsigned int y) const;
   unsigned char* GetPixel(unsigned int x, unsigned int y);

   void GetSubImage(unsigned int x, unsigned int y, unsigned int rectWidth, unsigned int rectHeight, std::vector<unsigned char>& subImagePixelData) const;
   void GetSubImage(unsigned int x, unsigned int y, unsigned int rectWidth, unsigned int rectHeight, unsigned int rectPixelComponents, std::vector<unsigned char>& subImagePixelData) const;
   void SetSubImage(unsigned int x, unsigned int y, unsigned int rectWidth, unsigned int rectHeight, const std::vector<unsigned char>& subImagePixelData);
   void SetSubImage(unsigned int x, unsigned int y, unsigned int rectWidth, unsigned int rectHeight, unsigned int rectPixelComponents, const std::vector<unsigned char>& subImagePixelData);

   void FlipVertically();
   void Scale(unsigned int newWidth, unsigned int newHeight);

private:
   unsigned int width;
   unsigned int height;
   unsigned int numPixelComponents;

   std::vector<unsigned char> pixelData;

   unsigned int GetPixelOffset(unsigned int x, unsigned int y) const;

   static unsigned int GetPixelOffset(unsigned int x, unsigned int y, unsigned int width, unsigned int numPixelComponents);

   void FinishLoad(unsigned char* pixels, unsigned int width, unsigned int height, unsigned int numPixelComponents);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}