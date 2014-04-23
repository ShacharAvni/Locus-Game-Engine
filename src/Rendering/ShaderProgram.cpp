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

#include "Locus/Rendering/ShaderProgram.h"
#include "Locus/Rendering/ShaderSourceStore.h"
#include "Locus/Rendering/ShaderLinkException.h"

#include <Locus/Rendering/Locus_glew.h>

#include <vector>

namespace Locus
{

ShaderProgram::ShaderProgram(const Shader& shader1, const Shader& shader2, bool doesTexturing, bool doesLighting, const std::vector<std::string>& attributes, const std::vector<std::string>& uniforms)
   : id(glCreateProgram()), doesTexturing(doesTexturing), doesLighting(doesLighting), attributes(attributes), uniforms(uniforms)
{
   glAttachShader(id, shader1.id);
   glAttachShader(id, shader2.id);

   glLinkProgram(id);

   GLint linkedResult = GL_FALSE;
   glGetProgramiv(id, GL_LINK_STATUS, &linkedResult);

   glDetachShader(id, shader1.id);
   glDetachShader(id, shader2.id);

   if (linkedResult != GL_TRUE)
   {
      GLint logLength = 0;
      glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

      std::string log;

      if (logLength > 0)
      {
         std::vector<char> logBuffer(logLength);
         glGetProgramInfoLog(id, logLength, nullptr, logBuffer.data());

         log = logBuffer.data();
      }

      glDeleteProgram(id);

      throw ShaderLinkException(std::string("Shader program failed to link.\n\nLog:\n") + log);
   }

   CacheAttributes();
   CacheUniforms();
}

ShaderProgram::~ShaderProgram()
{
   glDeleteProgram(id);
}

GLint ShaderProgram::GetAttributeOrUniformLocation(const char* attribute) const
{
   std::map<const char*, GLint>::const_iterator iter = attributeAndUniformLocationMap.find(attribute);

   if (iter != attributeAndUniformLocationMap.end())
   {
      return iter->second;
   }
   else
   {
      return -1;
   }
}

void ShaderProgram::CacheAttributes()
{
   CacheAttributesOrUniforms(attributes, glGetAttribLocation);
}

void ShaderProgram::CacheUniforms()
{
   CacheAttributesOrUniforms(uniforms, glGetUniformLocation);
}

void ShaderProgram::CacheAttributesOrUniforms(const std::vector<std::string>& attributesOrUniforms, const std::function<GLint(GLuint, const GLchar*)>& locationAcquisitionFunction)
{
   for (const std::string& attributeOrUniform : attributesOrUniforms)
   {
      attributeAndUniformLocationMap[attributeOrUniform.c_str()] = locationAcquisitionFunction(id, attributeOrUniform.c_str());
   }
}

void ShaderProgram::Use() const
{
   glUseProgram(id);
}

void ShaderProgram::Stop()
{
   glUseProgram(0);
}

}