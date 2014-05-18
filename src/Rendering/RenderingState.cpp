/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/RenderingState.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

RenderingState::RenderingState(GLInfo::GLSLVersion requiredGLSLVersion, bool useHighestSupportedGLSLVersion)
   : glInfo(requiredGLSLVersion), shaderController(glInfo, requiredGLSLVersion, useHighestSupportedGLSLVersion)
{
}

void RenderingState::SetOpenGLStateToDefault()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);

   glEnable(GL_CULL_FACE); 
   glEnable(GL_DEPTH_TEST); 

   glLineWidth(1.0f);
   glPointSize(1.0f);

   glDrawBuffer(GL_BACK);
   glReadBuffer(GL_BACK);
   glDepthMask(GL_TRUE);
   glDepthFunc(GL_LEQUAL);

   glFrontFace(GL_CCW); 
   glCullFace(GL_BACK);

   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   if ((GLEW_VERSION_2_0) && (shaderController.GetActiveGLSLVersion() < GLInfo::GLSLVersion::V_130))
   {
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_COLOR_ARRAY);
   }
}

void RenderingState::UploadTransformations()
{
   transformationStack.UploadTransformations(shaderController);
}

}