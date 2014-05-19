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

ShaderProgram::ShaderProgram(const Shader& shader1, const Shader& shader2, bool doesTexturing, bool doesLighting)
   : id(glCreateProgram()), doesTexturing(doesTexturing), doesLighting(doesLighting)
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
}

ShaderProgram::~ShaderProgram()
{
   glDeleteProgram(id);
}

GLint ShaderProgram::GetAttributeLocation(const std::string& attribute) const
{
   std::map<std::string, GLint>::const_iterator iter = attributeLocationMap.find(attribute);

   if (iter != attributeLocationMap.end())
   {
      return iter->second;
   }
   else
   {
      GLint attributeLocation = glGetAttribLocation(id, attribute.c_str());

      attributeLocationMap[attribute] = attributeLocation;

      return attributeLocation;
   }
}

GLint ShaderProgram::GetUniformLocation(const std::string& uniform) const
{
   std::map<std::string, GLint>::const_iterator iter = uniformLocationMap.find(uniform);

   if (iter != uniformLocationMap.end())
   {
      return iter->second;
   }
   else
   {
      GLint uniformLocation = glGetUniformLocation(id, uniform.c_str());

      uniformLocationMap[uniform] = uniformLocation;

      return uniformLocation;
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