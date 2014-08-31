/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/DrawUtility.h"
#include "Locus/Rendering/RenderingState.h"

#include "Locus/Geometry/Transformation.h"
#include "Locus/Geometry/Vector3.h"

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

bool DrawUtility::Unproject(
   float windowCoordX,
   float windowCoordY,
   float windowCoordZ,
   const Transformation& modelView,
   const Transformation& projection,
   int resolutionX,
   int resolutionY,
   Vector3& worldCoordinate)
{
   Locus::Matrix<float> modelViewProjectionInverted = projection * modelView;

   if (!modelViewProjectionInverted.Invert())
   {
      return false;
   }

   std::vector<float> windowCoordinate{ ( 2.0f * (windowCoordX / resolutionX) ) - 1.0f,
                                        ( 2.0f * ((resolutionY - windowCoordY) / resolutionY) ) - 1.0f,
                                        ( 2.0f * windowCoordZ ) - 1.0f,
                                        1.0f };

   std::vector<float> objectCoordinate = modelViewProjectionInverted * windowCoordinate;

   if (objectCoordinate[3] == 0.0f)
   {
      return false;
   }

   objectCoordinate[3] = (1.0f / objectCoordinate[3]);

   worldCoordinate.x = (objectCoordinate[0] * objectCoordinate[3]);
   worldCoordinate.y = (objectCoordinate[1] * objectCoordinate[3]);
   worldCoordinate.z = (objectCoordinate[2] * objectCoordinate[3]);

   return true;
}

}