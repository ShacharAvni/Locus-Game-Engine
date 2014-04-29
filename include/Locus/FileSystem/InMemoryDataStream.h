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

#include <vector>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_FILE_SYSTEM_API InMemoryDataStream : public DataStream
{
public:
   InMemoryDataStream(const std::vector<char>& data);
   InMemoryDataStream(std::vector<char>&& data);

   virtual std::size_t Read(char* bytes, std::size_t numBytesToRead) override;

   void SetData(const std::vector<char>& data);
   void SetData(std::vector<char>&& data);

   virtual bool IsEndOfStream() const override;
   virtual std::size_t CurrentPosition() const override;
   virtual std::size_t SizeInBytes() const override;
   std::size_t BytesRemaining() const;

   virtual bool Seek(std::size_t offset, SeekType seekType) override;
   bool SeekFromBeginning(std::size_t offset);
   bool SeekFromCurrentPosition(std::size_t offset);
   bool SeekFromEnd(std::size_t offset);

private:
   std::vector<char> data;
   std::size_t streamPosition;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}