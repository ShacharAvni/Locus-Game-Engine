/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/XML/XMLAttribute.h"

namespace Locus
{

XMLAttribute::XMLAttribute()
{
}

XMLAttribute::XMLAttribute(const std::string& name, const std::string& value)
   : name(name), value(value)
{
}

}