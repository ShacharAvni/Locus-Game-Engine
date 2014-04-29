 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusFileSystemAPI.h"
#include "DataStream.h"

#include <string>
#include <vector>
#include <memory>

namespace Locus
{

struct FileOnDisk_Impl;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_FILE_SYSTEM_API FileOnDisk : public DataStream
{
public:
   FileOnDisk(const std::string& filePath, DataStream::OpenOperation openOperation);
   FileOnDisk(const FileOnDisk&) = delete;
   FileOnDisk& operator=(const FileOnDisk&) = delete;
   ~FileOnDisk();

   virtual bool IsEndOfStream() const override;
   virtual std::size_t CurrentPosition() const override;
   virtual std::size_t SizeInBytes() const override;

   void ReadWholeFile(std::vector<char>& bytes);
   virtual std::size_t Read(char* bytes, std::size_t numBytesToRead) override;

   virtual bool Seek(std::size_t offset, DataStream::SeekType seekType) override;

private:
   std::unique_ptr<FileOnDisk_Impl> impl;
   std::string filePath;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}