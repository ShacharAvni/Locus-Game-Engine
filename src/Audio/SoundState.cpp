 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Audio/SoundState.h"

#include "Locus/Common/Exception.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace Locus
{

SoundState::SoundState()
{
   device = alcOpenDevice(nullptr);

   if (device == nullptr)
   {
      throw Exception("Could not open sound device");
   }

   context = alcCreateContext(device, nullptr);

   if (context == nullptr)
   {
      alcCloseDevice(device);
      throw Exception("Could not create sound context");
   }

   alcMakeContextCurrent(context);

   float orientation[] = {1, 0, 0, 0, 1, 0};
   SetListenerOrientation(orientation);

   float position[] = {0.0f, 0.0f, 0.0f};
   SetListenerPosition(position);
}

void SoundState::SetListenerOrientation(float* orientation)
{
   alListenerfv(AL_ORIENTATION, orientation);
}

void SoundState::SetListenerPosition(float* position)
{
   alListenerfv(AL_POSITION, position);
}

SoundState::~SoundState()
{
   alcDestroyContext(context);
   alcCloseDevice(device);
}

}