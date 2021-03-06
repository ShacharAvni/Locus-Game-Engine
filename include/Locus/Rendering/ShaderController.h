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

#include "Locus/Common/IDType.h"

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

   void UseProgram(ID_t whichProgram);
   void StopProgams();

   ID_t LoadShaderProgram(const Shader& shader1, const Shader& shader2, bool doesTexturing, bool doesLighting);
   ID_t LoadShaderProgram(GLInfo::GLSLVersion activeGLSLVersion, bool doesTexturing, unsigned int numLights);

   void SetTextureUniform(const std::string& whichTex, GLuint textureUnit);
   void SetMatrix4Uniform(const std::string& whichMatrix, const float* matrixElements);
   void SetMatrix3Uniform(const std::string& whichMatrix, const float* matrixElements);
   void SetGlobalAmbientLightColorUniform(const Color& globalAmbientColor);
   void SetLightUniforms(unsigned int whichLight, const Light& light);

   GLint GetAttributeLocation(const std::string& attribute);
   GLint GetUniformLocation(const std::string& uniform);

   GLint EnableAttribute(const std::string& attribute);

   bool CurrentProgramDoesLighting() const;
   bool CurrentProgramDoesTexturing() const;

private:
   std::unordered_map<ID_t, std::unique_ptr<ShaderProgram>> shaderProgramMap;

   ID_t nextProgramID;

   GLInfo::GLSLVersion activeGLSLVersion;

   ShaderProgram* currentProgram;

   void DisableCurrentProgramAttributes();
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}