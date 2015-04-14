/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/DrawablePointCloud.h"
#include "Locus/Rendering/ShaderController.h"
#include "Locus/Rendering/DefaultGPUVertexData.h"

#include "Locus/Common/Util.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

DrawablePointCloud::DrawablePointCloud()
{
}

DrawablePointCloud::DrawablePointCloud(const std::vector<FVector3>& pointCloudPositions, const std::vector<Color>& pointCloudColors)
   : PointCloud(pointCloudPositions), colors(pointCloudColors)
{
   Refit();
}

void DrawablePointCloud::Set(const std::vector<FVector3>& pointCloudPositions, const std::vector<Color>& pointCloudColors)
{
   positions = pointCloudPositions;
   colors = pointCloudColors;

   Refit();
}

void DrawablePointCloud::AddPosition(const FVector3& v, const Color& color)
{
   PointCloud::AddPosition(v);
   colors.push_back(color);
}

void DrawablePointCloud::Clear()
{
   PointCloud::Clear();
   ClearAndShrink(colors);
}

void DrawablePointCloud::Refit()
{
   if (positions.size() == 0)
   {
      ClearAndShrink(colors);
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
      if (defaultGPUVertexData != nullptr)
      {
         defaultGPUVertexData->Bind();
         defaultGPUVertexData->Buffer(numVertices, GL_STATIC_DRAW);

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

         defaultGPUVertexData->BufferSub(0, numVertices, vertData.data());

         defaultGPUVertexData->transferInfo.sendPositions = true;
         defaultGPUVertexData->transferInfo.sendColors = true;
         defaultGPUVertexData->transferInfo.sendNormals = false;
         defaultGPUVertexData->transferInfo.sendTexCoords = false;

         defaultGPUVertexData->drawMode = GL_POINTS;
      }
   }
}

}