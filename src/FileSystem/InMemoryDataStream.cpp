 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/FileSystem/InMemoryDataStream.h"

namespace Locus
{

InMemoryDataStream::InMemoryDataStream(const std::vector<char>& data)
   : data(data), streamPosition(0)
{
}

InMemoryDataStream::InMemoryDataStream(std::vector<char>&& data)
   : data(std::move(data)), streamPosition(0)
{
}

bool InMemoryDataStream::IsEndOfStream() const
{
   return (BytesRemaining() == 0);
}

std::size_t InMemoryDataStream::BytesRemaining() const
{
   return (SizeInBytes() - streamPosition);
}

std::size_t InMemoryDataStream::CurrentPosition() const
{
   return streamPosition;
}

std::size_t InMemoryDataStream::SizeInBytes() const
{
   return data.size();
}

bool InMemoryDataStream::Seek(std::size_t offset, SeekType seekType)
{
   switch (seekType)
   {
   case SeekType::Beginning:
      return SeekFromBeginning(offset);
      break;

   case SeekType::Current:
      return SeekFromCurrentPosition(offset);
      break;

   case SeekType::End:
      return SeekFromEnd(offset);
      break;
   }

   return false;
}

bool InMemoryDataStream::SeekFromBeginning(std::size_t offset)
{
   if (offset <= SizeInBytes())
   {
      streamPosition = offset;
      return true;
   }

   return false;
}

bool InMemoryDataStream::SeekFromCurrentPosition(std::size_t offset)
{
   if (offset <= BytesRemaining())
   {
      streamPosition += offset;
      return true;
   }

   return false;
}

bool InMemoryDataStream::SeekFromEnd(std::size_t offset)
{
   if (offset <= SizeInBytes())
   {
      streamPosition = (data.size() - offset);
      return true;
   }

   return false;
}

std::size_t InMemoryDataStream::Read(char* bytes, std::size_t numBytesToRead)
{
   if (IsEndOfStream())
   {
      return 0;
   }

   std::size_t bytesRemaining = BytesRemaining();

   std::size_t bytesRead = (numBytesToRead > bytesRemaining) ? bytesRemaining : numBytesToRead;

   for (std::size_t readByteIndex = 0; readByteIndex < bytesRead; ++readByteIndex)
   {
      bytes[readByteIndex] = data[streamPosition];
      ++streamPosition;
   }

   return bytesRead;
}

void InMemoryDataStream::SetData(const std::vector<char>& data)
{
   this->data = data;
   streamPosition = 0;
}

void InMemoryDataStream::SetData(std::vector<char>&& data)
{
   this->data = data;
   streamPosition = 0;
}

}