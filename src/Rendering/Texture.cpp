 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

#include "Locus/Rendering/Texture.h"
#include "Locus/Rendering/GLInfo.h"

#include <Locus/Rendering/Locus_glew.h>

#include <algorithm>

#include <cmath>
#include <cassert>

namespace Locus
{

Texture::Texture(const std::string& filePath, bool clamp, const GLInfo& glInfo)
   : Image(filePath)
{
   FinishInit(clamp, glInfo);
}

Texture::Texture(const MountedFilePath& mountedFilePath, bool clamp, const GLInfo& glInfo)
   : Image(mountedFilePath)
{
   FinishInit(clamp, glInfo);
}

Texture::~Texture()
{
   glDeleteTextures(1, &id);
}

void Texture::FinishInit(bool clamp, const GLInfo& glInfo)
{
   glGenTextures(1, &id);

   Bind();

   GenerateMipmaps(glInfo);

   bool doClamp = (clamp && (GLEW_VERSION_1_2 || glewIsExtensionSupported("GL_EXT_texture_edge_clamp")));

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, doClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, doClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
}

void Texture::Bind()
{
   glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::SetUnpackAlignmentForPixelComponents(unsigned int numPixelComponents)
{
   assert((numPixelComponents > 0) && (numPixelComponents <= 4));

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
   assert((numPixelComponents > 0) && (numPixelComponents <= 4));

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

void Texture::SendTextureData(const Image& image, GLint textureLevel)
{
   GLint format = Texture::GLFormat(image.NumPixelComponents());
   glTexImage2D(GL_TEXTURE_2D, textureLevel, format, image.Width(), image.Height(), 0, format, GL_UNSIGNED_BYTE, image.PixelData());
}

void Texture::GenerateMipmaps(const GLInfo& glInfo) const
{
   Texture::SetUnpackAlignmentForPixelComponents(NumPixelComponents());

   GLInfo::Vendor vendor = glInfo.GetVendor();

   if ((vendor == GLInfo::Vendor::Microsoft) || (vendor == GLInfo::Vendor::Unknown))
   {
      GenerateMipmapsLegacy();
   }
   else
   {
      bool vendorIsATI = (vendor == GLInfo::Vendor::ATI);

      if (GLEW_VERSION_3_0)
      {
         Texture::SendTextureData(*this, 0);

         if (vendorIsATI)
         {
            glEnable(GL_TEXTURE_2D);
         }

         glGenerateMipmap(GL_TEXTURE_2D);
      }
      else if (GLEW_VERSION_2_1 && glewIsExtensionSupported("GL_EXT_framebuffer_object"))
      {
         Texture::SendTextureData(*this, 0);

         if (vendorIsATI)
         {
            glEnable(GL_TEXTURE_2D);
         }

         glGenerateMipmapEXT(GL_TEXTURE_2D);
      }
      else
      {
         GenerateMipmapsLegacy();
      }
   }
}

void Texture::GenerateMipmapsLegacy() const
{
   if (GLEW_VERSION_1_4)
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

      Texture::SendTextureData(*this, 0);
   }
   else
   {
      GenerateManualMipmaps();
   }
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

void Texture::GenerateManualMipmaps() const
{
   unsigned int closestXPowerOf2 = Texture::ClosestPowerOf2(Width());
   unsigned int closestYPowerOf2 = Texture::ClosestPowerOf2(Height());

   Image image(PixelData(), Width(), Height(), NumPixelComponents());

   if ((closestXPowerOf2 != Width()) || (closestYPowerOf2 != Height()))
   {
      image.Scale(closestXPowerOf2, closestYPowerOf2);
   }

   GenerateManualMipmapsUsingPowerOf2Image(image);
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

   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (numLevels - 1));

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