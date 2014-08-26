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

#include "LocusRenderingAPI.h"

#include "Mesh.h"

#include <string>

namespace Locus
{

class DataStream;
struct MountedFilePath;

LOCUS_RENDERING_API bool ParseSAPFile(const std::string& fullFilePath, std::vector<Mesh>& meshes);
LOCUS_RENDERING_API bool ParseSAPFile(const MountedFilePath& mountedFilePath, std::vector<Mesh>& meshes);
LOCUS_RENDERING_API bool ParseSAPFile(DataStream& sapDataStream, std::vector<Mesh>& meshes);

}