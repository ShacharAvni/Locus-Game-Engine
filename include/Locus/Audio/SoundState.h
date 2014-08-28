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
 * \details This class wraps an OpenAL context. Locus SoundEffects
 * should be instantiated and destroyed while the OpenAL context
 * of this class is current. Use of OpenAL through other means
 * in tandem with Locus requires great care as using the SoundState
 * class can interfere with other OpenAL contexts and vice versa.
 * 
 * \example SoundEffects.cpp
 *
 * \sa SoundEffect
 */
class LOCUS_AUDIO_API SoundState
{
public:
   /*!
    * \details The OpenAL context wrapped by this class
    * will be current after instantiation.
    *
    * \throws Exception
    */
   SoundState();

   ~SoundState();

   /*!
    * \brief Makes the OpenAL context wrapped by this class
    * current.
    *
    * \details The use of other OpenAL contexts can interfere
    * with the use of this class or with Locus SoundEffects.
    * If OpenAL is not used besides through Locus then this
    * method can be ignored. This method should be called if
    * the OpenAL context has been changed before using this
    * instance or any Locus SoundEffects that were instantiated
    * while the OpenAL context of this instance was current.
    *
    * \example SoundEffects.cpp
    */
   void MakeOpenALContextCurrent();

   /*!
    * \brief Sets the orientation of the listener.
    *
    * \details The orientation is described as two vectors, the
    * "forward" vector which can be thought of as where the
    * listener is looking, and the "up" pointing upwards through
    * the listener's head. These vectors need not be normalized.
    * Undefined behaviour results if either of them are zero or if
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