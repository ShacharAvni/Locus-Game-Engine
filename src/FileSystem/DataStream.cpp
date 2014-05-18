/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/FileSystem/DataStream.h"

namespace Locus
{

DataStream::~DataStream()
{
}

std::size_t DataStream::Read(std::vector<char>& bytes, std::size_t numBytesToRead, std::size_t offsetIntoBytesVector)
{
   if ((offsetIntoBytesVector + numBytesToRead) > bytes.size())
   {
      return 0;
   }

   return Read(bytes.data() + offsetIntoBytesVector, numBytesToRead);
}

}