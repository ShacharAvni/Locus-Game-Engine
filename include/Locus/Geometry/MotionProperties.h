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

#include "Vector3.h"

namespace Locus
{

struct MotionProperties
{
   MotionProperties()
      : speed(0.0f), angularSpeed(0.0f)
   {
   }

   Vector3 direction;
   float speed;
   Vector3 rotation;
   float angularSpeed;
};

}