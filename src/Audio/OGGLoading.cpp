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

#include "Locus/Audio/OGGLoading.h"
#include "Locus/Audio/SoundData.h"

#include "Locus/Common/Endian.h"
#include "Locus/Common/Casts.h"

#include "Locus/FileSystem/FileOnDisk.h"
#include "Locus/FileSystem/File.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <vorbis/vorbisfile.h>

#include <cstdio>

namespace Locus
{

bool LoadOGG(const std::string& fullFilePath, SoundData& soundData)
{
   FileOnDisk fileOnDisk(fullFilePath, DataStream::OpenOperation::Read);

   return LoadOGG(fileOnDisk, soundData);
}

bool LoadOGG(const MountedFilePath& mountedFilePath, SoundData& soundData)
{
   File file(mountedFilePath, DataStream::OpenOperation::Read);

   return LoadOGG(file, soundData);
}

bool LoadOGG(DataStream& oggDataStream, SoundData& soundData)
{
   struct Callbacks : public ov_callbacks
   {
      Callbacks()
      {
         read_func = actual_read_func;
         seek_func = actual_seek_func;
         close_func = actual_close_func;
         tell_func = actual_tell_func;
      }

      static std::size_t actual_read_func(void* ptr, std::size_t size, std::size_t nmemb, void* datasource)
      {
         DataStream* dataStream = reinterpret_cast<DataStream*>(datasource);

         return (dataStream->Read(reinterpret_cast<char*>(ptr), size * nmemb) / size);
      }

      static int actual_seek_func(void* datasource, ogg_int64_t offset, int whence)
      {
         DataStream* dataStream = reinterpret_cast<DataStream*>(datasource);

         std::size_t offsetAsSizeT = LossyCast<ogg_int64_t, std::size_t>(offset);

         bool seekSuccessful = false;

         switch (whence)
         {
         case SEEK_SET:
            seekSuccessful = dataStream->Seek(offsetAsSizeT, DataStream::SeekType::Beginning);
            break;

         case SEEK_CUR:
            seekSuccessful = dataStream->Seek(offsetAsSizeT, DataStream::SeekType::Current);
            break;

         case SEEK_END:
            seekSuccessful = dataStream->Seek(offsetAsSizeT, DataStream::SeekType::End);
            break;
         }

         return seekSuccessful ? 0 : -1;
      }

      static int actual_close_func(void* /*datasource*/)
      {
         return 0;
      }

      static long actual_tell_func(void* datasource)
      {
         DataStream* dataStream = reinterpret_cast<DataStream*>(datasource);

         return LossyCast<std::size_t, long>(dataStream->CurrentPosition());
      }
   };

   OggVorbis_File oggFile;

   if (ov_open_callbacks(&oggDataStream, &oggFile, NULL, 0, Callbacks()) != 0)
   {
      return false;
   }

   vorbis_info* info = ov_info(&oggFile, -1);

   int dataLength = LossyCast<ogg_int64_t, int>( ov_pcm_total(&oggFile, -1) ) * info->channels * 2;    // always 16 bit data

   if (dataLength <= 0)
   {
      return false;
   }

   soundData.format = (info->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
   soundData.sampleRate = info->rate;

   soundData.rawData.resize(dataLength);

   int bytesRemaining = dataLength;

   int bitStream = 0;
   char* dataPointer = soundData.rawData.data();

   int endian = (SystemEndian() == Endian::Little) ? 0 : 1;

   do
   {
      long bytesRead = ov_read(&oggFile, dataPointer, bytesRemaining, endian, 2, 1, &bitStream);
      bytesRemaining -= bytesRead;
      dataPointer += bytesRead;
   } while (bytesRemaining > 0);

   ov_clear(&oggFile);

   return true;
}

}