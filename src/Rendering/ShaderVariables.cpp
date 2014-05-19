/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/ShaderVariables.h"

namespace Locus
{

namespace ShaderSource
{

LOCUS_RENDERING_API_AT_DEFINITION const std::string Map_Diffuse = "Map_Diffuse";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Mat_MVP = "Mat_MVP";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Mat_MV = "Mat_MV";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Mat_Normal = "Mat_Normal";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Color = "Color";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Vert_Pos = "Vert_Pos";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Vert_Tex = "Vert_Tex";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Vert_Normal = "Vert_Normal";

LOCUS_RENDERING_API_AT_DEFINITION const std::string Light_EyePos = "Light_EyePos";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Light_Diffuse = "Light_Diffuse";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Light_Ambient = "Light_Ambient";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Light_Specular = "Light_Specular";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Light_Attenuation = "Light_Attenuation";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Light_LinearAttenuation = "Light_LinearAttenuation";
LOCUS_RENDERING_API_AT_DEFINITION const std::string Light_QuadraticAttenuation = "Light_QuadraticAttenuation";
LOCUS_RENDERING_API_AT_DEFINITION const std::string ToLightDirection = "ToLightDirection";
LOCUS_RENDERING_API_AT_DEFINITION const std::string ToEyeDirection = "ToEyeDirection";
LOCUS_RENDERING_API_AT_DEFINITION const std::string CalculatedLightAttenuation = "CalculatedLightAttenuation";

}

}