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

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

struct LOCUS_XML_API XMLAttribute
{
   XMLAttribute();
   XMLAttribute(const std::string& name, const std::string& value);

   std::string name;
   std::string value;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}