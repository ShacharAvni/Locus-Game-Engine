/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/FileSystem/FileSystem.h"

#include "Locus/Common/Exception.h"

#include "physfs.h"

#include <cassert>

namespace Locus
{

FileSystem::FileSystem(const char* argv0)
{
   if (!PHYSFS_isInit())
   {
      if (PHYSFS_init(argv0) == 0)
      {
         throw Exception("Failed to initialize PHYSFS");
      }
   }
}

FileSystem::~FileSystem()
{
   if (PHYSFS_isInit())
   {
      #ifndef NDEBUG
      int physfsDeinitialized = 
      #endif
      PHYSFS_deinit();

      assert(physfsDeinitialized != 0);
   }
}

void MountDirectoryOrArchive(const std::string& fullPath)
{
   PHYSFS_mount(fullPath.c_str(), NULL, 1);
}

}