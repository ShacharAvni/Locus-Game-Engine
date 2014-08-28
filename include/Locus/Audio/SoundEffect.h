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

#include <string>
#include <memory>

namespace Locus
{

class DataStream;
struct MountedFilePath;

struct SoundEffectInternal;

/*!
 * \details SoundEffects become attached to
 * the OpenAL context that is current when they
 * are instantiated. That same OpenAL context should
 * also be current when the SoundEffect is destroyed.
 * The easiest use of Locus SoundEffects is to
 * instantiate a Locus SoundState in main and to NOT
 * use OpenAL through any means besides Locus. This avoids
 * the headache of any OpenAL context management that
 * would be necessary when using SoundEffects or the
 * SoundState.
 * 
 * \example SoundEffects.cpp
 *
 * \sa SoundState
 */
class LOCUS_AUDIO_API SoundEffect
{
public:
   /// \throws Exception
   SoundEffect();
   ~SoundEffect();

   SoundEffect(const SoundEffect&) = delete;
   SoundEffect& operator=(const SoundEffect&) = delete;

   /// Supported audio file formats
   enum class SoundFileType
   {
      WAV,
      OGG
   };

   /*!
    * \brief Load the SoundEffect from a file with the
    * file type deduced from the extension.
    *
    * \throws Exception if the file cannot be opened.
    *
    * \return true if successful, false if there is a problem
    * reading the file.
    *
    * \note Supported extensions are .wav and .ogg
    * (case insensitive).
    */
   bool Load(const std::string& fullFilePath);

   /*!
    * \brief Load the SoundEffect from a file on disk or
    * in an archive with the file type deduced from the
    * extension.
    *
    * \throws Exception if the file cannot be opened.
    *
    * \return true if successful, false if there is a problem
    * reading the file.
    *
    * \note Supported extensions are .wav and .ogg
    * (case insensitive).
    */
   bool Load(const MountedFilePath& mountedFilePath);

   /*!
    * \brief Load the SoundEffect from a file with a known
    * audio file format.
    *
    * \throws Exception if the file cannot be opened.
    *
    * \return true if successful, false if there is a problem
    * reading the file.
    *
    * \note The file extension is ignored.
    */
   bool Load(const std::string& fullFilePath, SoundFileType soundFileType);

   /*!
    * \brief Load the SoundEffect from a file on disk or
    * in an archive with a known audio file format.
    *
    * \throws Exception if the file cannot be opened.
    *
    * \return true if successful, false if there is a problem
    * reading the file.
    *
    * \note The file extension is ignored.
    */
   bool Load(const MountedFilePath& mountedFilePath, SoundFileType soundFileType);

   /// Loads the SoundEffect from a DataStream with a known audio file format.
   bool Load(DataStream& dataStream, SoundFileType soundFileType);

   /// Sets where the sound effect will be emitting from.
   void SetPosition(float x, float y, float z) const;

   /*!
    * \brief Plays the sound effect.
    *
    * \note This call is asynchronous.
    */
   void Play() const;

   /// \return true if this sound effect is currently playing.
   bool IsPlaying() const;

   /// \return true if the last call to Load was successful.
   bool IsLoaded() const;

private:
   std::unique_ptr<SoundEffectInternal> soundEffectInternal;

   void BufferData();

   template <class PathType>
   bool DoLoadFromDeducedExtension(const PathType& loadPath);

   template <class LoadSource>
   bool DoLoad(LoadSource& loadSource, SoundFileType soundFileType);

   static bool DeduceSoundFileTypeFromExtension(const std::string& filePath, SoundFileType& soundFileType);
   static bool DeduceSoundFileTypeFromExtension(const MountedFilePath& mountedFilePath, SoundFileType& soundFileType);

   void Clear();
};

}