/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/DefaultGPUVertexData.h"
#include "Locus/Rendering/ShaderController.h"
#include "Locus/Rendering/ShaderVariables.h"

#include <Locus/Rendering/Locus_glew.h>

#include <algorithm>

#include <cstring>

namespace Locus
{

DefaultGPUVertexData::DefaultGPUVertexData()
   : GPUVertexData(sizeof(GPUVertexDataStorage))
{
}

void DefaultGPUVertexData::SetAttributes(ShaderController& shaderController) const
{
   if (transferInfo.sendPositions)
   {
      GLint positionAttribLocation = shaderController.EnableAttribute(ShaderSource::Vert_Pos);

      glVertexAttribPointer(positionAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, position) ));
   }

   if (transferInfo.sendNormals && shaderController.CurrentProgramDoesLighting())
   {
      GLint normalAttribLocation = shaderController.EnableAttribute(ShaderSource::Vert_Normal);

      glVertexAttribPointer(normalAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, normal) ));
   }

   if (transferInfo.sendTexCoords)
   {
      GLint texCoordAttribLocation = shaderController.EnableAttribute(ShaderSource::Vert_Tex);

      glVertexAttribPointer(texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, texCoord) ));
   }

   if (transferInfo.sendColors)
   {
      GLint colorAttribLocation = shaderController.EnableAttribute(ShaderSource::Color);

      glVertexAttribPointer(colorAttribLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GPUVertexDataStorage), reinterpret_cast<GLvoid*>( offsetof(GPUVertexDataStorage, color) ));
   }
}

void DefaultGPUVertexData::SetClientState() const
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

void ZeroFill(GPUVertexDataStorage& gpuVertexDataStorage)
{
   std::memset(&gpuVertexDataStorage, 0, sizeof(gpuVertexDataStorage));
}

}