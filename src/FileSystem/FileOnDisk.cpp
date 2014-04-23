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
