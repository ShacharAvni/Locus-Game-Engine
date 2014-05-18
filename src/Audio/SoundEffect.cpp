/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

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