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

/*!
 * \details This class initializes PHYSFS on instantiation and
 * deinitializes PHYSFS on destruction. If PHYSFS is used by
 * some other means then instantiating this class is not
 * necessary. PHYSFS must be initialized somehow before using
 * the File class or anything that uses MountedFilePaths. It is
 * suggested to instantiate this class in main.
 *
 * \example MountDirectoryOrArchiveExample.cpp
 */
class LOCUS_FILE_SYSTEM_API FileSystem
{
public:
   /*!
    * \param[in] argv0 argv[0] as in int main(int argc, char** argv).
    * For a graphical application running on Windows, where argv is
    * not passed in to main, you can pass the full path to the executable
    * instead.
    * 
    * \throws Exception if the FileSystem can't be initialized.
    */
   FileSystem(const char* argv0);
   ~FileSystem();
};

/*!
 * \brief Adds a directory or archive to be used for operations involving
 * MountedFilePaths.
 *
 * \example MountDirectoryOrArchiveExample.cpp
 */
LOCUS_FILE_SYSTEM_API void MountDirectoryOrArchive(const std::string& fullPath);

} // namespace Locus