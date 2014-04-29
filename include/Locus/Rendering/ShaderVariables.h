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

namespace Locus
{

namespace ShaderSource
{

   LOCUS_RENDERING_API extern const char* const Map_Diffuse;
   LOCUS_RENDERING_API extern const char* const Mat_MVP;
   LOCUS_RENDERING_API extern const char* const Mat_MV;
   LOCUS_RENDERING_API extern const char* const Mat_Normal;
   LOCUS_RENDERING_API extern const char* const Color;
   LOCUS_RENDERING_API extern const char* const Vert_Pos;
   LOCUS_RENDERING_API extern const char* const Vert_Tex;
   LOCUS_RENDERING_API extern const char* const Vert_Normal;

   LOCUS_RENDERING_API extern const char* const Light_EyePos;
   LOCUS_RENDERING_API extern const char* const Light_Diffuse;
   LOCUS_RENDERING_API extern const char* const Light_Ambient;
   LOCUS_RENDERING_API extern const char* const Light_Specular;
   LOCUS_RENDERING_API extern const char* const Light_Attenuation;
   LOCUS_RENDERING_API extern const char* const Light_LinearAttenuation;
   LOCUS_RENDERING_API extern const char* const Light_QuadraticAttenuation;
   LOCUS_RENDERING_API extern const char* const ToLightDirection;
   LOCUS_RENDERING_API extern const char* const ToEyeDirection;
   LOCUS_RENDERING_API extern const char* const CalculatedLightAttenuation;

}

}