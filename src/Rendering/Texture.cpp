/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/Texture.h"
#include "Locus/Rendering/GLInfo.h"
#include "Locus/Rendering/Image.h"

#include <Locus/Rendering/Locus_glew.h>

#include <algorithm>

#include <cmath>
#include <cassert>

namespace Locus
{

Texture::Texture(const Image& image, bool clamp, const GLInfo& glInfo)
{
   Construct(image, MipmapGeneration::GLGenerateMipMap, clamp, glInfo);
}

Texture::Texture(const Image& image, MipmapGeneration mipmapGeneration, bool clamp, const GLInfo& glInfo)
{
   Construct(image, mipmapGeneration, clamp, glInfo);
}

void Texture::Construct(const Image& image, MipmapGeneration mipmapGeneration, bool clamp, const GLInfo& glInfo)
{
   glGenTextures(1, &id);

   Bind();

   GenerateMipmaps(image, mipmapGeneration, glInfo);

   bool doClamp = (clamp && (GLEW_VERSION_1_2 || glewIsExtensionSupported("GL_EXT_texture_edge_clamp")));

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, doClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, doClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
}

Texture::~Texture()
{
   glDeleteTextures(1, &id);
}

void Texture::Bind() const
{
   glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::SetUnpackAlignmentForPixelComponents(unsigned int numPixelComponents)
{
   assert(Image::ValidPixelComponents(numPixelComponents));

   switch (numPixelComponents)
   {
   case 4:
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
      break;

   case 2:
      glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
      break;

   default:
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      break;
   }
}

GLint Texture::GLFormat(unsigned int numPixelComponents)
{
   assert(Image::ValidPixelComponents(numPixelComponents));

   switch (numPixelComponents)
   {
   case 4:
      return GL_RGBA;
      break;

   case 3:
      return GL_RGB;
      break;

   case 2:
      return GL_RG;
      break;

   case 1:
   default:
      return GL_RED;
      break;
   }
}

GLint Texture::GLSizedFormat(unsigned int numPixelComponents)
{
   assert(Image::ValidPixelComponents(numPixelComponents));

   switch (numPixelComponents)
   {
   case 4:
      return GL_RGBA8;
      break;

   case 3:
      return GL_RGB8;
      break;

   case 2:
      return GL_RG8;
      break;

   case 1:
   default:
      return GL_R8;
      break;
   }
}

void Texture::SendTextureData(const Image& image, GLint textureLevel)
{
   GLint format = Texture::GLFormat(image.NumPixelComponents());
   glTexImage2D(GL_TEXTURE_2D, textureLevel, format, image.Width(), image.Height(), 0, format, GL_UNSIGNED_BYTE, image.PixelData());
}

void Texture::GenerateMipmaps(const Image& image, MipmapGeneration mipmapGeneration, const GLInfo& glInfo) const
{
   Texture::SetUnpackAlignmentForPixelComponents(image.NumPixelComponents());

   if (mipmapGeneration == MipmapGeneration::Manual)
   {
      GenerateManualMipmaps(image);
      return;
   }

   if (mipmapGeneration == MipmapGeneration::GLGenerateMipMap)
   {
      GLInfo::Vendor vendor = glInfo.GetVendor();

      if ((vendor == GLInfo::Vendor::Microsoft) || (vendor == GLInfo::Vendor::Unknown))
      {
         GenerateMipmapsLegacy(image, MipmapGeneration::GLGenerateMipMapLegacyLinear);
         return;
      }

      bool vendorIsATI = (vendor == GLInfo::Vendor::ATI);

      if (GLEW_VERSION_3_0)
      {
         Texture::SendTextureData(image, 0);

         if (vendorIsATI)
         {
            glEnable(GL_TEXTURE_2D);
         }

         glGenerateMipmap(GL_TEXTURE_2D);
         return;
      }

      if (GLEW_VERSION_2_1 && glewIsExtensionSupported("GL_EXT_framebuffer_object"))
      {
         Texture::SendTextureData(image, 0);

         if (vendorIsATI)
         {
            glEnable(GL_TEXTURE_2D);
         }

         glGenerateMipmapEXT(GL_TEXTURE_2D);
         return;
      }
   }

   if ((mipmapGeneration == MipmapGeneration::NoMipMapLinear) || (mipmapGeneration == MipmapGeneration::NoMipMapNearest))
   {
      SendTextureDataWithoutMipmaps(image, mipmapGeneration);
      return;
   }

   GenerateMipmapsLegacy(image, mipmapGeneration);
}

void Texture::GenerateMipmapsLegacy(const Image& image, MipmapGeneration mipmapGeneration) const
{
   if (GLEW_VERSION_1_4)
   {
      bool mipMapNearest = (mipmapGeneration == MipmapGeneration::GLGenerateMipMapLegacyNearest);

      GLint minFilterParam = mipMapNearest ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR;
      GLint magFilterParam = mipMapNearest ? GL_NEAREST : GL_LINEAR;

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterParam);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterParam);

      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

      Texture::SendTextureData(image, 0);
   }
   else
   {
      GenerateManualMipmaps(image);
   }
}

