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

#include "Locus/Audio/SoundEffect.h"
#include "Locus/Audio/WAVLoading.h"
#include "Locus/Audio/OGGLoading.h"

#include "Locus/Common/Parsing.h"
#include "Locus/Common/Casts.h"

#include "Locus/FileSystem/DataStream.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>
#include <string>

namespace Locus
{

SoundEffect::SoundEffect()
{
   alGenBuffers(1, &bufferID);
   alGenSources(1, &sourceID);

   Clear();

   SetPosition(0.0f, 0.0f, 0.0f);
}

SoundEffect::~SoundEffect()
{
   alDeleteSources(1, &sourceID);
   alDeleteBuffers(1, &bufferID);
}

void SoundEffect::Clear()
{
   soundData.format = 0;
   soundData.sampleRate = 0;
   soundData.rawData.clear();

   size = 0;
}

bool SoundEffect::DeduceSoundFileTypeFromExtension(const std::string& filePath, SoundEffect::SoundFileType& soundFileType)
{
   //file path must have:
   //a name of at least one character
   //a 3 character extension (either wav or ogg)
   //and the "dot" character
   //Hence the file path must have at least 5 characters

   if (filePath.length() < 5)
   {
      return false;
   }

   std::string extension = filePath.substr(filePath.length() - 3);

   ToLower(extension);

   if (extension == "wav")
   {
      soundFileType = SoundFileType::WAV;
      return true;
   }
   else if (extension == "ogg")
   {
      soundFileType = SoundFileType::OGG;
      return true;
   }

   return false;
}

bool SoundEffect::DeduceSoundFileTypeFromExtension(const MountedFilePath& mountedFilePath, SoundEffect::SoundFileType& soundFileType)
{
   return SoundEffect::DeduceSoundFileTypeFromExtension(mountedFilePath.path, soundFileType);
}

template <class PathType>
bool SoundEffect::DoLoadFromDeducedExtension(const PathType& loadPath)
{
   SoundFileType soundFileType;

   if (SoundEffect::DeduceSoundFileTypeFromExtension(loadPath, soundFileType))
   {
      return Load(loadPath, soundFileType);
   }

   return false;
}

bool SoundEffect::Load(const std::string& fullFilePath)
{
   return DoLoadFromDeducedExtension<std::string>(fullFilePath);
}

bool SoundEffect::Load(const MountedFilePath& mountedFilePath)
{
   return DoLoadFromDeducedExtension<MountedFilePath>(mountedFilePath);
}

template <class LoadSource>
bool SoundEffect::DoLoad(LoadSource& loadSource, SoundFileType soundFileType)
{
   bool loaded = false;

   switch (soundFileType)
   {
   case SoundFileType::WAV:
      loaded = LoadWAV(loadSource, soundData);
      break;

   case SoundFileType::OGG:
      loaded = LoadOGG(loadSource, soundData);
      break;
   }

   if (loaded)
   {
      BufferData();
      soundData.rawData.clear();
   }
   else
   {
      Clear();
   }

   return loaded;
}

bool SoundEffect::Load(const std::string& fullFilePath, SoundEffect::SoundFileType soundFileType)
{
   return DoLoad<const std::string>(fullFilePath, soundFileType);
}

bool SoundEffect::Load(const MountedFilePath& mountedFilePath, SoundEffect::SoundFileType soundFileType)
{
   return DoLoad<const MountedFilePath>(mountedFilePath, soundFileType);
}

bool SoundEffect::Load(DataStream& dataStream, SoundFileType soundFileType)
{
   return DoLoad<DataStream>(dataStream, soundFileType);
}

void SoundEffect::BufferData()
{
   size = LossyCast<std::size_t, int>(soundData.rawData.size());

   alBufferData(bufferID, soundData.format, soundData.rawData.data(), size, soundData.sampleRate);
   alSourcei(sourceID, AL_BUFFER, bufferID);
}

void SoundEffect::SetPosition(float x, float y, float z) const
{
   alSource3f(sourceID, AL_POSITION, x, y, z);
}

bool SoundEffect::IsLoaded() const
{
   return (size > 0);
}

void SoundEffect::Play() const
{
   if (IsLoaded())
   {
      alSourcePlay(sourceID);
   }
}

}