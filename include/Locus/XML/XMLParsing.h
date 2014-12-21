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

#include "LocusXMLAPI.h"

#include <string>

namespace Locus
{

struct XMLTag;
struct MountedFilePath;
class DataStream;

LOCUS_XML_API void ParseXMLFile(const std::string& fullFilePath, XMLTag& rootTag);

LOCUS_XML_API void ParseXMLFile(const MountedFilePath& mountedFilePath, XMLTag& rootTag);

LOCUS_XML_API void ParseXMLFile(DataStream& xmlDataStream, XMLTag& rootTag);

}