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

#include "LocusRenderingAPI.h"

#include "GLCommonTypes.h"

#include "Locus/FileSystem/MountedFilePath.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace Locus
{

class GLInfo;
class Texture;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API TextureManager
{
public:
   TextureManager(const GLInfo& glInfo);
   TextureManager(const TextureManager&) = delete;
   TextureManager& operator=(const TextureManager&) = delete;
   virtual ~TextureManager();

   void Load(const std::string& textureName, const std::string& textureLocation, bool clamp);
   void Load(const std::string& textureName, const MountedFilePath& textureLocation, bool clamp);

   Texture* GetTexture(const std::string& textureName) const;

   virtual void UnLoad();

private:
   std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

   const GLInfo& glInfo;

   template <class FilePathType>
   void Load(const std::string& textureName, const FilePathType& textureFilePath, bool clamp);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}