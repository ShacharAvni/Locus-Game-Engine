/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/TextureManager.h"
#include "Locus/Rendering/GLInfo.h"
#include "Locus/Rendering/Texture.h"

namespace Locus
{

TextureManager::TextureManager(const GLInfo& glInfo)
   : glInfo(glInfo)
{
}

TextureManager::~TextureManager()
{
   UnLoad();
}

template <class FilePathType>
void TextureManager::Load(const std::string& textureName, const FilePathType& textureFilePath, bool clamp)
{
   if (textures.find(textureName) == textures.end())
   {
      std::unique_ptr<Texture> texture(new Texture(textureFilePath, clamp, glInfo));

      texture->ClearPixelData();

      textures[textureName] = std::move(texture);
   }
}

void TextureManager::Load(const std::string& textureName, const std::string& textureLocation, bool clamp)
{
   Load<std::string>(textureName, textureLocation, clamp);
}

void TextureManager::Load(const std::string& textureName, const MountedFilePath& textureLocation, bool clamp)
{
   Load<MountedFilePath>(textureName, textureLocation, clamp);
}

Texture* TextureManager::GetTexture(const std::string& textureName) const
{
   Texture* texture = nullptr;

   std::unordered_map<std::string, std::unique_ptr<Texture>>::const_iterator textureMapIterator = textures.find(textureName);
   if (textureMapIterator != textures.end())
   {
      texture = textureMapIterator->second.get();
   }

   return texture;
}

void TextureManager::UnLoad()
{
   textures.clear();
}

}