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

#include <vector>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

/// A DataStream wrapping a contiguous chunk of data in memory.
class LOCUS_FILE_SYSTEM_API InMemoryDataStream : public DataStream
{
public:
   /// \details The vector is deep copied by this instance.
   InMemoryDataStream(const std::vector<char>& data);

   InMemoryDataStream(std::vector<char>&& data);

   /// \sa DataStream::Read(char* bytes, std::size_t numBytesToRead)
   virtual std::size_t Read(char* bytes, std::size_t numBytesToRead) override;

   /*!
    * \brief Sets the data used by this stream.
    *
    * \details The vector is deep copied by this instance.
    * The seek position is set to the beginning.
    */
   void SetData(const std::vector<char>& data);

   /*!
    * \brief Sets the data used by this stream.
    *
    * \details The seek position is set to the
    * beginning.
    */
   void SetData(std::vector<char>&& data);

   /// \sa DataStream::IsEndOfStream
   virtual bool IsEndOfStream() const override;

   /// \sa DataStream::CurrentPosition
   virtual std::size_t CurrentPosition() const override;

   /// \sa DataStream::SizeInBytes
   virtual std::size_t SizeInBytes() const override;

   /// \return The number of bytes that need to be read to bring the seek position to the end of the file.
   std::size_t BytesRemaining() const;

   /// \sa DataStream::Seek
   virtual bool Seek(std::size_t offset, SeekType seekType) override;

private:
   std::vector<char> data;
   std::size_t streamPosition;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

} // namespace Locus