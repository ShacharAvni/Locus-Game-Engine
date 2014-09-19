/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "LocusFileSystemAPI.h"

#include <string>
#include <vector>

namespace Locus
{

struct MountedFilePath;

/// \return The full path to the current running executable.
LOCUS_FILE_SYSTEM_API std::string GetExePath();

/*!
 * \brief Reads the whole contents of the file in binary format.
 *
 * \param[in] filePath The full path of the file.
 *
 * \param[in,out] data The data read from the file. This vector
 * will be sized appropriately by this function.
 *
 * \throws Exception
 */
LOCUS_FILE_SYSTEM_API void ReadWholeFile(const std::string& filePath, std::vector<char>& data);

/*!
 * \brief Reads the whole contents of the file in binary format.
 *
 * \param[in] mountedFilePath Path to the file in an archive or
 * on disk, relative to a path passed to MountDirectoryOrArchive.
 *
 * \param[in,out] data The data read from the file. This vector
 * will be sized appropriately by this function.
 *
 * \throws Exception
 *
 * \sa MountDirectoryOrArchive
 */
LOCUS_FILE_SYSTEM_API void ReadWholeFile(const MountedFilePath& mountedFilePath, std::vector<char>& data);

} // namespace Locus