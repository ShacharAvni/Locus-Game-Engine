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

#include "GLInfo.h"

#include <string>

namespace Locus
{

namespace ShaderSource
{

LOCUS_RENDERING_API std::string Vert(GLInfo::GLSLVersion version, bool textured, unsigned int numLights);
LOCUS_RENDERING_API std::string Frag(GLInfo::GLSLVersion version, bool textured, unsigned int numLights);

LOCUS_RENDERING_API std::string GetMultiVariableName(const std::string& variableName, unsigned int index);

}

}