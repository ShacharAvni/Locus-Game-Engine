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
#include "DataStream.h"

#include <string>
#include <vector>
#include <memory>

namespace Locus
{

struct FileOnDisk_Impl;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

/// A plain old file.
class LOCUS_FILE_SYSTEM_API FileOnDisk : public DataStream
{
public:
   /*!
    * \param[in] filePath The full path to the file.
    * \param[in] openMode The usage pattern required for opening the file.
    */
   FileOnDisk(const std::string& filePath, DataStream::OpenMode openMode);

   ~FileOnDisk();

   FileOnDisk(const FileOnDisk&) = delete;
   FileOnDisk& operator=(const FileOnDisk&) = delete;

   /// \sa DataStream::IsEndOfStream
   virtual bool IsEndOfStream() const override;

   /// \sa DataStream::CurrentPosition
   virtual std::size_t CurrentPosition() const override;

   /// \sa DataStream::SizeInBytes
   virtual std::size_t SizeInBytes() const override;

   /// \sa File::ReadWholeFile
   void ReadWholeFile(std::vector<char>& bytes);

   /// \sa DataStream::Read
   virtual std::size_t Read(char* bytes, std::size_t numBytesToRead) override;

   /// \sa DataStream::Seek
   virtual bool Seek(std::size_t offset, DataStream::SeekType seekType) override;

private:
   std::unique_ptr<FileOnDisk_Impl> impl;
   std::string filePath;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

} // namespace Locus