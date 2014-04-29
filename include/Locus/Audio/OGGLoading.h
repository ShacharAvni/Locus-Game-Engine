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

#include "Locus/FileSystem/MountedFilePath.h"

#include <string>
#include <vector>

namespace Locus
{

struct SoundData;
class DataStream;

LOCUS_AUDIO_API bool LoadOGG(const std::string& fullFilePath, SoundData& soundData);
LOCUS_AUDIO_API bool LoadOGG(const MountedFilePath& mountedFilePath, SoundData& soundData);
LOCUS_AUDIO_API bool LoadOGG(DataStream& oggDataStream, SoundData& soundData);

}