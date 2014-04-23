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