 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/FileSystem/MountedFilePath.h"

namespace Locus
{

MountedFilePath::MountedFilePath(const char* path)
   : path(path)
{
}

MountedFilePath::MountedFilePath(const std::string& path)
   : path(path)
{
}

MountedFilePath operator +(const MountedFilePath& first, const MountedFilePath& second)
{
   return MountedFilePath(first.path + second.path);
}

MountedFilePath operator +(const MountedFilePath& first, const std::string& second)
{
   return MountedFilePath(first.path + second);
}

}