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
   MeshVertex()
      : color(Color::White())
   {
   }

   MeshVertex(const Color& color, const TextureCoordinate& textureCoordinate)
      : color(color), textureCoordinate(textureCoordinate)
   {
   }

   void Interpolate(const Vector3& interpolatePosition, const MeshVertex& faceVertex1, const MeshVertex& faceVertex2, const MeshVertex& faceVertex3, const Vector3& barycentricCoordinates)
   {
      ModelVertex::Interpolate(interpolatePosition, faceVertex1, faceVertex2, faceVertex3, barycentricCoordinates);

      textureCoordinate.x = (barycentricCoordinates.x * faceVertex1.textureCoordinate.x) + (barycentricCoordinates.y * faceVertex2.textureCoordinate.x) + (barycentricCoordinates.z * faceVertex3.textureCoordinate.x);
      textureCoordinate.y = (barycentricCoordinates.x * faceVertex1.textureCoordinate.y) + (barycentricCoordinates.y * faceVertex2.textureCoordinate.y) + (barycentricCoordinates.z * faceVertex3.textureCoordinate.y);
   }

   Color color;
   TextureCoordinate textureCoordinate;
};

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API Mesh : public Model<MeshVertexIndexer, MeshVertex>, public SingleDrawable
{
public:
   Mesh();
   Mesh(const std::vector<std::vector<MeshVertex>>& faceTriangles);

   std::vector<TextureCoordinate> getTextureCoords(std::size_t faceIndex) const;

   void AddColor(const Color& color);
   void AddTextureCoordinate(const TextureCoordinate& tc);

   void SetColor(const Color& color);

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