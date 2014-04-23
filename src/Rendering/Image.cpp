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

#include "Locus/Rendering/Image.h"

#include "Locus/Common/Casts.h"
#include "Locus/Common/ScopeFinalizer.h"
#include "Locus/Common/Util.h"
#include "Locus/Common/Exception.h"

#include "Locus/FileSystem/File.h"

#include "stb_image/stb_image.h"

#include <cassert>

namespace Locus
{

Image::Image(const std::string& filePath)
{
   int numPixelsX = 0;
   int numPixelsY = 0;
   int numPixelComponentsAsInt = 0;

   unsigned char* pixels = stbi_load(filePath.c_str(), &numPixelsX, &numPixelsY, &numPixelComponentsAsInt, 0);

   if (pixels == nullptr)
   {
      throw Exception(std::string("Failed to load image ") + filePath);
   }

   FinishLoad(pixels, static_cast<unsigned int>(numPixelsX), static_cast<unsigned int>(numPixelsY), static_cast<unsigned int>(numPixelComponentsAsInt));
}

Image::Image(const MountedFilePath& mountedFilePath)
{
   int numPixelsX = 0;
   int numPixelsY = 0;
   int numPixelComponentsAsInt = 0;

   unsigned char* pixels = nullptr;

   {
      File file(mountedFilePath, DataStream::OpenOperation::Read);

      std::vector<char> bytes;
      file.ReadWholeFile(bytes);

      const stbi_uc* bytesInMemory = reinterpret_cast<const stbi_uc*>(bytes.data());

      pixels = stbi_load_from_memory(bytesInMemory, LossyCast<std::size_t, int>(bytes.size()), &numPixelsX, &numPixelsY, &numPixelComponentsAsInt, 0);
   }

   if (pixels == nullptr)
   {
      throw Exception(std::string("Failed to load image ") + mountedFilePath.path);
   }

   FinishLoad(pixels, static_cast<unsigned int>(numPixelsX), static_cast<unsigned int>(numPixelsY), static_cast<unsigned int>(numPixelComponentsAsInt));
}

Image::Image(const unsigned char* pixelData, unsigned int width, unsigned int height, unsigned int numPixelComponents)
   : pixelData(pixelData, pixelData + (numPixelComponents * width * height)), numPixelComponents(numPixelComponents), width(width), height(height)
{
}

void Image::FinishLoad(unsigned char* pixels, unsigned int width, unsigned int height, unsigned int numPixelComponents)
{
   ScopeFinalizer pixelsFreer([&pixels]()
   {
      stbi_image_free(pixels);
   });

   this->numPixelComponents = numPixelComponents;
   this->width = width;
   this->height = height;

   pixelData.insert(pixelData.end(), pixels, pixels + (numPixelComponents * width * height));

   FlipVertically();
}

unsigned int Image::Width() const
{
   return width;
}

unsigned int Image::Height() const
{
   return height;
}

unsigned int Image::NumPixelComponents() const
{
   return numPixelComponents;
}

const unsigned char* Image::PixelData() const
{
   return pixelData.data();
}

void Image::ClearPixelData()
{
   Util::ClearAndShrink(pixelData);
}

void Image::FlipVertically()
{
   unsigned int middleY = height / 2;

   for (unsigned int yOffset = 0; yOffset <= middleY; ++yOffset)
   {
      for (unsigned int pixelX = 0; pixelX < width; ++pixelX)
      {
         unsigned int offsetFromTop = (numPixelComponents * ((yOffset * width) + pixelX));
         unsigned int offsetFromBottom = (numPixelComponents * ((height - yOffset - 1) * width + pixelX));

         for (unsigned int component = 0; component < numPixelComponents; ++component)
         {
            std::swap(pixelData[offsetFromTop + component], pixelData[offsetFromBottom + component]);
         }
      }
   }
}

void Image::Scale(unsigned int newWidth, unsigned int newHeight)
{
   assert(newWidth != 0);
   assert(newHeight != 0);

   float newPixelWidth = (width / static_cast<float>(newWidth));
   float newPixelHeight = (height / static_cast<float>(newHeight));

   std::vector<unsigned char> newPixelData(numPixelComponents * newWidth * newHeight);

   std::vector<int> singlePixel(numPixelComponents);

   for (unsigned int newPixelIndexY = 0; newPixelIndexY < newHeight; ++newPixelIndexY)
   {
      for (unsigned int newPixelIndexX = 0; newPixelIndexX < newWidth; ++newPixelIndexX)
      {
         float xFrom = newPixelIndexX * newPixelWidth;
         float xTo = xFrom + newPixelWidth;
         float yFrom = newPixelIndexY * newPixelHeight;
         float yTo = yFrom + newPixelHeight;

         unsigned int oldPixelXFrom = static_cast<unsigned int>( std::floor(xFrom) );
         unsigned int oldPixelXTo = static_cast<unsigned int>( std::floor(xTo) );
         unsigned int oldPixelYFrom = static_cast<unsigned int>( std::floor(yFrom) );
         unsigned int oldPixelYTo = static_cast<unsigned int>( std::floor(yTo) );

         if (oldPixelXFrom == width)
         {
            oldPixelXFrom = width - 1;
         }

         if (oldPixelXTo == width)
         {
            oldPixelXTo = width - 1;
         }

         if (oldPixelYFrom == height)
         {
            oldPixelYFrom = height - 1;
         }

         if (oldPixelYTo == height)
         {
            oldPixelYTo = height - 1;
         }

         for (unsigned int component = 0; component < numPixelComponents; ++component)
         {
            singlePixel[component] = 0;
         }

         int numPixelsHit = ((oldPixelXTo - oldPixelXFrom + 1) * (oldPixelYTo - oldPixelYFrom + 1));

         for (unsigned int originalPixelIndexY = oldPixelYFrom; originalPixelIndexY <= oldPixelYTo; ++originalPixelIndexY)
         {
            for (unsigned int originalPixelIndexX = oldPixelXFrom; originalPixelIndexX <= oldPixelXTo; ++originalPixelIndexX)
            {
               unsigned int pixelOffset = (numPixelComponents * ((originalPixelIndexY * width) + originalPixelIndexX));

               for (unsigned int component = 0; component < numPixelComponents; ++component)
               {
                  singlePixel[component] += pixelData[pixelOffset + component];
               }
            }
         }

         unsigned int pixelOffset = (numPixelComponents * ((newPixelIndexY * newWidth) + newPixelIndexX));

         for (unsigned int component = 0; component < numPixelComponents; ++component)
         {
            newPixelData[pixelOffset + component] = static_cast<unsigned char>(singlePixel[component] / static_cast<float>(numPixelsHit));
         }
      }
   }

   pixelData = std::move(newPixelData);
   width = newWidth;
   height = newHeight;
}

}