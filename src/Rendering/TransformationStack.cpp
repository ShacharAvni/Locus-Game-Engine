/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/TransformationStack.h"
#include "Locus/Rendering/ShaderController.h"
#include "Locus/Rendering/ShaderVariables.h"

#include "Locus/Geometry/Vector3.h"

#include "Locus/Common/Util.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

TransformationStack::TransformationStack()
   : currentMode(ModelView)
{
   Clear();
}

void TransformationStack::Clear()
{
   Util::ClearStack(transformationStacks[ModelView]);
   Util::ClearStack(transformationStacks[Projection]);
   Util::ClearStack(transformationStacks[Texture]);

   transformationStacks[ModelView].push( Transformation::Identity() );
   transformationStacks[Projection].push( Transformation::Identity() );
   transformationStacks[Texture].push( Transformation::Identity() );
}

void TransformationStack::SetTransformationMode(TransformationMode mode)
{
   currentMode = mode;
}

void TransformationStack::Push()
{
   transformationStacks[currentMode].push(transformationStacks[currentMode].top());
}

void TransformationStack::Push(const Transformation& trans)
{
   transformationStacks[currentMode].push(trans);
}

void TransformationStack::Pop()
{
   if (transformationStacks[currentMode].size() > 0)
   {
      transformationStacks[currentMode].pop();
   }
}

void TransformationStack::Load(const Transformation& trans)
{
   TopTransformation(currentMode) = trans;
}

void TransformationStack::LoadIdentity()
{
   Load( Transformation::Identity() );
}

void TransformationStack::Translate(const Vector3& t)
{
   TopTransformation(currentMode).TranslateBy(t);
}

void TransformationStack::Scale(const Vector3& scale)
{
   TopTransformation(currentMode).ScaleBy(scale);
}

void TransformationStack::SetLegacyMatrixMode(TransformationMode mode)
{
   switch(mode)
   {
      case ModelView:
         glMatrixMode(GL_MODELVIEW);
         break;

      case Projection:
         glMatrixMode(GL_PROJECTION);
         break;

      case Texture:
         glMatrixMode(GL_TEXTURE);
         break;

      default:
         break;
   }
}

const Transformation& TransformationStack::TopTransformation(TransformationMode mode) const
{
   return transformationStacks[mode].top();
}

Transformation& TransformationStack::TopTransformation(TransformationMode mode)
{
   return transformationStacks[mode].top();
}

void TransformationStack::UploadLegacyMatrix(const Transformation& transformation)
{
   glLoadMatrixf( transformation.Elements() );
}

void TransformationStack::UploadLegacyMatrices(const Transformation* modelTransformation)
{
   SetLegacyMatrixMode(ModelView);

   if (modelTransformation != nullptr)
   {
      Transformation modelViewMatrix = transformationStacks[ModelView].top();
      modelViewMatrix.MultMatrix(*modelTransformation);
      UploadLegacyMatrix(modelViewMatrix);
   }
   else
   {
      UploadLegacyMatrix(transformationStacks[ModelView].top());
   }

   SetLegacyMatrixMode(Projection);
   UploadLegacyMatrix(transformationStacks[Projection].top());
}

void TransformationStack::UploadMatricesToShader(ShaderController& shaderController, const Transformation* modelTransformation)
{
   Transformation modelViewProjectionMatrix = TopTransformation(Projection);
   modelViewProjectionMatrix.MultMatrix(TopTransformation(ModelView));

   if (modelTransformation != nullptr)
   {
      modelViewProjectionMatrix.MultMatrix(*modelTransformation);
   }

   shaderController.SetMatrix4Uniform(ShaderSource::Mat_MVP, modelViewProjectionMatrix.Elements());

   if (shaderController.CurrentProgramDoesLighting())
   {
      Transformation modelViewMatrix = TopTransformation(ModelView);

      if (modelTransformation != nullptr)
      {
         modelViewMatrix.MultMatrix(*modelTransformation);
      }

      shaderController.SetMatrix4Uniform(ShaderSource::Mat_MV, modelViewMatrix.Elements());

      //for now, assume that only rotations, translations, and homogeneous scales have been done.
      //Therefore, the normal matrix would be the same as the top left sub matrix of the model view matrix
      //(Otherwise, we would have to use the transpose of the inverse the top left sub matrix)
      shaderController.SetMatrix3Uniform(ShaderSource::Mat_Normal, modelViewMatrix.SubMatrix(3, 3).Elements());
   }
}

void TransformationStack::UploadTransformations(ShaderController& shaderController)
{
   switch(shaderController.GetActiveGLSLVersion())
   {
      case GLInfo::GLSLVersion::V_130:
         UploadMatricesToShader(shaderController, nullptr);
         break;

      default:
         UploadLegacyMatrices(nullptr);
         break;
   }
}

void TransformationStack::UploadTransformations(ShaderController& shaderController, const Transformation& modelTransformation)
{
   switch(shaderController.GetActiveGLSLVersion())
   {
      case GLInfo::GLSLVersion::V_130:
         UploadMatricesToShader(shaderController, &modelTransformation);
         break;

      default:
         UploadLegacyMatrices(&modelTransformation);
         break;
   }
}

}