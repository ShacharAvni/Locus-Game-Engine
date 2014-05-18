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

#include "Drawable.h"
#include "GLCommonTypes.h"
#include "Mesh.h"
#include "Quad.h"

namespace Locus
{

class Texture;

class LOCUS_RENDERING_API SkyBox : public Drawable
{
public:
   SkyBox(float radius);

   void SetTextures(Texture* frontTexture, Texture* backTexture, Texture* leftTexture, Texture* rightTexture, Texture* upTexture, Texture* downTexture);

   virtual void CreateGPUVertexData() override;
   virtual void DeleteGPUVertexData() override;
   virtual void UpdateGPUVertexData() override;

   virtual void Draw(RenderingState& renderingState) const override;

private:
   Quad frontQuad;
   Quad backQuad;
   Quad leftQuad;
   Quad rightQuad;
   Quad upQuad;
   Quad downQuad;

   Texture* frontTexture;
   Texture* backTexture;
   Texture* leftTexture;
   Texture* rightTexture;
   Texture* upTexture;
   Texture* downTexture;

   float radius;
};

}