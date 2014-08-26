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

bool LoadOGG(const std::string& fullFilePath, SoundData& soundData);
bool LoadOGG(const MountedFilePath& mountedFilePath, SoundData& soundData);
bool LoadOGG(DataStream& oggDataStream, SoundData& soundData);

}