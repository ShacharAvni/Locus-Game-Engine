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

#pragma once

#include "LocusRenderingAPI.h"

#include "Locus/Geometry/Model.h"
#include "Locus/Geometry/Vector3.h"

#include "SingleDrawable.h"
#include "TextureCoordinate.h"
#include "Color.h"
#include "GPUVertexDataStorage.h"

#include <cstddef>

namespace Locus
{

class ShaderController;

struct MeshVertexIndexer : public ModelVertexIndexer
{
   MeshVertexIndexer() 
      : ModelVertexIndexer(), textureCoordinateID(0), normalID(0), colorID(0)
   {
   }

   MeshVertexIndexer(std::size_t positionID, std::size_t textureCoordinateID, std::size_t normalID, std::size_t colorID)
      : ModelVertexIndexer(positionID), textureCoordinateID(textureCoordinateID), normalID(normalID), colorID(colorID)
   {
   }

   bool operator <(const MeshVertexIndexer& other) const
   {
      if (ModelVertexIndexer::operator<(other))
      {
         return true;
      }

      if (textureCoordinateID == other.textureCoordinateID)
      {
         if (normalID == other.normalID)
         {
            return colorID < other.colorID;
         }
         else
         {
            return normalID < other.normalID;
         }
      }
      else
      {
         return textureCoordinateID < other.textureCoordinateID;
      }
   }

   std::size_t textureCoordinateID;
   std::size_t normalID;
   std::size_t colorID;
};

struct MeshVertex : public ModelVertex
{
   void Interpolate(const Vector3& interpolatePosition, const MeshVertex& faceVertex1, const MeshVertex& faceVertex2, const MeshVertex& faceVertex3, const Vector3& barycentricCoordinates)
   {
      ModelVertex::Interpolate(interpolatePosition, faceVertex1, faceVertex2, faceVertex3, barycentricCoordinates);

      textureCoordinate.x = (barycentricCoordinates.x * faceVertex1.textureCoordinate.x) + (barycentricCoordinates.y * faceVertex2.textureCoordinate.x) + (barycentricCoordinates.z * faceVertex3.textureCoordinate.x);
      textureCoordinate.y = (barycentricCoordinates.x * faceVertex1.textureCoordinate.y) + (barycentricCoordinates.y * faceVertex2.textureCoordinate.y) + (barycentricCoordinates.z * faceVertex3.textureCoordinate.y);
   }

   TextureCoordinate textureCoordinate;
};

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API Mesh : public Model<MeshVertexIndexer, MeshVertex>, public SingleDrawable
{
public:
   Mesh();
   Mesh(const std::vector<std::vector<MeshVertex>>& faceTriangles);

   std::vector<TextureCoordinate> getTextureCoords(std::size_t faceIndex) const;

   void SetColor(const Color& color);

   void AddTextureCoordinate(const TextureCoordinate& tc);

   void AssignNormals();
   virtual void Clear();

   virtual void UpdateGPUVertexData() override;
   void SetGPUVertexAttributes(ShaderController& ShaderController) const;
   void BindGPUVertexData() const;

   GPUVertexDataTransferInfo gpuVertexDataTransferInfo;

protected:
   virtual MeshVertex GetVertex(std::size_t faceIndex, int vertexIndex) const override;

   virtual void Construct(const std::vector<std::vector<MeshVertex>>& faceTriangles, std::vector<std::size_t>* degenerateFaceIndices = nullptr) override;

private:
   std::vector<Vector3> normals;
   std::vector<TextureCoordinate> textureCoordinates;
   std::vector<Color> colors;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}