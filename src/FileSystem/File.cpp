/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

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
   : impl(std::make_unique<File_Impl>()), mountedFilePath(mountedFilePath)
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