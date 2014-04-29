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

#include "LocusRenderingAPI.h"

#include "Shader.h"
#include "GLCommonTypes.h"

#include <vector>
#include <string>
#include <map>
#include <functional>

#include <cstring>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API ShaderProgram
{
public:
   ShaderProgram(const Shader& shader1, const Shader& shader2, bool doesTexturing, bool doesLighting, const std::vector<std::string>& attributes, const std::vector<std::string>& uniforms);
   ShaderProgram(const ShaderProgram&) = delete;
   ShaderProgram& operator=(const ShaderProgram&) = delete;
   ~ShaderProgram();

   void Use() const;

   const GLuint id;
   const bool doesTexturing;
   const bool doesLighting;

   GLint GetAttributeOrUniformLocation(const char* attribute) const;

   static void Stop();

private:
   struct AttributeAndUniformLocationMapEquality
   {
      bool operator()(const char* a, const char* b) const
      {
         return (std::strcmp(a, b) < 0);
      }
   };

   std::map<const char*, GLint, AttributeAndUniformLocationMapEquality> attributeAndUniformLocationMap;

   std::vector<std::string> attributes;
   std::vector<std::string> uniforms;

   void CacheAttributes();
   void CacheUniforms();

   void CacheAttributesOrUniforms(const std::vector<std::string>& attributesOrUniforms, const std::function<GLint(GLuint, const GLchar*)>& locationAcquisitionFunction);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}