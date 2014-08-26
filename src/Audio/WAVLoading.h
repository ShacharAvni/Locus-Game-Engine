/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#pragma once

#include <string>

namespace Locus
{

struct SoundData;
class DataStream;
struct MountedFilePath;

bool LoadWAV(const std::string& fullFilePath, SoundData& soundData);
bool LoadWAV(const MountedFilePath& mountedFilePath, SoundData& soundData);
bool LoadWAV(DataStream& wavDataStream, SoundData& soundData);

}