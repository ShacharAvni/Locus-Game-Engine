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

#include "GLCommonTypes.h"

#include <string>

namespace Locus
{

class LOCUS_RENDERING_API Shader
{
public:
   enum class ShaderType
   {
      Vertex = 0,
      Fragment
   };

   Shader(ShaderType type, const std::string& source);
   ~Shader();

   Shader(const Shader&) = delete;
   Shader& operator=(const Shader&) = delete;

   const GLuint id;
};

}