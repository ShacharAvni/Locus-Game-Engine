 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Rendering/ShaderController.h"
#include "Locus/Rendering/ShaderSourceStore.h"
#include "Locus/Rendering/ShaderVariables.h"

#include "Locus/Common/Exception.h"

#include <Locus/Rendering/Locus_glew.h>

#include <algorithm>

namespace Locus
{

ShaderController::ShaderController(const GLInfo& glInfo, GLInfo::GLSLVersion requiredGLSLVersion, bool useHighestSupportedGLSLVersion)
   : activeGLSLVersion(useHighestSupportedGLSLVersion ? glInfo.GetHighestSupportedGLSLVersion() : requiredGLSLVersion), currentProgram(nullptr)
{
}

GLInfo::GLSLVersion ShaderController::GetActiveGLSLVersion() const
{
   return activeGLSLVersion;
}

void ShaderController::UseProgram(unsigned int whichProgram)
{
   std::unordered_map<unsigned int, std::unique_ptr<ShaderProgram>>::iterator programKeyPairIter = shaderProgramMap.find(whichProgram);

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

void ShaderController::LoadShaderProgram(unsigned int whichProgram, const Shader& shader1, const Shader& shader2, bool doesTexturing, bool doesLighting, const std::vector<std::string>& attributes, const std::vector<std::string>& uniforms)
{
   shaderProgramMap[whichProgram].reset( new ShaderProgram(shader1, shader2, doesTexturing, doesLighting, attributes, uniforms) );
}

void ShaderController::SetTextureUniform(const char* whichTex, GLuint textureUnit)
{
   if (currentProgram != nullptr)
   {
      GLint uniformLocation = currentProgram->GetAttributeOrUniformLocation(whichTex);

      if (uniformLocation != -1)
      {
         glUniform1i(uniformLocation, textureUnit);
      }
   }
}

void ShaderController::SetMatrix4Uniform(const char* whichMatrix, const float* matrixElements)
{
   if (currentProgram != nullptr)
   {
      GLint uniformLocation = currentProgram->GetAttributeOrUniformLocation(whichMatrix);

      if (uniformLocation != -1)
      {
         glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, matrixElements);
      }
   }
}

void ShaderController::SetMatrix3Uniform(const char* whichMatrix, const float* matrixElements)
{
   if (currentProgram != nullptr)
   {
      GLint uniformLocation = currentProgram->GetAttributeOrUniformLocation(whichMatrix);

      if (uniformLocation != -1)
      {
         glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, matrixElements);
      }
   }
}

void ShaderController::SetLightUniforms(unsigned int whichLight, const Light& light)
{
   if (currentProgram != nullptr)
   {
      GLint eyePosUniformLocation = currentProgram->GetAttributeOrUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_EyePos, whichLight).c_str() );

      if (eyePosUniformLocation != -1)
      {
         glUniform3f(eyePosUniformLocation, light.eyePosition.x, light.eyePosition.y, light.eyePosition.z);
      }

      GLint diffuseLightUniformLocation = currentProgram->GetAttributeOrUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_Diffuse, whichLight).c_str() );

      if (diffuseLightUniformLocation != -1)
      {
         glUniform4f(diffuseLightUniformLocation, light.diffuseColor.r / 255.0f, light.diffuseColor.g / 255.0f, light.diffuseColor.b / 255.0f, light.diffuseColor.a / 255.0f);
      }

      GLint attenuationUniformLocation = currentProgram->GetAttributeOrUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_Attenuation, whichLight).c_str() );

      if (attenuationUniformLocation != -1)
      {
         glUniform1f(attenuationUniformLocation, light.attenuation);
      }

      GLint linearAttenuationUniformLocation = currentProgram->GetAttributeOrUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_LinearAttenuation, whichLight).c_str() );

      if (linearAttenuationUniformLocation != -1)
      {
         glUniform1f(linearAttenuationUniformLocation, light.linearAttenuation);
      }

      GLint quadraticAttenuationUniformLocation = currentProgram->GetAttributeOrUniformLocation( ShaderSource::GetMultiVariableName(ShaderSource::Light_QuadraticAttenuation, whichLight).c_str() );

      if (quadraticAttenuationUniformLocation != -1)
      {
         glUniform1f(quadraticAttenuationUniformLocation, light.quadraticAttenuation);
      }
   }
}

GLint ShaderController::GetAttributeLocation(const char* whichVar)
{
   if (currentProgram != nullptr)
   {
      return currentProgram->GetAttributeOrUniformLocation(whichVar);
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
      glDisableVertexAttribArray( currentProgram->GetAttributeOrUniformLocation(ShaderSource::Vert_Pos) );

      if (currentProgram->doesLighting)
      {
         glDisableVertexAttribArray( currentProgram->GetAttributeOrUniformLocation(ShaderSource::Vert_Normal) );
      }

      if (currentProgram->doesTexturing)
      {
         glDisableVertexAttribArray( currentProgram->GetAttributeOrUniformLocation(ShaderSource::Vert_Tex) );
      }

      glDisableVertexAttribArray( currentProgram->GetAttributeOrUniformLocation(ShaderSource::Color) );
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