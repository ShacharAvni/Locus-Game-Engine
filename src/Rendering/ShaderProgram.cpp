/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

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