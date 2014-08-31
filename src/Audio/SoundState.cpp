/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Audio/SoundState.h"

#include "Locus/Common/Exception.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <cassert>

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

   MakeOpenALContextCurrent();

   SetListenerOrientation(0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

   SetListenerPosition(0.0f, 0.0f, 0.0f);
}

void SoundState::MakeOpenALContextCurrent()
{
   alcMakeContextCurrent(context);
}

void SoundState::SetListenerOrientation(float xForward, float yForward, float zForward, float xUp, float yUp, float zUp)
{
   float orientation[] = {xForward, yForward, zForward, xUp, yUp, zUp};

   alListenerfv(AL_ORIENTATION, orientation);
}

void SoundState::SetListenerPosition(float x, float y, float z)
{
   float position[] = {x, y, z};

   alListenerfv(AL_POSITION, position);
}

SoundState::~SoundState()
{
   alcDestroyContext(context);

   #ifndef NDEBUG
      ALenum error = alGetError();
      assert(error != AL_NO_ERROR);
   #endif

   alcCloseDevice(device);

   #ifndef NDEBUG
      error = alGetError();
      assert(error != AL_NO_ERROR);
   #endif
}

}