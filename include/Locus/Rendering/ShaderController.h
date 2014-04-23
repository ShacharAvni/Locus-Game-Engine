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

#include "ShaderProgram.h"
#include "GLInfo.h"
#include "Light.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API ShaderController
{
public:
   ShaderController(const GLInfo& glInfo, GLInfo::GLSLVersion requiredGLSLVersion, bool useHighestSupportedGLSLVersion);
   ShaderController(const ShaderController&) = delete;
   ShaderController& operator=(const ShaderController&) = delete;

   GLInfo::GLSLVersion GetActiveGLSLVersion() const;

   void UseProgram(unsigned int whichProgram);
   void StopProgams();

   void LoadShaderProgram(unsigned int whichProgram, const Shader& shader1, const Shader& shader2, bool doesTexturing, bool doesLighting, const std::vector<std::string>& attributes, const std::vector<std::string>& uniforms);

   void SetTextureUniform(const char* whichTex, GLuint textureUnit);
   void SetMatrix4Uniform(const char* whichMatrix, const float* matrixElements);
   void SetMatrix3Uniform(const char* whichMatrix, const float* matrixElements);
   void SetLightUniforms(unsigned int whichLight, const Light& light);

   GLint GetAttributeLocation(const char* whichVar);

   bool CurrentProgramDoesLighting() const;
   bool CurrentProgramDoesTexturing() const;

   void DisableCurrentProgramAttributes();

private:
   std::unordered_map<unsigned int, std::unique_ptr<ShaderProgram>> shaderProgramMap;

   GLInfo::GLSLVersion activeGLSLVersion;

   ShaderProgram* currentProgram;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}