void Texture::SendTextureDataWithoutMipmaps(const Image& image, MipmapGeneration mipmapGeneration) const
{
   bool nearest = (mipmapGeneration == MipmapGeneration::NoMipMapNearest);

   GLint minFilterParam = nearest ? GL_NEAREST : GL_LINEAR;
   GLint magFilterParam = nearest ? GL_NEAREST : GL_LINEAR;

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterParam);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterParam);

   Texture::SendTextureData(image, 0);
}

unsigned int Texture::ClosestPowerOf2(unsigned int num)
{
   assert(num >= 1);

   unsigned int closestPowerOf2 = 1;
   while (closestPowerOf2 < num)
   {
      closestPowerOf2 *= 2;
   }

   if (closestPowerOf2 != 1)
   {
      unsigned int previousPowerOf2 = closestPowerOf2 / 2;

      unsigned int diffPrevious = num - previousPowerOf2;

      if ((closestPowerOf2 - num) > diffPrevious)
      {
         closestPowerOf2 = previousPowerOf2;
      }
   }

   return closestPowerOf2;
}

void Texture::GenerateManualMipmaps(const Image& image) const
{
   unsigned int closestXPowerOf2 = Texture::ClosestPowerOf2(image.Width());
   unsigned int closestYPowerOf2 = Texture::ClosestPowerOf2(image.Height());

   Image scaledImage(image);

   if ((closestXPowerOf2 != image.Width()) || (closestYPowerOf2 != image.Height()))
   {
      scaledImage.Scale(closestXPowerOf2, closestYPowerOf2);
   }

   GenerateManualMipmapsUsingPowerOf2Image(scaledImage);
}

void Texture::GenerateManualMipmapsUsingPowerOf2Image(Image& image)
{
   GLint maxTextureSize = 0;
   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

   const float log2 = std::log(2.0f);

   int maxLevels = (int) (std::log((float) maxTextureSize) / log2) + 1;

   int divisionsX = (int) std::ceil(std::log((float) image.Width()) / log2) + 1;
   int divisionsY = (int) std::ceil(std::log((float) image.Height()) / log2) + 1;

   int numLevels = std::max(divisionsX, divisionsY);
   numLevels = std::min(numLevels, maxLevels);

   Texture::SendTextureData(image, 0);

   for (int level = 1; level < numLevels; ++level)
   {
      unsigned int newWidth = ((image.Width() != 1) ? (image.Width() / 2) : 1);
      unsigned int newHeight = ((image.Height() != 1) ? (image.Height() / 2) : 1);

      image.Scale(newWidth, newHeight);

      Texture::SendTextureData(image, level);
   }
}

}