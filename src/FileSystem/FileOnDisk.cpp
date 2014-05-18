/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/FileSystem/FileOnDisk.h"

#include "Locus/Common/Casts.h"
#include "Locus/Common/Exception.h"

#include <cstdio>
#include <cassert>

namespace Locus
{

struct FileOnDisk_Impl
{
   std::FILE* fileHandle;
};

FileOnDisk::FileOnDisk(const std::string& filePath, DataStream::OpenOperation openOperation)
   : impl(new FileOnDisk_Impl()), filePath(filePath)
{
   const char* mode = "";

   switch (openOperation)
   {
   case OpenOperation::Append:
      mode = "ab";
      break;

   case OpenOperation::Read:
      mode = "rb";
      break;

   case OpenOperation::Write:
      mode = "wb";
      break;

   default:
      throw Exception("Unknown File Open Operation");
   }

   impl->fileHandle = nullptr;

#if defined(LOCUS_WINDOWS) && defined(_MSC_VER)
   errno_t openError = fopen_s(&(impl->fileHandle), filePath.c_str(), mode);

   if (openError != 0)
   {
      throw Exception(std::string("Failed to open file ") + filePath + "\nerror code: " + std::to_string(openError));
   }
#else
   impl->fileHandle = std::fopen(filePath.c_str(), mode);
#endif

   if (impl->fileHandle == nullptr)
   {
      throw Exception(std::string("Failed to open file ") + filePath);
   }
}

FileOnDisk::~FileOnDisk()
{
   #ifndef NDEBUG
      int closeResult = 
   #endif
   std::fclose(impl->fileHandle);

   assert(closeResult == 0);
}

bool FileOnDisk::IsEndOfStream() const
{
   return (feof(impl->fileHandle) != 0);
}

std::size_t FileOnDisk::CurrentPosition() const
{
   return LossyCast<long int, std::size_t>( ftell(impl->fileHandle) );
}

std::size_t FileOnDisk::SizeInBytes() const
{
   std::fpos_t position;

   if (fgetpos(impl->fileHandle, &position) == 0)
   {
      if (fseek(impl->fileHandle, 0, SEEK_END) == 0)
      {
         long int fileSize = ftell(impl->fileHandle);

         if (fileSize != -1)
         {
            if (fsetpos(impl->fileHandle, &position) == 0)
            {
               return LossyCast<long int, std::size_t>(fileSize);
            }
         }
      }
   }

   throw Exception(std::string("Could not obtain file size of file ") + filePath);
}

void FileOnDisk::ReadWholeFile(std::vector<char>& bytes)
{
   if (!Seek(0, SeekType::Beginning))
   {
      throw Exception(std::string("Seek failed on file ") + filePath);
   }

   std::size_t fileSizeInBytes = SizeInBytes();

   if (fileSizeInBytes == 0)
   {
      bytes.clear();
      return;
   }

   std::size_t bytesToRead = fileSizeInBytes;

   std::size_t maxBytesToRead = std::numeric_limits<std::size_t>::max();

   if (fileSizeInBytes > maxBytesToRead)
   {
      bytesToRead = maxBytesToRead;
   }

   bytes.resize(bytesToRead);

   std::size_t bytesRead = Read(bytes.data(), bytesToRead);

   if (bytesRead != bytesToRead)
   {
      throw Exception(std::string("Failed to read whole file") + filePath);
   }
}

std::size_t FileOnDisk::Read(char* bytes, std::size_t numBytesToRead)
{
   return fread(bytes, 1, numBytesToRead, impl->fileHandle);
}

bool FileOnDisk::Seek(std::size_t offset, DataStream::SeekType seekType)
{
   int origin = 0;

   switch (seekType)
   {
   case SeekType::Beginning:
      origin = SEEK_SET;
      break;

   case SeekType::Current:
      origin = SEEK_CUR;
      break;

   case SeekType::End:
      origin = SEEK_END;
      break;

   default:
      return false;
   }

   return (fseek(impl->fileHandle, LossyCast<std::size_t, long int>(offset), origin) == 0);
}

}