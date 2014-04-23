 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

#include "Locus/Rendering/SkyBox.h"
#include "Locus/Rendering/RenderingState.h"
#include "Locus/Rendering/ShaderVariables.h"
#include "Locus/Rendering/Texture.h"

#include "Locus/Geometry/Vector3.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

SkyBox::SkyBox(float radius)
   : frontTexture(nullptr), backTexture(nullptr), leftTexture(nullptr), rightTexture(nullptr), upTexture(nullptr), downTexture(nullptr), radius(radius)
{
   Vector3 v1(-radius, -radius, -radius);
   Vector3 v2(radius, -radius, radius);
   Vector3 v3(-radius, -radius, radius);
   Vector3 v4(radius, -radius, -radius);
   Vector3 v5(-radius, radius, -radius);

   float quadLength = 2 * radius;

   Vector3 up(0.0, 1.0, 0.0);
   Vector3 left(-1.0, 0.0, 0.0);
   Vector3 right(1.0, 0.0, 0.0);
   Vector3 back(0.0, 0.0, 1.0);
   Vector3 forward(0.0, 0.0, -1.0);

   frontQuad.Set(v1, right, up, quadLength, quadLength, Color::White());
   backQuad.Set(v2, left, up, quadLength, quadLength, Color::White());
   leftQuad.Set(v3, forward, up, quadLength, quadLength, Color::White());
   rightQuad.Set(v4, back, up, quadLength, quadLength, Color::White());
   upQuad.Set(v5, right, back, quadLength, quadLength, Color::White());
   downQuad.Set(v3, right, forward, quadLength, quadLength, Color::White());
}

void SkyBox::SetTextures(Texture* frontTexture, Texture* backTexture, Texture* leftTexture, Texture* rightTexture, Texture* upTexture, Texture* downTexture)
{
   this->frontTexture = frontTexture;
   this->backTexture = backTexture;
   this->leftTexture = leftTexture;
   this->rightTexture = rightTexture;
   this->upTexture = upTexture;
   this->downTexture = downTexture;
}

void SkyBox::CreateGPUVertexData()
{
   frontQuad.CreateGPUVertexData();
   backQuad.CreateGPUVertexData();
   leftQuad.CreateGPUVertexData();
   rightQuad.CreateGPUVertexData();
   upQuad.CreateGPUVertexData();
   downQuad.CreateGPUVertexData();
}

void SkyBox::DeleteGPUVertexData()
{
   frontQuad.DeleteGPUVertexData();
   backQuad.DeleteGPUVertexData();
   leftQuad.DeleteGPUVertexData();
   rightQuad.DeleteGPUVertexData();
   upQuad.DeleteGPUVertexData();
   downQuad.DeleteGPUVertexData();
}

void SkyBox::UpdateGPUVertexData()
{
   frontQuad.UpdateGPUVertexData();
   backQuad.UpdateGPUVertexData();
   leftQuad.UpdateGPUVertexData();
   rightQuad.UpdateGPUVertexData();
   upQuad.UpdateGPUVertexData();
   downQuad.UpdateGPUVertexData();
}

void SkyBox::Draw(RenderingState& renderingState) const
{
   renderingState.shaderController.SetTextureUniform(ShaderSource::Map_Diffuse, 0);

   if (frontTexture != nullptr)
   {
      frontTexture->Bind();
   }

   renderingState.transformationStack.UploadTransformations(renderingState.shaderController, frontQuad.CurrentModelTransformation());
   frontQuad.Draw(renderingState);

   if (backTexture != nullptr)
   {
      backTexture->Bind();
   }

   renderingState.transformationStack.UploadTransformations(renderingState.shaderController, backQuad.CurrentModelTransformation());
   backQuad.Draw(renderingState);

   if (leftTexture != nullptr)
   {
      leftTexture->Bind();
   }

   renderingState.transformationStack.UploadTransformations(renderingState.shaderController, leftQuad.CurrentModelTransformation());
   leftQuad.Draw(renderingState);

   if (rightTexture != nullptr)
   {
      rightTexture->Bind();
   }

   renderingState.transformationStack.UploadTransformations(renderingState.shaderController, rightQuad.CurrentModelTransformation());
   rightQuad.Draw(renderingState);

   if (upTexture != nullptr)
   {
      upTexture->Bind();
   }

   renderingState.transformationStack.UploadTransformations(renderingState.shaderController, upQuad.CurrentModelTransformation());
   upQuad.Draw(renderingState);

   if (downTexture != nullptr)
   {
      downTexture->Bind();
   }

   renderingState.transformationStack.UploadTransformations(renderingState.shaderController, downQuad.CurrentModelTransformation());
   downQuad.Draw(renderingState);
}

}