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
#include "MountedFilePath.h"

#include <vector>
#include <memory>

namespace Locus
{

struct File_Impl;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

/// Represents either a file on disk or a file in an archive.
class LOCUS_FILE_SYSTEM_API File : public DataStream
{
public:
   /*!
    * \details The MountedFilePath provided must be relative
    * to a path that was passed to MountDirectoryOrArchive.
    *
    * \throws Exception
    *
    * \sa MountDirectoryOrArchive DataStream::OpenOperation
    */
   File(const MountedFilePath& mountedFilePath, DataStream::OpenOperation openOperation);

   File(const File&) = delete;
   File& operator=(const File&) = delete;
   ~File();

   /// \sa DataStream::IsEndOfStream
   virtual bool IsEndOfStream() const override;

   /// \sa DataStream::CurrentPosition
   virtual std::size_t CurrentPosition() const override;

   /// \sa DataStream::SizeInBytes
   virtual std::size_t SizeInBytes() const override;

   /*!
    * \brief Reads the whole file into the provided std::vector.
    *
    * \param[in,out] bytes The data read from the file. This vector
    * does not need to be the correct size at call time.
    *
    * \throws Exception
    */
   void ReadWholeFile(std::vector<char>& bytes);

   /// \sa DataStream::Read(char* bytes, std::size_t numBytesToRead)
   virtual std::size_t Read(char* bytes, std::size_t numBytesToRead) override;

   /// \sa DataStream::Seek
   virtual bool Seek(std::size_t offset, DataStream::SeekType seekType) override;

private:
   std::unique_ptr<File_Impl> impl;
   MountedFilePath mountedFilePath;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

} // namespace Locus