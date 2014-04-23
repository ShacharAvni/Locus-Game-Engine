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

#include "Locus/Rendering/ShaderVariables.h"

namespace Locus
{

namespace ShaderSource
{

LOCUS_RENDERING_API_AT_DEFINITION const char* const Map_Diffuse = "Map_Diffuse";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Mat_MVP = "Mat_MVP";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Mat_MV = "Mat_MV";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Mat_Normal = "Mat_Normal";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Color = "Color";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Vert_Pos = "Vert_Pos";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Vert_Tex = "Vert_Tex";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Vert_Normal = "Vert_Normal";

LOCUS_RENDERING_API_AT_DEFINITION const char* const Light_EyePos = "Light_EyePos";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Light_Diffuse = "Light_Diffuse";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Light_Ambient = "Light_Ambient";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Light_Specular = "Light_Specular";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Light_Attenuation = "Light_Attenuation";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Light_LinearAttenuation = "Light_LinearAttenuation";
LOCUS_RENDERING_API_AT_DEFINITION const char* const Light_QuadraticAttenuation = "Light_QuadraticAttenuation";
LOCUS_RENDERING_API_AT_DEFINITION const char* const ToLightDirection = "ToLightDirection";
LOCUS_RENDERING_API_AT_DEFINITION const char* const ToEyeDirection = "ToEyeDirection";
LOCUS_RENDERING_API_AT_DEFINITION const char* const CalculatedLightAttenuation = "CalculatedLightAttenuation";

}

}
