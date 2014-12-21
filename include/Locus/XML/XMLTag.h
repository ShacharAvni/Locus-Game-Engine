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
#include "XMLAttribute.h"

#include <string>
#include <vector>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

struct LOCUS_XML_API XMLTag
{
   std::string name;
   std::string value;
   std::vector<XMLAttribute> attributes;
   std::vector<XMLTag> subTags;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}