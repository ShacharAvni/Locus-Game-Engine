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