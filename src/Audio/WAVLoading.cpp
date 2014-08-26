/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "WAVLoading.h"
#include "SoundData.h"

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
   FileOnDisk fileOnDisk(fullFilePath, DataStream::OpenMode::Read);

   return LoadWAV(fileOnDisk, soundData);
}

bool LoadWAV(const MountedFilePath& mountedFilePath, SoundData& soundData)
{
   File file(mountedFilePath, DataStream::OpenMode::Read);

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