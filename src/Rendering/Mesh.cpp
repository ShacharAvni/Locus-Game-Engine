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

#include "Locus/Rendering/Mesh.h"
#include "Locus/Rendering/GPUVertexData.h"
#include "Locus/Rendering/RenderingState.h"

#include "Locus/Common/Util.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

Mesh::Mesh()
   : Model<MeshVertexIndexer, MeshVertex>(), colors(1, Color::White())
{
}

Mesh::Mesh(const std::vector<std::vector<MeshVertex>>& faceTriangles)
   : Model<MeshVertexIndexer, MeshVertex>(), colors(1, Color::White())
{
   std::vector<std::vector<MeshVertex>> faceTrianglesActual;
   Model::GetTriangles(faceTriangles, faceTrianglesActual);

   Construct(faceTrianglesActual);
}

void Mesh::SetColor(const Color& color)
{
   //only allow one color for now

   colors.clear();
   colors.resize(1, color);
   colors.shrink_to_fit();
}

void Mesh::AddTextureCoordinate(const TextureCoordinate& tc)
{
   textureCoordinates.push_back(tc);
}

void Mesh::Construct(const std::vector<std::vector<MeshVertex>>& faceTriangles, std::vector<std::size_t>* degenerateFaceIndices)
{
   std::vector<std::size_t> degenerateFaceIndicesFromModel;
   Model::Construct(faceTriangles, &degenerateFaceIndicesFromModel);

   textureCoordinates.clear();

   std::size_t numFaces = faceTriangles.size();

   std::size_t numDegenerateFaces = degenerateFaceIndicesFromModel.size();
   bool hasAnyMoreDegenerateFaces = numDegenerateFaces > 0;
   std::size_t currentDegenerateFaceIndexIndex = 0;

   std::vector<TextureCoordinate> vertTexCoords;
   vertTexCoords.reserve(numFaces * Triangle3D_t::NumPointsOnATriangle);

   for (std::size_t faceIndex = 0; faceIndex < numFaces; ++faceIndex)
   {
      bool shouldAddFace = !hasAnyMoreDegenerateFaces;

      if (hasAnyMoreDegenerateFaces)
      {
         //degenerateFaceIndicesFromModel is guaranteed to come in sorted order
         shouldAddFace = degenerateFaceIndicesFromModel[currentDegenerateFaceIndexIndex] != faceIndex;

         if (!shouldAddFace)
         {
            ++currentDegenerateFaceIndexIndex;

            if (currentDegenerateFaceIndexIndex == numDegenerateFaces)
            {
               hasAnyMoreDegenerateFaces = false;
            }
         }
      }

      if (shouldAddFace)
      {
         for (std::size_t i = 0; i < Triangle3D_t::NumPointsOnATriangle; ++i)
         {
            vertTexCoords.push_back(faceTriangles[faceIndex][i].textureCoordinate);
         }
      }
   }

   //get unique texture coordinates
   std::unordered_map<std::size_t, std::size_t> sortedTexCoordMap;
   Util::GetUniqueItems<TextureCoordinate>(vertTexCoords, textureCoordinates, sortedTexCoordMap);

   //fill faces

   std::size_t numVertices = 0;
   for (std::size_t faceIndex = 0, numFaces = faces.size(); faceIndex < numFaces; ++faceIndex)
   {
      for (std::size_t vertexIndex = 0; vertexIndex < Triangle3D_t::NumPointsOnATriangle; ++vertexIndex)
      {
         faces[faceIndex][vertexIndex].textureCoordinateID = sortedTexCoordMap[numVertices];
         ++numVertices;
      }
   }

   if (degenerateFaceIndices != nullptr)
   {
      *degenerateFaceIndices = degenerateFaceIndicesFromModel;
   }
}

MeshVertex Mesh::GetVertex(std::size_t faceIndex, int vertexIndex) const
{
   MeshVertex vertex;

   vertex.textureCoordinate = textureCoordinates[faces[faceIndex][vertexIndex].textureCoordinateID];

   return vertex;
}

