/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/GPUVertexData.h"
#include "Locus/Rendering/ShaderController.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

GPUVertexData::GPUVertexData(std::size_t sizeOfSingleElementInBytes)
   : drawMode(GL_NONE), vertexBufferID(0),
     sizeOfSingleElementInBytes(sizeOfSingleElementInBytes),
     populated(false), numVertices(0)
{
   glGenBuffers(1, &vertexBufferID);
}

GPUVertexData::~GPUVertexData()
{
   glDeleteBuffers(1, &vertexBufferID);
}

void GPUVertexData::Bind() const
{
   glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
}

void GPUVertexData::Buffer(std::size_t numVertices, GLenum usage)
{
   populated = true;
   this->numVertices = numVertices;

   glBufferData(GL_ARRAY_BUFFER, numVertices * sizeOfSingleElementInBytes, nullptr, usage);
}

void GPUVertexData::BufferSub(GLintptr offset, std::size_t numSubVertices, GLvoid* data)
{
   glBufferSubData(GL_ARRAY_BUFFER, offset, numSubVertices * sizeOfSingleElementInBytes, data);
}

void GPUVertexData::PreDraw(ShaderController& shaderController) const
{
   if (populated)
   {
      Bind();

      if (shaderController.GetActiveGLSLVersion() == GLInfo::GLSLVersion::V_130)
      {
         SetAttributes(shaderController);
      }
      else
      {
         SetClientState();
      }
   }
}

void GPUVertexData::Draw(ShaderController& shaderController) const
{
   if (populated)
   {
      PreDraw(shaderController);

      glDrawArrays(drawMode, 0, numVertices);

      if (shaderController.GetActiveGLSLVersion() < GLInfo::GLSLVersion::V_130)
      {
         GPUVertexData::SetClientStateToDefault();
      }
   }
}

void GPUVertexData::SetClientStateToDefault()
{
   if (GLEW_VERSION_2_0)
   {
      if (glIsEnabled(GL_NORMAL_ARRAY))
      {
         glDisableClientState(GL_NORMAL_ARRAY);
      }

      if (glIsEnabled(GL_TEXTURE_COORD_ARRAY))
      {
         glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      }
   }
}

}