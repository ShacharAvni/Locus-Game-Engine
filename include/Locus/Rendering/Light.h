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

#include "Locus/Math/Vectors.h"

#include "Color.h"

namespace Locus
{

struct Light
{
   FVector3 eyePosition;
   Color diffuseColor;
   Color ambientColor;
   Color specularColor;
   float attenuation;
   float linearAttenuation;
   float quadraticAttenuation;
};

}