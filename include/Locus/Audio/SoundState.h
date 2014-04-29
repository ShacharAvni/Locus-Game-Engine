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

#include "LocusAudioAPI.h"

extern "C"
{

struct ALCdevice_struct;
struct ALCcontext_struct;

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

}

namespace Locus
{

class LOCUS_AUDIO_API SoundState
{
public:
   SoundState();
   ~SoundState();

   void SetListenerOrientation(float* orientation);
   void SetListenerPosition(float* position);

private:
   ALCdevice* device;
   ALCcontext* context;
};

}