void Mesh::AssignNormals()
{
   std::size_t numPositions = positions.size();

   normals.clear();
   normals.resize(numPositions);

   std::vector<std::size_t> normalHitCounts(numPositions);

   Vector3 faceNormal;

   for (face_t& face : faces)
   {
      faceNormal = (positions[face[1].positionID] - positions[face[0].positionID]).cross(positions[face[2].positionID] - positions[face[1].positionID]);

      for (std::size_t vertexIndex = 0; vertexIndex < Triangle3D_t::NumPointsOnATriangle; ++vertexIndex)
      {
         face[vertexIndex].normalID = face[vertexIndex].positionID;

         normals[face[vertexIndex].positionID] += faceNormal;
         ++normalHitCounts[face[vertexIndex].positionID];
      }
   }

   for (std::size_t normalIndex = 0; normalIndex < numPositions; ++normalIndex)
   {
      normals[normalIndex] /= static_cast<float>(normalHitCounts[normalIndex]);
   }
}

void Mesh::Clear()
{
   Model::Clear();

   Util::ClearAndShrink(normals);
   Util::ClearAndShrink(textureCoordinates);
}

void Mesh::SetGPUVertexAttributes(ShaderController& ShaderController) const
{
   gpuVertexData->SetAttributes(ShaderController);
}

void Mesh::BindGPUVertexData() const
{
   gpuVertexData->Bind();
}

void Mesh::UpdateGPUVertexData()
{
   if (numTotalVertices > 0)
   {
      if (gpuVertexData != nullptr)
      {
         bool hasNormals = normals.size() > 0;

         bool sendNormals = gpuVertexDataTransferInfo.sendNormals && hasNormals;

         gpuVertexData->Bind();
         gpuVertexData->Buffer(numTotalVertices, GL_STATIC_DRAW);

         std::vector<GPUVertexDataStorage> vertData(numTotalVertices);

         const Color& meshColor = colors[0];

         std::size_t vertDataIndex = 0;
         for (const face_t& face : faces)
         {
            for (const MeshVertexIndexer& vertex : face)
            {
               GPUVertexDataStorage& currentVertData = vertData[vertDataIndex];

               if (gpuVertexDataTransferInfo.sendPositions)
               {
                  currentVertData.position[0] = static_cast<float>(positions[vertex.positionID].x);
                  currentVertData.position[1] = static_cast<float>(positions[vertex.positionID].y);
                  currentVertData.position[2] = static_cast<float>(positions[vertex.positionID].z);
               }

               if (sendNormals)
               {
                  currentVertData.normal[0] = static_cast<float>(normals[vertex.normalID].x);
                  currentVertData.normal[1] = static_cast<float>(normals[vertex.normalID].y);
                  currentVertData.normal[2] = static_cast<float>(normals[vertex.normalID].z);
               }

               if (gpuVertexDataTransferInfo.sendColors)
               {
                  currentVertData.color[0] = meshColor.r;
                  currentVertData.color[1] = meshColor.g;
                  currentVertData.color[2] = meshColor.b;
                  currentVertData.color[3] = meshColor.a;
               }

               if (gpuVertexDataTransferInfo.sendTexCoords)
               {
                  currentVertData.texCoord[0] = static_cast<float>(textureCoordinates[vertex.textureCoordinateID].x);
                  currentVertData.texCoord[1] = static_cast<float>(textureCoordinates[vertex.textureCoordinateID].y);
               }

               ++vertDataIndex;
            }
         }

         gpuVertexData->BufferSub(0, numTotalVertices, vertData.data());

         gpuVertexData->transferInfo = gpuVertexDataTransferInfo;
         gpuVertexData->transferInfo.sendNormals = sendNormals;

         gpuVertexData->drawMode = GL_TRIANGLES;
      }
   }
}

}