 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Rendering/Shader.h"

#include "Locus/Common/Exception.h"

#include <Locus/Rendering/Locus_glew.h>

#include <vector>

namespace Locus
{

Shader::Shader(ShaderType type, const std::string& source)
   : id( (type == ShaderType::Vertex) ? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER) )
{
   const char* c_str = source.c_str();
   glShaderSource(id, 1, &c_str, nullptr);

   glCompileShader(id);

   GLint compiledResult = GL_FALSE;

   glGetShaderiv(id, GL_COMPILE_STATUS, &compiledResult);

   if (compiledResult != GL_TRUE)
   {
      GLint logLength = 0;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

      std::string log;

      if (logLength > 0)
      {
         std::vector<char> logBuffer(logLength);
         glGetShaderInfoLog(id, logLength, nullptr, logBuffer.data());

         log = logBuffer.data();
      }

      glDeleteShader(id);

      throw Exception(std::string("Shader compilation failed.\n\nLog:\n") + log);
   }
}

Shader::~Shader()
{
   glDeleteShader(id);
}

}