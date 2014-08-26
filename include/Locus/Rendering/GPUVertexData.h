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

#include "GPUVertexDataStorage.h"
#include "GLCommonTypes.h"

#include <cstddef>

namespace Locus
{

class ShaderController;

class LOCUS_RENDERING_API GPUVertexData
{
public:
   GPUVertexData();
   ~GPUVertexData();

   GPUVertexData(const GPUVertexData&) = delete;
   GPUVertexData& operator=(const GPUVertexData&) = delete;

   void Bind() const;

   void Buffer(std::size_t numVertices, GLenum usage);
   void BufferSub(GLintptr offset, std::size_t numSubVertices, GLvoid* data);

   static void SetClientStateToDefault();

   void SetAttributes(ShaderController& shaderController) const;
   void SetClientState() const;

   void PreDraw(ShaderController& shaderController) const;
   void Draw(ShaderController& shaderController) const;

   GLenum drawMode;

   GPUVertexDataTransferInfo transferInfo;

private:
   GLuint vertexBufferID;

   bool populated;

   std::size_t numVertices;
};

}