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

#include "LocusFileSystemAPI.h"

#include <vector>

namespace Locus
{

class LOCUS_FILE_SYSTEM_API DataStream
{
public:
   virtual ~DataStream();

   virtual std::size_t Read(char* bytes, std::size_t numBytesToRead) = 0;
   std::size_t Read(std::vector<char>& bytes, std::size_t numBytesToRead, std::size_t offsetIntoBytesVector);

   virtual bool IsEndOfStream() const = 0;

   virtual std::size_t CurrentPosition() const = 0;

   virtual std::size_t SizeInBytes() const = 0;

   enum class SeekType
   {
      Beginning,
      Current,
      End
   };

   virtual bool Seek(std::size_t offset, SeekType seekType) = 0;

   enum class OpenOperation
   {
      Read,
      Write,
      Append
   };
};

}