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

#include "ShaderProgram.h"
#include "GLInfo.h"
#include "Light.h"

#include <unordered_map>
#include <string>
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

   void LoadShaderProgram(unsigned int whichProgram, const Shader& shader1, const Shader& shader2, bool doesTexturing, bool doesLighting);
   void LoadShaderProgram(unsigned int whichProgram, GLInfo::GLSLVersion activeGLSLVersion, bool doesTexturing, unsigned int numLights);

   void SetTextureUniform(const std::string& whichTex, GLuint textureUnit);
   void SetMatrix4Uniform(const std::string& whichMatrix, const float* matrixElements);
   void SetMatrix3Uniform(const std::string& whichMatrix, const float* matrixElements);
   void SetLightUniforms(unsigned int whichLight, const Light& light);

   GLint GetAttributeLocation(const std::string& attribute);
   GLint GetUniformLocation(const std::string& uniform);

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