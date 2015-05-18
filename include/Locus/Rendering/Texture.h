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
#include "TextureFiltering.h"

namespace Locus
{

class Image;
class GLInfo;

class LOCUS_RENDERING_API Texture
{
public:
   enum class MipmapGeneration
   {
      GLGenerateMipMap,
      GLGenerateMipMapLegacy,
      Manual,
      None
   };

   Texture(const Image& image, MipmapGeneration mipmapGeneration, TextureFiltering filtering, bool clamp, const GLInfo& glInfo);
   ~Texture();

   Texture(const Texture&) = delete;
   Texture& operator=(const Texture&) = delete;

   void Bind() const;

   static void SetUnpackAlignmentForPixelComponents(unsigned int numPixelComponents);
   static GLint GLFormat(unsigned int numPixelComponents);
   static GLint GLSizedFormat(unsigned int numPixelComponents);

   static void SendTextureData(const Image& image, GLint textureLevel);

private:
   GLuint id;

   static unsigned int ClosestPowerOf2(unsigned int num);

   void GenerateMipmaps(const Image& image, MipmapGeneration mipmapGeneration, const GLInfo& glInfo) const;
   void GenerateMipmapsLegacy(const Image& image) const;
   void GenerateManualMipmaps(const Image& image) const;
   static void GenerateManualMipmapsUsingPowerOf2Image(Image& image);
};

}