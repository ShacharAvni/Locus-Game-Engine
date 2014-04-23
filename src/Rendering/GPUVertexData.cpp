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

#include "Locus/Rendering/GPUVertexData.h"
#include "Locus/Rendering/GPUVertexDataStorage.h"
#include "Locus/Rendering/ShaderController.h"
#include "Locus/Rendering/ShaderVariables.h"

#include <Locus/Rendering/Locus_glew.h>

#include <vector>
#include <algorithm>

#include <cstring>

namespace Locus
{

GPUVertexData::GPUVertexData()
   : drawMode(GL_NONE), vertexBufferID(0), 
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

   glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GPUVertexDataStorage), nullptr, usage);
}

void GPUVertexData::BufferSub(GLintptr offset, std::size_t numSubVertices, GLvoid* data)
{
   glBufferSubData(GL_ARRAY_BUFFER, offset, numSubVertices * sizeof(GPUVertexDataStorage), data);
}

void GPUVertexData::SetAttributes(ShaderController& shaderController) const
{
   if (transferInfo.sendPositions)
   {
      GLint positionAttribLocation = shaderController.GetAttributeLocation(ShaderSource::Vert_Pos);

      glEnableVertexAttribArray(positionAttribLocation);
      glVertexAttribPointer(positionAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, position) ));
   }

   if (transferInfo.sendNormals && shaderController.CurrentProgramDoesLighting())
   {
      GLint normalAttribLocation = shaderController.GetAttributeLocation(ShaderSource::Vert_Normal);

      glEnableVertexAttribArray(normalAttribLocation);
      glVertexAttribPointer(normalAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, normal) ));
   }

   if (transferInfo.sendTexCoords)
   {
      GLint texCoordAttribLocation = shaderController.GetAttributeLocation(ShaderSource::Vert_Tex);

      glEnableVertexAttribArray(texCoordAttribLocation);
      glVertexAttribPointer(texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, texCoord) ));
   }

   if (transferInfo.sendColors)
   {
      GLint colorAttribLocation = shaderController.GetAttributeLocation(ShaderSource::Color);

      glEnableVertexAttribArray(colorAttribLocation);
      glVertexAttribPointer(colorAttribLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, color) ));
   }
}

void GPUVertexData::SetClientState() const
{
   if (transferInfo.sendPositions)
   {
      glVertexPointer(3, GL_FLOAT, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, position) ));
   }

   if (transferInfo.sendTexCoords)
   {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, texCoord) ));
   }

   if (transferInfo.sendNormals)
   {
      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, normal) ));
   }

   if (transferInfo.sendColors)
   {
      glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, color) ));
   }
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

void ZeroFill(GPUVertexDataStorage& gpuVertexDataStorage)
{
   std::memset(&gpuVertexDataStorage, 0, sizeof(gpuVertexDataStorage));
}

}