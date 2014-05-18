/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include <Locus/Rendering/Locus_glew.h>

static GLEWContext* currentGLEWContext = nullptr;

GLEWContext* glewGetContext()
{
   return currentGLEWContext;
}

namespace Locus
{

void ChangeCurrentContext(GLEWContext* newGLEWContext)
{
   currentGLEWContext = newGLEWContext;
}

void ContextDestroyed(GLEWContext* glewContextToBeDestroyed)
{
   if (glewGetContext() == glewContextToBeDestroyed)
   {
      ChangeCurrentContext(nullptr);
   }
}

}