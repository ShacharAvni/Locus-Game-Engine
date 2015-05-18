/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/TextureArray.h"
#include "Locus/Rendering/Texture.h"

#include "Locus/Common/Exception.h"
#include "Locus/Common/Casts.h"

#include <Locus/Rendering/Locus_glew.h>

#include <cassert>

namespace Locus
{

TextureArray::TextureArray(const std::vector<Image>& images, bool mipmaps, TextureFiltering filtering, bool clamp)
{
   assert(TextureArray::ImagesAreWellFormed(images));

   if (!GLEW_VERSION_3_0)
   {
      throw Exception("OpenGL 3.0 is required to use Texture Arrays");
   }

   glGenTextures(1, &id);
   glBindTexture(GL_TEXTURE_2D_ARRAY, id);

   GLsizei width = LossyCast<GLsizei, unsigned int>(images[0].Width());
   GLsizei height = LossyCast<GLsizei, unsigned int>(images[0].Height());
   unsigned int numPixelComponents = images[0].NumPixelComponents();

   GLsizei numImages = LossyCast<GLsizei, std::size_t>(images.size());

   Texture::SetUnpackAlignmentForPixelComponents(numPixelComponents);

   bool linearFiltering = (filtering == TextureFiltering::Linear);

   GLint magFilterParam = linearFiltering ? GL_LINEAR : GL_NEAREST;

   GLint minFilterParam;

   if (mipmaps)
   {
      minFilterParam = linearFiltering ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
   }
   else
   {
      minFilterParam = linearFiltering ? GL_LINEAR : GL_NEAREST;
   }

   glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilterParam);
   glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilterParam);

   glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

   GLint format = Texture::GLFormat(numPixelComponents);

   glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, Texture::GLSizedFormat(numPixelComponents), width, height, numImages, 0, format, GL_UNSIGNED_BYTE, nullptr);

   for (GLsizei imageIndex = 0; imageIndex < numImages; ++imageIndex)
   {
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, imageIndex, width, height, 1, format, GL_UNSIGNED_BYTE, images[imageIndex].PixelData());
   }

   if (mipmaps)
   {
      glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
   }
}

TextureArray::~TextureArray()
{
   glDeleteTextures(1, &id);
}

bool TextureArray::ImagesAreWellFormed(const std::vector<Image>& images)
{
   if (images.empty())
   {
      return false;
   }

   unsigned int width = images[0].Width();
   unsigned int height = images[0].Height();

   if ((width == 0) || (height == 0))
   {
      return false;
   }

   unsigned int numPixelComponents = images[0].NumPixelComponents();

   if (!Image::ValidPixelComponents(numPixelComponents))
   {
      return false;
   }

   for (std::size_t index = 1, numImages = images.size(); index < numImages; ++index)
   {
      if ((images[index].Width() != width) || (images[index].Height() != height) || (images[index].NumPixelComponents() != numPixelComponents))
      {
         return false;
      }
   }

   return true;
}

void TextureArray::Bind() const
{
   glBindTexture(GL_TEXTURE_2D_ARRAY, id);
}

}