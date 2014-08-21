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
 * \details This class must be instantiated for using the File
 * class or anything that uses MountedFilePaths. It is suggested
 * to instantiate this in main. It is an error to instantiate this
 * class more than once.
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
    * \throws Exception
    */
   FileSystem(const char* argv0);
   ~FileSystem();
};

/*!
 * \brief Adds a directory or archive to be used for operations involving
 * MountedFilePaths.
 *
 * \code{.cpp}
 * #include "Locus/FileSystem/FileSystem.h"
 * #include "Locus/FileSystem/File.h"
 * #include "Locus/Common/Exception.h"
 *
 * #include <iostream>
 *
 * int main(int argc, char** argv)
 * {
 *    try
 *    {
 *       //Must be done before other Locus file operations
 *       Locus::FileSystem fileSystem(argv[0]);
 *
 *       //Add the directory "path_to/foo". Now MountedFilePaths can be relative to path_to/foo
 *       Locus::MountDirectoryOrArchive("path_to/foo");
 *
 *       //Open the file "bar.txt" which must be in the directory path_to/foo.
 *       Locus::File file1( Locus::MountedFilePath("bar.txt"), Locus::DataStream::OpenOperation::Read );
 *
 *       //Add the archive "path_to/zipFile.zip". Now MountedFilePaths can be relative to path_to/zipFile.zip
 *       Locus::MountDirectoryOrArchive("path_to/zipFile.zip");
 *
 *       //Open the file "baz.jpg" which must be either in the directory path_to/foo or in the archive path_to/zipFile.zip.
 *       Locus::File file2( Locus::MountedFilePath("baz.jpg"), Locus::DataStream::OpenOperation::Read );
 *    }
 *    catch (Locus::Exception& locusException)
 *    {
 *       std::cerr << locusException.Message() << std::endl;
 *       return 1;
 *    }
 *    catch (...)
 *    {
 *       std::cerr << "Something unexpected happened." << std::endl;
 *       return 1;
 *    }
 *
 *    return 0;
 * }
 * \endcode
 */
LOCUS_FILE_SYSTEM_API void MountDirectoryOrArchive(const std::string& fullPath);

}