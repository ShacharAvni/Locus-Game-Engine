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

#include "WAVLoading.h"
#include "OGGLoading.h"
#include "SoundData.h"
#include "OpenALUtil.h"

#include "Locus/Common/Parsing.h"
#include "Locus/Common/Casts.h"
#include "Locus/Common/Exception.h"

#include "Locus/FileSystem/DataStream.h"
#include "Locus/FileSystem/MountedFilePath.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <string>

#include <cassert>

namespace Locus
{

struct SoundEffectInternal
{
   SoundData data;

   int size;

   unsigned int bufferID;
   unsigned int sourceID;
};

SoundEffect::SoundEffect()
   : soundEffectInternal(std::make_unique<SoundEffectInternal>())
{
   alGenBuffers(1, &soundEffectInternal->bufferID);

   ALenum error = alGetError();

   if (error != AL_NO_ERROR)
   {
      throw Exception(std::string("An OpenAL error occurred while generating buffers: ") + OpenALErrorToString(error));
   }

   alGenSources(1, &soundEffectInternal->sourceID);

   error = alGetError();

   if (error != AL_NO_ERROR)
   {
      throw Exception(std::string("An OpenAL error occurred while generating sources: ") + OpenALErrorToString(error));
   }

   Clear();

   SetPosition(0.0f, 0.0f, 0.0f);
}

SoundEffect::~SoundEffect()
{
   alDeleteSources(1, &soundEffectInternal->sourceID);

   assert(alGetError() == AL_NO_ERROR);

   alDeleteBuffers(1, &soundEffectInternal->bufferID);

   assert(alGetError() == AL_NO_ERROR);
}

void SoundEffect::Clear()
{
   soundEffectInternal->data.format = 0;
   soundEffectInternal->data.sampleRate = 0;
   soundEffectInternal->data.rawData.clear();

   soundEffectInternal->size = 0;
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
      loaded = LoadWAV(loadSource, soundEffectInternal->data);
      break;

   case SoundFileType::OGG:
      loaded = LoadOGG(loadSource, soundEffectInternal->data);
      break;
   }

   if (loaded)
   {
      BufferData();
      soundEffectInternal->data.rawData.clear();
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
   const SoundData& soundData = soundEffectInternal->data;

   soundEffectInternal->size = LossyCast<std::size_t, int>(soundData.rawData.size());

   alBufferData(soundEffectInternal->bufferID, soundData.format, soundData.rawData.data(), soundEffectInternal->size, soundData.sampleRate);
   alSourcei(soundEffectInternal->sourceID, AL_BUFFER, soundEffectInternal->bufferID);
}

void SoundEffect::SetPosition(float x, float y, float z) const
{
   alSource3f(soundEffectInternal->sourceID, AL_POSITION, x, y, z);
}

bool SoundEffect::IsLoaded() const
{
   return (soundEffectInternal->size > 0);
}

void SoundEffect::Play() const
{
   if (IsLoaded())
   {
      alSourcePlay(soundEffectInternal->sourceID);
   }
}

bool SoundEffect::IsPlaying() const
{
   ALint sourceStateValue = 0;

   alGetSourcei(soundEffectInternal->sourceID, AL_SOURCE_STATE, &sourceStateValue);

   return (sourceStateValue == AL_PLAYING);
}

}