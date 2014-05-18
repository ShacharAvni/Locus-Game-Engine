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

#include "LocusRenderingAPI.h"

namespace Locus
{

class LOCUS_RENDERING_API GLInfo
{
public:
   enum class Vendor
   {
      Unknown = 0,
      ATI,
      NVIDIA,
      Intel,
      Microsoft
   };

   enum class GLSLVersion
   {
      Unsupported = 0,
      V_110,
      V_120,
      V_130
   };

   GLInfo(GLSLVersion minRequiredGLSLVersion);

   Vendor GetVendor() const;

   GLSLVersion GetHighestSupportedGLSLVersion() const;
   bool Supports(GLSLVersion version) const;

private:
   Vendor vendor;
   GLSLVersion highestSupportedGLSLVersion;

   void InitializeVendor();
   bool InitializeGLSL();
};

}