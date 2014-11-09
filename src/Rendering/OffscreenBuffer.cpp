/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Common/Exception.h"

#include "Locus/Rendering/OffscreenBuffer.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

OffscreenBuffer::OffscreenBuffer(int maxWidth, int maxHeight)
{
   glGenFramebuffers(1, &fboID);
   glGenRenderbuffers(1, &renderBufferID);
   glGenRenderbuffers(1, &depthBufferID);
   glGenBuffers(1, &pixelPackBufferID);

   glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelPackBufferID);
   glBufferData(GL_PIXEL_PACK_BUFFER, 4, 0, GL_STREAM_READ);
   glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

   Bind();

   glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, maxWidth, maxHeight);
   glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBufferID);

   glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, maxWidth, maxHeight);
   glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

   glBindRenderbuffer(GL_RENDERBUFFER, 0);

   GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

   OffscreenBuffer::Unbind();

   if (status != GL_FRAMEBUFFER_COMPLETE)
   {
      DestroyBuffers();

      throw Exception("Framebuffer incomplete");
   }
}

OffscreenBuffer::~OffscreenBuffer()
{
   DestroyBuffers();
}

void OffscreenBuffer::DestroyBuffers()
{
   glDeleteFramebuffers(1, &fboID);
   glDeleteRenderbuffers(1, &renderBufferID);
   glDeleteRenderbuffers(1, &depthBufferID);
   glDeleteBuffers(1, &pixelPackBufferID);
}

void OffscreenBuffer::Bind() const
{
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);
}

void OffscreenBuffer::Unbind()
{
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

Color OffscreenBuffer::ReadPixel(int x, int y)
{
   glBindFramebuffer(GL_READ_FRAMEBUFFER, fboID);

   glReadBuffer(GL_COLOR_ATTACHMENT0);

   glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelPackBufferID);
   glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

   void* bufferMemory = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

   char* data = static_cast<char*>(bufferMemory);

   Color color;
   color.r = data[0];
   color.g = data[1];
   color.b = data[2];
   color.a = data[3];

   glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

   glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

   glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

   return color;
}

}