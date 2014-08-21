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

#include <vector>

namespace Locus
{

/*!
 * \brief Virtual base class for something (file, archive, vector of bytes, etc.)
 * that can be read from, bytes at a time, from a location that can be seeked to.
 *
 * \details The underlying data is read only.
 */
class LOCUS_FILE_SYSTEM_API DataStream
{
public:
   virtual ~DataStream();

   /*!
    * \brief Reads bytes from the DataStream, starting at the current read location.
    *
    * \param[in,out] bytes The bytes to be read. Must be pre-allocated.
    * \param[in] numBytesToRead The number of bytes to read starting at the current
    * read location.
    *
    * \return The number of bytes read.
    */
   virtual std::size_t Read(char* bytes, std::size_t numBytesToRead) = 0;

   /*!
    * \brief Reads bytes from the DataStream into a std::vector, starting at the current
    * read location.
    *
    * \param[in,out] bytes The bytes to be read. It must be at least size
    * offsetIntoBytesVector + numBytesToRead. It is not resized by this method.
    * \param[in] numBytesToRead The number of bytes to read starting at the current
    * read location.
    * \param[in] offsetIntoBysteVector The zero-based index into the std::vector bytes
    * at which data read from this method will be placed.
    *
    * \return The number of bytes read.
    */
   std::size_t Read(std::vector<char>& bytes, std::size_t numBytesToRead, std::size_t offsetIntoBytesVector);

   /// Returns true if the end of stream has been reached.
   virtual bool IsEndOfStream() const = 0;

   /// Returns the current read position (zero-based offset).
   virtual std::size_t CurrentPosition() const = 0;

   /// Returns the size in bytes of the underlying stream.
   virtual std::size_t SizeInBytes() const = 0;

   /// How to interpret the offset of the seek operation.
   enum class SeekType
   {
      Beginning, ///< Offset is interpreted as a zero-based index from the beginning of the stream.
      Current, ///< Offset is interpreted as a zero-based index from the current read position.
      End ///< Offset is interpreted as a zero-based index from the end of the stream.
   };

   /*!
    * \brief Changes the current read position.
    *
    * \param[in] offset A zero-based index interpreted differently depending on the seekType
    * provided.
    * \param[in] seekType how to interpret the offset provided.
    *
    * \return true if the operation has succeeded.
    *
    * \sa SeekType
    */
   virtual bool Seek(std::size_t offset, SeekType seekType) = 0;

   /// The permissions required when opening the File or FileOnDisk.
   enum class OpenOperation
   {
      Read,
      Write,
      Append
   };
};

} // namespace Locus