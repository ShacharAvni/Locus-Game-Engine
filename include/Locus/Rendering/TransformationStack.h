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

#include "Locus/Math/VectorsFwd.h"

#include "Locus/Geometry/Transformation.h"

#include <stack>

namespace Locus
{

class ShaderController;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API TransformationStack
{
public:
   TransformationStack();

   enum TransformationMode
   {
      ModelView = 0,
      Projection = 1,
      Texture = 2,
      NUM_TRANSFORMATION_MODES
   };

   void Clear();

   void SetTransformationMode(TransformationMode mode);

   void Push(const Transformation& trans);
   void Push();
   void Pop();

   void Load(const Transformation& trans);
   void LoadIdentity();

   void Multiply(const Transformation& trans);

   void Translate(const FVector3& t);
   void Scale(const FVector3& scale);

   const Transformation& TopTransformation(TransformationMode mode) const;
   Transformation& TopTransformation(TransformationMode mode);

   void UploadTransformations(ShaderController& shaderController);
   void UploadTransformations(ShaderController& shaderController, const Transformation& modelTransformation);

private:
   TransformationMode currentMode;

   std::stack<Transformation> transformationStacks[NUM_TRANSFORMATION_MODES];

   void SetLegacyMatrixMode(TransformationMode mode);
   void UploadLegacyMatrix(const Transformation& transformation);

   void UploadMatricesToShader(ShaderController& shaderController, const Transformation* modelTransformation);
   void UploadLegacyMatrices(const Transformation* modelTransformation);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}