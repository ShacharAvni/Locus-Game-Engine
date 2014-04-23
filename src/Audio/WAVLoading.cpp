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

#include "Locus/Audio/WAVLoading.h"
#include "Locus/Audio/SoundData.h"

#include "Locus/Common/Endian.h"

#include "Locus/FileSystem/FileOnDisk.h"
#include "Locus/FileSystem/File.h"

#include <AL/al.h>

#include <cstdint>
#include <cstring>

namespace Locus
{

bool LoadWAV(const std::string& fullFilePath, SoundData& soundData)
{
   FileOnDisk fileOnDisk(fullFilePath, DataStream::OpenOperation::Read);

   return LoadWAV(fileOnDisk, soundData);
}

bool LoadWAV(const MountedFilePath& mountedFilePath, SoundData& soundData)
{
   File file(mountedFilePath, DataStream::OpenOperation::Read);

   return LoadWAV(file, soundData);
}

bool LoadWAV(DataStream& wavDataStream, SoundData& soundData)
{
   std::size_t bytesRead = 0;

#define VERIFY_BYTES_READ(bytesTarget) if(bytesRead != bytesTarget) return false;

   char fourByteBuffer[4] = {0};

   bytesRead = wavDataStream.Read(fourByteBuffer, 4); 
   VERIFY_BYTES_READ(4)

   if (std::strncmp(fourByteBuffer, "RIFF", 4) != 0)
   {
      return false;
   }

   bool bigEndian = (SystemEndian() == Endian::Big);

   bytesRead = wavDataStream.Read(fourByteBuffer, 4);
   VERIFY_BYTES_READ(4)

   bytesRead = wavDataStream.Read(fourByteBuffer, 4);      //WAVE
   VERIFY_BYTES_READ(4)

   bytesRead = wavDataStream.Read(fourByteBuffer, 4);      //fmt
   VERIFY_BYTES_READ(4)

   bytesRead = wavDataStream.Read(fourByteBuffer, 4);      //16
   VERIFY_BYTES_READ(4)

   char twoByteBuffer[2] = {0};

   bytesRead = wavDataStream.Read(fourByteBuffer, 2);      //1
   VERIFY_BYTES_READ(2)

   bytesRead = wavDataStream.Read(twoByteBuffer, 2);
   VERIFY_BYTES_READ(2)

   EndianNumber<std::int16_t> twoBytes(twoByteBuffer);

   if (bigEndian)
   {
      twoBytes.ReverseEndian();
   }

   int channel = twoBytes.num;

   bytesRead = wavDataStream.Read(fourByteBuffer, 4);
   VERIFY_BYTES_READ(4)

   EndianNumber<std::int32_t> fourBytes(fourByteBuffer);

   if (bigEndian)
   {
      fourBytes.ReverseEndian();
   }

   soundData.sampleRate = fourBytes.num;

   bytesRead = wavDataStream.Read(fourByteBuffer, 4);
   VERIFY_BYTES_READ(4)

   bytesRead = wavDataStream.Read(twoByteBuffer, 2);
   VERIFY_BYTES_READ(2)

   bytesRead = wavDataStream.Read(twoByteBuffer, 2);
   VERIFY_BYTES_READ(2)

   twoBytes.Set(twoByteBuffer);

   if (bigEndian)
   {
      twoBytes.ReverseEndian();
   }

   int bps = twoBytes.num;

   bytesRead = wavDataStream.Read(fourByteBuffer, 4);
   VERIFY_BYTES_READ(4)

   bytesRead = wavDataStream.Read(fourByteBuffer, 4);
   VERIFY_BYTES_READ(4)

   fourBytes.Set(fourByteBuffer);

   if (bigEndian)
   {
      fourBytes.ReverseEndian();
   }

   std::size_t dataSize = fourBytes.num;

   if (channel == 1)
   {
      if (bps == 8)
      {
         soundData.format = AL_FORMAT_MONO8;
      }
      else
      {
         soundData.format = AL_FORMAT_MONO16;
      }
   }
   else
   {
      if (bps == 8)
      {
         soundData.format = AL_FORMAT_STEREO8;
      }
      else
      {
         soundData.format = AL_FORMAT_STEREO16;
      }
   }

   soundData.rawData.resize(dataSize);
   bytesRead = wavDataStream.Read(soundData.rawData.data(), dataSize);

   VERIFY_BYTES_READ(dataSize)

   return true;

#undef VERIFY_BYTES_READ
}

}