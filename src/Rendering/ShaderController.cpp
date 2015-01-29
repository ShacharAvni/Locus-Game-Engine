/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/ShaderController.h"
#include "Locus/Rendering/ShaderSourceStore.h"
#include "Locus/Rendering/ShaderVariables.h"

#include "Locus/Common/Exception.h"

#include <Locus/Rendering/Locus_glew.h>

#include <algorithm>

namespace Locus
{

ShaderController::ShaderController(const GLInfo& glInfo, GLInfo::GLSLVersion requiredGLSLVersion, bool useHighestSupportedGLSLVersion)
   : nextProgramID(1), activeGLSLVersion(useHighestSupportedGLSLVersion ? glInfo.GetHighestSupportedGLSLVersion() : requiredGLSLVersion), currentProgram(nullptr)
{
}

GLInfo::GLSLVersion ShaderController::GetActiveGLSLVersion() const
{
   return activeGLSLVersion;
}

void ShaderController::UseProgram(ID_t whichProgram)
{
   std::unordered_map<ID_t, std::unique_ptr<ShaderProgram>>::iterator programKeyPairIter = shaderProgramMap.find(whichProgram);

   if (programKeyPairIter != shaderProgramMap.end())
   {
      DisableCurrentProgramAttributes();
      programKeyPairIter->second->Use();
      currentProgram = programKeyPairIter->second.get();
   }
   else
   {
      throw Exception("Requested shader program has not been loaded");
   }
}

void ShaderController::StopProgams()
{
   if (currentProgram != nullptr)
   {
      ShaderProgram::Stop();
      currentProgram = nullptr;
   }
}

ID_t ShaderController::LoadShaderProgram(const Shader& shader1, const Shader& shader2, bool doesTexturing, bool doesLighting)
{
   ID_t thisProgramID = nextProgramID;

   shaderProgramMap[thisProgramID] = std::make_unique<ShaderProgram>(shader1, shader2, doesTexturing, doesLighting);

   ++nextProgramID;

   return thisProgramID;
}

ID_t ShaderController::LoadShaderProgram(GLInfo::GLSLVersion activeGLSLVersion, bool doesTexturing, unsigned int numLights)
{
   ID_t thisProgramID = nextProgramID;

   shaderProgramMap[thisProgramID] = std::make_unique<ShaderProgram>
   (
      Shader(Shader::ShaderType::Vertex, Locus::ShaderSource::Vert(activeGLSLVersion, doesTexturing, numLights)),
      Shader(Shader::ShaderType::Fragment, Locus::ShaderSource::Frag(activeGLSLVersion, doesTexturing, numLights)),
      doesTexturing,
      (numLights > 0)
   );

   ++nextProgramID;

   return thisProgramID;
}

void ShaderController::SetTextureUniform(const std::string& whichTex, GLuint textureUnit)
{
   if (currentProgram != nullptr)
   {
      GLint uniformLocation = currentProgram->GetUniformLocation(whichTex);

      if (uniformLocation != -1)
      {
         glUniform1i(uniformLocation, textureUnit);
      }
   }
}

void ShaderController::SetMatrix4Uniform(const std::string& whichMatrix, const float* matrixElements)
{
   if (currentProgram != nullptr)
   {
      GLint uniformLocation = currentProgram->GetUniformLocation(whichMatrix);

      if (uniformLocation != -1)
      {
         glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, matrixElements);
      }
   }
}

void ShaderController::SetMatrix3Uniform(const std::string& whichMatrix, const float* matrixElements)
{
   if (currentProgram != nullptr)
   {
      GLint uniformLocation = currentProgram->GetUniformLocation(whichMatrix);

      if (uniformLocation != -1)
      {
         glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, matrixElements);
      }
   }
}

