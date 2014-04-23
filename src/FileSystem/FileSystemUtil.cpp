 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

#include "Locus/FileSystem/FileSystemUtil.h"

#include "Locus/FileSystem/FileOnDisk.h"
#include "Locus/FileSystem/File.h"

#include "Locus/Common/Exception.h"

#if defined(LOCUS_WINDOWS)
   #define NOMINMAX
   #include <windows.h>
#elif defined(LOCUS_OSX)
   #include <stdlib.h>
#else
   #include <unistd.h>
#endif

#include <mutex>

namespace Locus
{

static std::string fullExePath;
static std::mutex getExePathMutex;

static std::string StripExeFromFullPath(const char* fullExePathIncludingExecutable)
{
   std::string exePath(fullExePathIncludingExecutable);

   std::size_t indexOfLastSlash = 
#ifdef LOCUS_WINDOWS
      exePath.rfind('\\');
#else
      exePath.rfind('/');
#endif

   return exePath.substr(0, indexOfLastSlash + 1);
}

#if defined(LOCUS_WINDOWS)
static std::string GetExePath_Internal()
{
   char buffer[MAX_PATH + 1];
   if (GetModuleFileName(NULL, buffer, sizeof(buffer)) != 0)
   {
      return StripExeFromFullPath(buffer);
   }
   else
   {
      return "";
   }
}

#elif defined(LOCUS_OSX)
static std::string GetExePath_Internal()
{
   std::vector<char> buffer(1024);
   uint32_t size = buffer.size();

   if (_NSGetExecutablePath(buffer.data(), &size) == 0)
   {
         return StripExeFromFullPath( realpath(buffer.data()) );
   }
   else
   {
      buffer.resize(size);
      if (_NSGetExecutablePath(buffer.data(), &size) == 0)
      {
            return StripExeFromFullPath( realpath(buffer.data()) );
      }
      else
      {
         return "";
      }
   }
}

#else
static std::string GetExePath_Internal()
{
   char buffer[1024];
   ssize_t len = ::readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

   if (len != -1)
   {
      buffer[len] = '\0';
      return StripExeFromFullPath(buffer);
   }
   else
   {
      return "";
   }
}

#endif

std::string GetExePath()
{
   std::lock_guard<std::mutex> getExePathLockGuard(getExePathMutex);

   if (fullExePath.length() == 0)
   {
      fullExePath = GetExePath_Internal();

      if (fullExePath.length() == 0)
      {
         throw Exception("Failed to resolve path to the executable");
      }
   }

   return fullExePath;
}

void ReadWholeFile(const std::string& filePath, std::vector<char>& data)
{
   FileOnDisk fileOnDisk(filePath, DataStream::OpenOperation::Read);

   fileOnDisk.ReadWholeFile(data);
}

void ReadWholeFile(const MountedFilePath& mountedFilePath, std::vector<char>& data)
{
   File file(mountedFilePath, DataStream::OpenOperation::Read);

   file.ReadWholeFile(data);
}

}