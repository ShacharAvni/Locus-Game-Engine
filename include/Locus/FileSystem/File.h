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

class LOCUS_FILE_SYSTEM_API File : public DataStream
{
public:
   File(const MountedFilePath& mountedFilePath, DataStream::OpenOperation openOperation);
   File(const File&) = delete;
   File& operator=(const File&) = delete;
   ~File();

   virtual bool IsEndOfStream() const override;
   virtual std::size_t CurrentPosition() const override;
   virtual std::size_t SizeInBytes() const override;

   void ReadWholeFile(std::vector<char>& bytes);
   virtual std::size_t Read(char* bytes, std::size_t numBytesToRead) override;

   virtual bool Seek(std::size_t offset, DataStream::SeekType seekType) override;

private:
   std::unique_ptr<File_Impl> impl;
   MountedFilePath mountedFilePath;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}