void ShaderController::SetGlobalAmbientLightColorUniform(const Color& globalAmbientColor)
{
   if (currentProgram != nullptr)
   {
      GLint globalAmbientLightColorUniformLocation = currentProgram->GetUniformLocation(ShaderSource::Light_GlobalAmbient);

      if (globalAmbientLightColorUniformLocation != -1)
      {
         glUniform4f(globalAmbientLightColorUniformLocation, globalAmbientColor.r / 255.0f, globalAmbientColor.g / 255.0f, globalAmbientColor.b / 255.0f, globalAmbientColor.a / 255.0f);
      }
   }
}

void ShaderController::SetLightUniforms(unsigned int whichLight, const Light& light)
{
   if (currentProgram != nullptr)
   {
      GLint eyePosUniformLocation = currentProgram->GetUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_EyePos, whichLight) );

      if (eyePosUniformLocation != -1)
      {
         glUniform3f(eyePosUniformLocation, light.eyePosition.x, light.eyePosition.y, light.eyePosition.z);
      }

      GLint diffuseLightUniformLocation = currentProgram->GetUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_Diffuse, whichLight) );

      if (diffuseLightUniformLocation != -1)
      {
         glUniform4f(diffuseLightUniformLocation, light.diffuseColor.r / 255.0f, light.diffuseColor.g / 255.0f, light.diffuseColor.b / 255.0f, light.diffuseColor.a / 255.0f);
      }

      GLint ambientLightUniformLocation = currentProgram->GetUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_Ambient, whichLight) );

      if (ambientLightUniformLocation != -1)
      {
         glUniform4f(ambientLightUniformLocation, light.ambientColor.r / 255.0f, light.ambientColor.g / 255.0f, light.ambientColor.b / 255.0f, light.ambientColor.a / 255.0f);
      }

      GLint specularLightUniformLocation = currentProgram->GetUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_Specular, whichLight) );

      if (specularLightUniformLocation != -1)
      {
         glUniform4f(specularLightUniformLocation, light.specularColor.r / 255.0f, light.specularColor.g / 255.0f, light.specularColor.b / 255.0f, light.specularColor.a / 255.0f);
      }

      GLint attenuationUniformLocation = currentProgram->GetUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_Attenuation, whichLight) );

      if (attenuationUniformLocation != -1)
      {
         glUniform1f(attenuationUniformLocation, light.attenuation);
      }

      GLint linearAttenuationUniformLocation = currentProgram->GetUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_LinearAttenuation, whichLight) );

      if (linearAttenuationUniformLocation != -1)
      {
         glUniform1f(linearAttenuationUniformLocation, light.linearAttenuation);
      }

      GLint quadraticAttenuationUniformLocation = currentProgram->GetUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_QuadraticAttenuation, whichLight) );

      if (quadraticAttenuationUniformLocation != -1)
      {
         glUniform1f(quadraticAttenuationUniformLocation, light.quadraticAttenuation);
      }
   }
}

GLint ShaderController::GetAttributeLocation(const std::string& attribute)
{
   if (currentProgram != nullptr)
   {
      return currentProgram->GetAttributeLocation(attribute);
   }
   else
   {
      return -1;
   }
}

GLint ShaderController::GetUniformLocation(const std::string& uniform)
{
   if (currentProgram != nullptr)
   {
      return currentProgram->GetUniformLocation(uniform);
   }
   else
   {
      return -1;
   }
}

GLint ShaderController::EnableAttribute(const std::string& attribute)
{
   if (currentProgram != nullptr)
   {
      return currentProgram->EnableAttribute(attribute);
   }
   else
   {
      return -1;
   }
}

void ShaderController::DisableCurrentProgramAttributes()
{
   if (currentProgram != nullptr)
   {
      currentProgram->DisableProgramAttributes();
   }
}

bool ShaderController::CurrentProgramDoesLighting() const
{
   if (currentProgram != nullptr)
   {
      return currentProgram->doesLighting;
   }
   else
   {
      return false;
   }
}

bool ShaderController::CurrentProgramDoesTexturing() const
{
   if (currentProgram != nullptr)
   {
      return currentProgram->doesTexturing;
   }
   else
   {
      return false;
   }
}

}