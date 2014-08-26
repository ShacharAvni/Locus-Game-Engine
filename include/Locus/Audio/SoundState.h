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

/*!
 * \details This class must be instantiated before using Locus
 * SoundEffects. It is suggested to instantiate this in main.
 * The instance of this class should outlive any Locus SoundEffect
 * instances.
 */
class LOCUS_AUDIO_API SoundState
{
public:
   /// \throws Exception
   SoundState();

   ~SoundState();

   /*!
    * \brief Sets the orientation of the listener.
    *
    * \details The orientation is described as two vectors, the
    * "forward" vector which can be thought of as where the
    * listener is looking, and the "up" pointing upwards through
    * the listener's head. These vectors need not be normalized.
    * Undefined behaviou results if either of them are zero or if
    * they are linearly dependent.
    */
   void SetListenerOrientation(float xForward, float yForward, float zForward, float xUp, float yUp, float zUp);

   /// Sets where the listener is.
   void SetListenerPosition(float x, float y, float z);

private:
   ALCdevice* device;
   ALCcontext* context;
};

}