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
#include "GLCommonTypes.h"

#include <cstddef>

namespace Locus
{

class ShaderController;

class LOCUS_RENDERING_API GPUVertexData
{
public:
   GPUVertexData(std::size_t sizeOfSingleElementInBytes);
   virtual ~GPUVertexData();

   GPUVertexData(const GPUVertexData&) = delete;
   GPUVertexData& operator=(const GPUVertexData&) = delete;

   void Bind() const;

   void Buffer(std::size_t numVertices, GLenum usage);
   void BufferSub(GLintptr offset, std::size_t numSubVertices, GLvoid* data);

   static void SetClientStateToDefault();

   virtual void SetAttributes(ShaderController& shaderController) const = 0;
   virtual void SetClientState() const = 0;

   void PreDraw(ShaderController& shaderController) const;
   void Draw(ShaderController& shaderController) const;

   GLenum drawMode;

private:
   GLuint vertexBufferID;

   std::size_t sizeOfSingleElementInBytes;

   bool populated;

   std::size_t numVertices;
};

}