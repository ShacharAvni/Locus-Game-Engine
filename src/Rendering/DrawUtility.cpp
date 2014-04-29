 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Rendering/DrawUtility.h"
#include "Locus/Rendering/RenderingState.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

void DrawUtility::BeginDrawing2D(RenderingState& renderingState, int resolutionX, int resolutionY)
{
   glDisable(GL_DEPTH_TEST);

   renderingState.transformationStack.SetTransformationMode(TransformationStack::Projection);
   renderingState.transformationStack.Push();
   renderingState.transformationStack.Load( Transformation::Orthographic(0, static_cast<float>(resolutionX), static_cast<float>(resolutionY), 0, 0, 1) );

   renderingState.transformationStack.SetTransformationMode(TransformationStack::ModelView);
   renderingState.transformationStack.Push();

   renderingState.UploadTransformations();
}

void DrawUtility::EndDrawing2D(RenderingState& renderingState)
{
   renderingState.transformationStack.SetTransformationMode(TransformationStack::Projection);
   renderingState.transformationStack.Pop();

   renderingState.transformationStack.SetTransformationMode(TransformationStack::ModelView);
   renderingState.transformationStack.Pop();

   glEnable(GL_DEPTH_TEST);
}

}