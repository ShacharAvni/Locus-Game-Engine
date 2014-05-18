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

#ifndef GLEW_MX
   #define GLEW_MX
#endif

#include <GL/glew.h>

LOCUS_RENDERING_API GLEWContext* glewGetContext();

namespace Locus
{

LOCUS_RENDERING_API void ChangeCurrentContext(GLEWContext* newGLEWContext);
LOCUS_RENDERING_API void ContextDestroyed(GLEWContext* glewContextToBeDestroyed);

}