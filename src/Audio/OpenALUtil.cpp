/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "OpenALUtil.h"

namespace Locus
{

std::string OpenALErrorToString(ALenum openALError)
{
   switch (openALError)
   {
   case AL_NO_ERROR:
   default:
      return "AL_NO_ERROR";

   case AL_INVALID_NAME:
      return "AL_INVALID_NAME";

   case AL_INVALID_ENUM:
      return "AL_INVALID_ENUM";

   case AL_INVALID_VALUE:
      return "AL_INVALID_VALUE";

   case AL_INVALID_OPERATION:
      return "AL_INVALID_OPERATION";

   case AL_OUT_OF_MEMORY:
      return "AL_OUT_OF_MEMORY";
   }
}

}