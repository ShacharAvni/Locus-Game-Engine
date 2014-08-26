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

#include "Shader.h"
#include "GLCommonTypes.h"

#include <string>
#include <map>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API ShaderProgram
{
public:
   ShaderProgram(const Shader& shader1, const Shader& shader2, bool doesTexturing, bool doesLighting);
   ~ShaderProgram();

   ShaderProgram(const ShaderProgram&) = delete;
   ShaderProgram& operator=(const ShaderProgram&) = delete;

   void Use() const;

   const GLuint id;
   const bool doesTexturing;
   const bool doesLighting;

   GLint GetAttributeLocation(const std::string& attribute) const;
   GLint GetUniformLocation(const std::string& uniform) const;

   static void Stop();

private:
   mutable std::map<std::string, GLint> attributeLocationMap;
   mutable std::map<std::string, GLint> uniformLocationMap;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}