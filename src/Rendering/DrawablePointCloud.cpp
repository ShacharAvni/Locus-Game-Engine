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

#include "Locus/Rendering/DrawablePointCloud.h"
#include "Locus/Rendering/ShaderController.h"
#include "Locus/Rendering/GPUVertexData.h"

#include "Locus/Common/Util.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

DrawablePointCloud::DrawablePointCloud()
{
}

DrawablePointCloud::DrawablePointCloud(const std::vector<Vector3>& pointCloudPositions, const std::vector<Color>& pointCloudColors)
   : PointCloud(pointCloudPositions), colors(pointCloudColors)
{
   Refit();
}

void DrawablePointCloud::Set(const std::vector<Vector3>& pointCloudPositions, const std::vector<Color>& pointCloudColors)
{
   positions = pointCloudPositions;
   colors = pointCloudColors;

   Refit();
}

void DrawablePointCloud::AddPosition(const Vector3& v, const Color& color)
{
   PointCloud::AddPosition(v);
   colors.push_back(color);
}

void DrawablePointCloud::Clear()
{
   PointCloud::Clear();
   Util::ClearAndShrink(colors);
}

void DrawablePointCloud::Refit()
{
   if (positions.size() == 0)
   {
      Util::ClearAndShrink(colors);
   }
   else if (colors.size() == 0)
   {
      colors.resize(positions.size(), Color::White());
   }
   else if (colors.size() < positions.size())
   {
      std::vector<Color> padColors(positions.size() - colors.size(), colors[colors.size() - 1]);
      colors.insert(colors.end(), padColors.begin(), padColors.end());
   }
   else if (colors.size() > positions.size())
   {
      colors.resize(positions.size());
   }
}

void DrawablePointCloud::UpdateGPUVertexData()
{
   std::size_t numVertices = positions.size();

   if (numVertices > 0)
   {
      if (gpuVertexData != nullptr)
      {
         gpuVertexData->Bind();
         gpuVertexData->Buffer(numVertices, GL_STATIC_DRAW);

         std::vector<GPUVertexDataStorage> vertData(numVertices);

         for (std::size_t vertDataIndex = 0; vertDataIndex < numVertices; ++vertDataIndex)
         {
            vertData[vertDataIndex].position[0] = static_cast<float>(positions[vertDataIndex].x);
            vertData[vertDataIndex].position[1] = static_cast<float>(positions[vertDataIndex].y);
            vertData[vertDataIndex].position[2] = static_cast<float>(positions[vertDataIndex].z);

            vertData[vertDataIndex].color[0] = colors[vertDataIndex].r;
            vertData[vertDataIndex].color[1] = colors[vertDataIndex].g;
            vertData[vertDataIndex].color[2] = colors[vertDataIndex].b;
            vertData[vertDataIndex].color[3] = colors[vertDataIndex].a;
         }

         gpuVertexData->BufferSub(0, numVertices, vertData.data());

         gpuVertexData->transferInfo.sendPositions = true;
         gpuVertexData->transferInfo.sendColors = true;
         gpuVertexData->transferInfo.sendNormals = false;
         gpuVertexData->transferInfo.sendTexCoords = false;

         gpuVertexData->drawMode = GL_POINTS;
      }
   }
}

}