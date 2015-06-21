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

#include <vector>

#include <cassert>

namespace Locus
{

template <class T>
class Array3D
{
public:
   Array3D(std::size_t xSize, std::size_t ySize, std::size_t zSize)
      : xSize(xSize),
        ySize(ySize),
        zSize(zSize),
        data(Size())
   {
   }

   Array3D(std::size_t xSize, std::size_t ySize, std::size_t zSize, const T& value)
      : xSize(xSize),
        ySize(ySize),
        zSize(zSize),
        data(Size(), value)
   {
   }

   std::size_t Size() const
   {
      return (xSize * ySize * zSize);
   }

   std::size_t XSize() const
   {
      return xSize;
   }

   std::size_t YSize() const
   {
      return ySize;
   }

   std::size_t ZSize() const
   {
      return zSize;
   }

   typename std::vector<T>::iterator begin()
   {
      return data.begin();
   }

   typename std::vector<T>::const_iterator begin() const
   {
      return data.begin();
   }

   typename std::vector<T>::iterator end()
   {
      return data.end();
   }

   typename std::vector<T>::const_iterator end() const
   {
      return data.end();
   }

   std::size_t FlattenedIndex(std::size_t xIndex, std::size_t yIndex, std::size_t zIndex) const
   {
      assert((xIndex < xSize) && (yIndex < ySize) && (zIndex < zSize));

      return (zIndex * xSize * ySize) + (yIndex * xSize) + xIndex;
   }

   void IndexesFromFlattenedIndex(std::size_t flattenedIndex, std::size_t& xIndex, std::size_t& yIndex, std::size_t& zIndex) const
   {
      assert(flattenedIndex < Size());

      zIndex = (flattenedIndex / (ySize * xSize));
      xIndex = (flattenedIndex % xSize);
      yIndex = (((flattenedIndex - xIndex) % (ySize * xSize)) / xSize);
   }

   T& At(std::size_t flattenedIndex)
   {
      assert(flattenedIndex < Size());

      return data[flattenedIndex];
   }

   const T& At(std::size_t flattenedIndex) const
   {
      return const_cast<Array3D*>(this)->At(flattenedIndex);
   }

   T& At(std::size_t xIndex, std::size_t yIndex, std::size_t zIndex)
   {
      return At(FlattenedIndex(xIndex, yIndex, zIndex));
   }

   const T& At(std::size_t xIndex, std::size_t yIndex, std::size_t zIndex) const
   {
      return const_cast<Array3D*>(this)->At(xIndex, yIndex, zIndex);
   }

private:
   std::size_t xSize;
   std::size_t ySize;
   std::size_t zSize;

   std::vector<T> data;
};

}