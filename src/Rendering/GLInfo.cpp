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

#include "Locus/Rendering/GLInfo.h"

#include "Locus/Common/Exception.h"

#include <string>
#include <algorithm>
#include <regex>

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

GLInfo::GLInfo(GLSLVersion minRequiredGLSLVersion)
   : vendor(Vendor::Unknown), highestSupportedGLSLVersion(GLSLVersion::Unsupported)
{
   InitializeVendor();

   if (!InitializeGLSL())
   {
      throw Exception("Failed to read GLSL version");
   }

   if (!Supports(minRequiredGLSLVersion))
   {
      throw Exception("Required GLSL version is unsupported");
   }
}

GLInfo::Vendor GLInfo::GetVendor() const
{
   return vendor;
}

void GLInfo::InitializeVendor()
{
   vendor = Vendor::Unknown;

   std::string vendorString = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

   if (vendorString.length() > 0)
   {
      //make the vendor string lowercase
      std::transform(vendorString.begin(), vendorString.end(), vendorString.begin(), tolower);

      if (vendorString.find("nvidia") != std::string::npos)
      {
         vendor = Vendor::NVIDIA;
      }
      else if (vendorString.find("ati") != std::string::npos)
      {
         vendor = Vendor::ATI;
      }
      else if (vendorString.find("intel") != std::string::npos)
      {
         vendor = Vendor::Intel;
      }
      else if (vendorString.find("microsoft") != std::string::npos)
      {
         vendor = Vendor::Microsoft;
      }
   }
}

bool GLInfo::InitializeGLSL()
{
   std::string rawGLSLVersionString = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

   if (rawGLSLVersionString.length() > 0)
   {
      std::regex getVersionPattern("(\\d+?)\\.(\\d+)");

      std::smatch matches;

      if (std::regex_search(rawGLSLVersionString, matches, getVersionPattern))
      {
         int majorVersion = std::stoi(matches[1]);
         int minorVersion = std::stoi(matches[2]);

         if (majorVersion > 1)
         {
            highestSupportedGLSLVersion = GLSLVersion::V_130;
         }
         else if (minorVersion >= 30)
         {
            highestSupportedGLSLVersion = GLSLVersion::V_130;
         }
         else if (minorVersion >= 20)
         {
            highestSupportedGLSLVersion = GLSLVersion::V_120;
         }
         else
         {
            highestSupportedGLSLVersion = GLSLVersion::V_110;
         }

         return true;
      }
   }

   return false;
}

GLInfo::GLSLVersion GLInfo::GetHighestSupportedGLSLVersion() const
{
   return highestSupportedGLSLVersion;
}

bool GLInfo::Supports(GLInfo::GLSLVersion version) const
{
   return version <= highestSupportedGLSLVersion;
}

}