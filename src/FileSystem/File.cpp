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

#include "Locus/FileSystem/File.h"

#include "Locus/Common/Exception.h"
#include "Locus/Common/Casts.h"

#include "physfs.h"

#include <limits>

#include <cassert>

namespace Locus
{

struct File_Impl
{
   PHYSFS_File* physfsFileHandle;
};

File::File(const MountedFilePath& mountedFilePath, DataStream::OpenOperation openOperation)
   : impl(new File_Impl()), mountedFilePath(mountedFilePath)
{
   const char* path = mountedFilePath.path.c_str();

   int fileExists = PHYSFS_exists(path);

   if (fileExists == 0)
   {
      throw Exception(std::string("File ") + path + " does not exist in the search path");
   }

   switch (openOperation)
   {
   case OpenOperation::Read:
      impl->physfsFileHandle = PHYSFS_openRead(path);
      break;

   case OpenOperation::Write:
      impl->physfsFileHandle = PHYSFS_openWrite(path);
      break;

   case OpenOperation::Append:
      impl->physfsFileHandle = PHYSFS_openAppend(path);
      break;
   }

   if (impl->physfsFileHandle == nullptr)
   {
      throw Exception(std::string("Failed to open file ") + path + "\n" + PHYSFS_getLastError());
   }
}

File::~File()
{
   #ifndef NDEBUG
   int closed = 
   #endif
   PHYSFS_close(impl->physfsFileHandle);

   assert(closed != 0);
}

bool File::IsEndOfStream() const
{
   return (PHYSFS_eof(impl->physfsFileHandle) != 0);
}

std::size_t File::CurrentPosition() const
{
   PHYSFS_sint64 position = PHYSFS_tell(impl->physfsFileHandle);

   if (position == -1)
   {
      throw Exception(std::string("Could not obtain current position of file ") + mountedFilePath.path + "\n" + PHYSFS_getLastError());
   }

   return LossyCast<PHYSFS_sint64, std::size_t>(position);
}

std::size_t File::SizeInBytes() const
{
   return LossyCast<PHYSFS_sint64, std::size_t>( PHYSFS_fileLength(impl->physfsFileHandle) );
}

void File::ReadWholeFile(std::vector<char>& bytes)
{
   if (PHYSFS_seek(impl->physfsFileHandle, 0) == 0)
   {
      throw Exception(std::string("Seek failed on file ") + mountedFilePath.path + "\n" + PHYSFS_getLastError());
   }

   int64_t fileSizeInBytes = SizeInBytes();

   if (fileSizeInBytes == -1)
   {
      throw Exception(std::string("File sizing failed on file") + mountedFilePath.path + "\n" + PHYSFS_getLastError());
   }

   std::size_t bytesToRead = static_cast<size_t>(fileSizeInBytes);

   std::size_t maxBytesToRead = std::numeric_limits<std::size_t>::max();

   if (static_cast<uint64_t>(fileSizeInBytes) > maxBytesToRead)
   {
      bytesToRead = maxBytesToRead;
   }

   bytes.resize(bytesToRead);

   int64_t bytesRead = Read(bytes.data(), bytesToRead);

   if (static_cast<uint64_t>(bytesRead) != bytesToRead)
   {
      throw Exception(std::string("Failed to read whole file") + mountedFilePath.path + "\n" + PHYSFS_getLastError());
   }
}

std::size_t File::Read(char* bytes, std::size_t numBytesToRead)
{
   PHYSFS_sint64 bytesRead = PHYSFS_read(impl->physfsFileHandle, bytes, 1, LossyCast<std::size_t, PHYSFS_uint32>(numBytesToRead));

   if (bytesRead == -1)
   {
      return 0;
   }

   return LossyCast<PHYSFS_sint64, std::size_t>(bytesRead);
}

bool File::Seek(std::size_t offset, DataStream::SeekType seekType)
{
   std::size_t seekPosition = 0;

   switch (seekType)
   {
   case SeekType::Beginning:
      seekPosition = offset;
      break;

   case SeekType::Current:
      seekPosition = CurrentPosition() + offset;
      break;

   case SeekType::End:
      {
         std::size_t sizeInBytes = SizeInBytes();

         if (offset > sizeInBytes)
         {
            return false;
         }
            
         seekPosition = (sizeInBytes - offset);
      }
      break;

   default:
      return false;
   }

   return (PHYSFS_seek(impl->physfsFileHandle, seekPosition) != 0);
}

}
