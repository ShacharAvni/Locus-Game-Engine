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