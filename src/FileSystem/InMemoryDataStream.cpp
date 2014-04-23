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