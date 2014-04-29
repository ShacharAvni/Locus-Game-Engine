 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusAudioAPI.h"
#include "SoundData.h"

#include "Locus/FileSystem/MountedFilePath.h"

#include <string>
#include <vector>

namespace Locus
{

class DataStream;

class LOCUS_AUDIO_API SoundEffect
{
public:
   SoundEffect();
   ~SoundEffect();

   enum class SoundFileType
   {
      WAV,
      OGG
   };

   bool Load(const std::string& fullFilePath);
   bool Load(const MountedFilePath& mountedFilePath);

   bool Load(const std::string& fullFilePath, SoundFileType soundFileType);
   bool Load(const MountedFilePath& mountedFilePath, SoundFileType soundFileType);
   bool Load(DataStream& dataStream, SoundFileType soundFileType);

   void SetPosition(float x, float y, float z) const;

   void Play() const;

   bool IsLoaded() const;

private:
   SoundData soundData;
   int size;

   unsigned int bufferID;
   unsigned int sourceID;

   void BufferData();

   template <class PathType>
   bool DoLoadFromDeducedExtension(const PathType& loadPath);

   template <class LoadSource>
   bool DoLoad(LoadSource& loadSource, SoundFileType soundFileType);

   static bool DeduceSoundFileTypeFromExtension(const std::string& filePath, SoundFileType& soundFileType);
   static bool DeduceSoundFileTypeFromExtension(const MountedFilePath& mountedFilePath, SoundFileType& soundFileType);

   void Clear();
};

}