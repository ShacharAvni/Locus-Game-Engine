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

typedef std::vector<std::unique_ptr<Mesh>> MeshVector_t;

LOCUS_RENDERING_API bool ParseSAPFile(const std::string& fullFilePath, MeshVector_t& meshes);
LOCUS_RENDERING_API bool ParseSAPFile(const MountedFilePath& mountedFilePath, MeshVector_t& meshes);
LOCUS_RENDERING_API bool ParseSAPFile(DataStream& sapDataStream, MeshVector_t& meshes);

}