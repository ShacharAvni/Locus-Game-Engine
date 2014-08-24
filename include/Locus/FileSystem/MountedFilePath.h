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

#include "LocusFileSystemAPI.h"

#include <string>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

/*!
 * \brief A path to a file, relative to a directory or
 * archive whose path was passed to the function
 * MountDirectoryOrArchive.
 *
 * \details Using MountedFilePaths requires a FileSystem
 * instance to have been instantiated.
 *
 * \sa FileSystem MountDirectoryOrArchive
 */
struct LOCUS_FILE_SYSTEM_API MountedFilePath
{
   explicit MountedFilePath(const char* path);
   explicit MountedFilePath(const std::string& path);

   std::string path;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

/// \return second appended to first.
LOCUS_FILE_SYSTEM_API MountedFilePath operator +(const MountedFilePath& first, const MountedFilePath& second);

/// \return second appended to first.
LOCUS_FILE_SYSTEM_API MountedFilePath operator +(const MountedFilePath& first, const std::string& second);

} // namespace Locus