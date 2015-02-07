/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/Image.h"

#include "Locus/Common/Casts.h"
#include "Locus/Common/ScopeFinalizer.h"
#include "Locus/Common/Util.h"
#include "Locus/Common/Exception.h"

#include "Locus/FileSystem/File.h"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include <cassert>

namespace Locus
{

Image::Image(unsigned int width, unsigned int height, unsigned int numPixelComponents)
   : width(width), height(height), numPixelComponents(numPixelComponents), pixelData(width * height * numPixelComponents)
{
   assert(Image::ValidPixelComponents(numPixelComponents));
}

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
      File file(mountedFilePath, DataStream::OpenMode::Read);

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
   assert(Image::ValidPixelComponents(numPixelComponents));
}

void Image::FinishLoad(unsigned char* pixels, unsigned int width, unsigned int height, unsigned int numPixelComponents)
{
   ScopeFinalizer pixelsFreer([&pixels]()
   {
      stbi_image_free(pixels);
   });

   this->width = width;
   this->height = height;
   this->numPixelComponents = numPixelComponents;

   pixelData.insert(pixelData.end(), pixels, pixels + (numPixelComponents * width * height));

   FlipVertically();
}

bool Image::ValidPixelComponents(unsigned int numPixelComponents)
{
   return ((numPixelComponents > 0) && (numPixelComponents <= 4));
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

unsigned int Image::GetPixelOffset(unsigned int x, unsigned int y) const
{
   assert(x < width);
   assert(y < height);

   return Image::GetPixelOffset(x, y, width, numPixelComponents);
}

unsigned int Image::GetPixelOffset(unsigned int x, unsigned int y, unsigned int width, unsigned int numPixelComponents)
{
   return (numPixelComponents * ((y * width) + x));
}

const unsigned char* Image::GetPixel(unsigned int x, unsigned int y) const
{
   return const_cast<Image*>(this)->GetPixel(x, y);
}

unsigned char* Image::GetPixel(unsigned int x, unsigned int y)
{
   return &(pixelData[GetPixelOffset(x, y)]);
}

void Image::SetPixelComponents(unsigned int numPixelComponents)
{
   assert(Image::ValidPixelComponents(numPixelComponents));

   if (this->numPixelComponents != numPixelComponents)
   {
      std::vector<unsigned char> newPixelData(numPixelComponents * width * height);

      unsigned int minPixelComponents = std::min(this->numPixelComponents, numPixelComponents);

      for (unsigned int x = 0; x < width; ++x)
      {
         for (unsigned int y = 0; y < height; ++y)
         {
            const unsigned char* pixel = GetPixel(x, y);

            unsigned int offsetIntoNewPixelData = Image::GetPixelOffset(x, y, width, numPixelComponents);

            for (unsigned int component = 0; component < minPixelComponents; ++component)
            {
               newPixelData[offsetIntoNewPixelData + component] = pixel[component];
            }
         }
      }

      this->numPixelComponents = numPixelComponents;

      pixelData = std::move(newPixelData);
   }
}

void Image::GetSubImage(unsigned int x, unsigned int y, unsigned int rectWidth, unsigned int rectHeight, std::vector<unsigned char>& subImagePixelData) const
{
   GetSubImage(x, y, rectWidth, rectHeight, numPixelComponents, subImagePixelData);
}

void Image::GetSubImage(unsigned int x, unsigned int y, unsigned int rectWidth, unsigned int rectHeight, unsigned int rectPixelComponents, std::vector<unsigned char>& subImagePixelData) const
{
   assert(x < width);
   assert(y < height);
   assert((x + rectWidth) <= width);
   assert((y + rectHeight) <= height);
   assert(rectPixelComponents <= numPixelComponents);

   subImagePixelData.resize(rectWidth * rectHeight * rectPixelComponents);

   for (unsigned int pixelX = x; pixelX < (x + rectWidth); ++pixelX)
   {
      for (unsigned int pixelY = y; pixelY < (y + rectHeight); ++pixelY)
      {
         const unsigned char* pixel = GetPixel(pixelX, pixelY);

         unsigned int offsetIntoSubImage = Image::GetPixelOffset(pixelX - x, pixelY - y, rectWidth, rectPixelComponents);

         for (unsigned int component = 0; component < rectPixelComponents; ++component)
         {
            subImagePixelData[offsetIntoSubImage + component] = pixel[component];
         }
      }
   }
}

void Image::SetSubImage(unsigned int x, unsigned int y, unsigned int rectWidth, unsigned int rectHeight, const std::vector<unsigned char>& subImagePixelData)
{
   SetSubImage(x, y, rectWidth, rectHeight, numPixelComponents, subImagePixelData);
}

void Image::SetSubImage(unsigned int x, unsigned int y, unsigned int rectWidth, unsigned int rectHeight, unsigned int rectPixelComponents, const std::vector<unsigned char>& subImagePixelData)
{
   assert(x < width);
   assert(y < height);
   assert((x + rectWidth) <= width);
   assert((y + rectHeight) <= height);
   assert(rectPixelComponents <= numPixelComponents);
   assert(subImagePixelData.size() == (rectWidth * rectHeight * rectPixelComponents));

   for (unsigned int pixelX = x; pixelX < (x + rectWidth); ++pixelX)
   {
      for (unsigned int pixelY = y; pixelY < (y + rectHeight); ++pixelY)
      {
         unsigned char* pixel = GetPixel(pixelX, pixelY);

         unsigned int offsetIntoSubImage = Image::GetPixelOffset(pixelX - x, pixelY - y, rectWidth, rectPixelComponents);

         for (unsigned int component = 0; component < rectPixelComponents; ++component)
         {
            pixel[component] = subImagePixelData[offsetIntoSubImage + component];
         }
      }
   }
}

void Image::FlipVertically()
{
   unsigned int middleY = height / 2;

   for (unsigned int yOffset = 0; yOffset < middleY; ++yOffset)
   {
      for (unsigned int pixelX = 0; pixelX < width; ++pixelX)
      {
         unsigned char* pixelFromTop = GetPixel(pixelX, yOffset);
         unsigned char* pixelFromBottom = GetPixel(pixelX, (height - yOffset - 1));

         for (unsigned int component = 0; component < numPixelComponents; ++component)
         {
            std::swap(pixelFromTop[component], pixelFromBottom[component]);
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
               unsigned char* pixel = GetPixel(originalPixelIndexX, originalPixelIndexY);

               for (unsigned int component = 0; component < numPixelComponents; ++component)
               {
                  singlePixel[component] += pixel[component];
               }
            }
         }

         unsigned int pixelOffset = Image::GetPixelOffset(newPixelIndexX, newPixelIndexY, newWidth, numPixelComponents);

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

bool Image::SaveAsBMP(const std::string& filePath) const
{
   return (stbi_write_bmp(filePath.c_str(), Width(), Height(), NumPixelComponents(), pixelData.data()) != 0);
}

bool Image::SaveAsPNG(const std::string& filePath) const
{
   return (stbi_write_png(filePath.c_str(), Width(), Height(), NumPixelComponents(), pixelData.data(), 0) != 0);
}

bool Image::SaveAsTGA(const std::string& filePath) const
{
   return (stbi_write_tga(filePath.c_str(), Width(), Height(), NumPixelComponents(), pixelData.data()) != 0);
}

}