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

#include "Locus/Geometry/Vector3.h"

#include "Color.h"

namespace Locus
{

struct Light
{
   Vector3 eyePosition;
   Color diffuseColor;
   float attenuation;
   float linearAttenuation;
   float quadraticAttenuation;
};

}