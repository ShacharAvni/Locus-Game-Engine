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
#include "Image.h"

#include <vector>

namespace Locus
{

class LOCUS_RENDERING_API TextureArray
{
public:
   TextureArray(const std::vector<Image>& images, bool minMagFilterLinear, bool clamp);
   ~TextureArray();

   TextureArray(const TextureArray&) = delete;
   TextureArray& operator=(const TextureArray&) = delete;

   void Bind() const;

private:
   GLuint id;

   static bool ImagesAreWellFormed(const std::vector<Image>& images);
};

}