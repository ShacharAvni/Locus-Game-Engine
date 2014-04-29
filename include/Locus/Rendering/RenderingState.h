 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusRenderingAPI.h"

#include "TransformationStack.h"
#include "ShaderController.h"
#include "GLInfo.h"

#include <vector>

namespace Locus
{

class LOCUS_RENDERING_API RenderingState
{
public:
   RenderingState(GLInfo::GLSLVersion requiredGLSLVersion, bool useHighestSupportedGLSLVersion);
   RenderingState(const RenderingState&) = delete;
   RenderingState& operator=(const RenderingState&) = delete;

   void SetOpenGLStateToDefault();

   void UploadTransformations();

   GLInfo glInfo;
   TransformationStack transformationStack;
   ShaderController shaderController;
};

}