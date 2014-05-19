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

#include <string>

namespace Locus
{

namespace ShaderSource
{

LOCUS_RENDERING_API extern const std::string Map_Diffuse;
LOCUS_RENDERING_API extern const std::string Mat_MVP;
LOCUS_RENDERING_API extern const std::string Mat_MV;
LOCUS_RENDERING_API extern const std::string Mat_Normal;
LOCUS_RENDERING_API extern const std::string Color;
LOCUS_RENDERING_API extern const std::string Vert_Pos;
LOCUS_RENDERING_API extern const std::string Vert_Tex;
LOCUS_RENDERING_API extern const std::string Vert_Normal;

LOCUS_RENDERING_API extern const std::string Light_EyePos;
LOCUS_RENDERING_API extern const std::string Light_Diffuse;
LOCUS_RENDERING_API extern const std::string Light_Ambient;
LOCUS_RENDERING_API extern const std::string Light_Specular;
LOCUS_RENDERING_API extern const std::string Light_Attenuation;
LOCUS_RENDERING_API extern const std::string Light_LinearAttenuation;
LOCUS_RENDERING_API extern const std::string Light_QuadraticAttenuation;
LOCUS_RENDERING_API extern const std::string ToLightDirection;
LOCUS_RENDERING_API extern const std::string ToEyeDirection;
LOCUS_RENDERING_API extern const std::string CalculatedLightAttenuation;

}

}