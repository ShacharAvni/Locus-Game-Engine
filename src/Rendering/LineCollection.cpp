/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/LineCollection.h"
#include "Locus/Rendering/GPUVertexData.h"

#include "Locus/Common/Util.h"

#include <Locus/Rendering/Locus_glew.h>

namespace Locus
{

LineCollection::LineCollection()
{
}

LineCollection::LineCollection(const LineCollection::lines_t& lines)
   : lines(lines)
{
   Refit();
}

LineCollection::LineCollection(const LineCollection::lines_t& lines, const std::vector<Color>& colors)
   : lines(lines), colors(colors)
{
   Refit();
}

void LineCollection::AddLine(const LineCollection::line_t& line, const Color& color)
{
   lines.push_back(line);
   colors.push_back(color);
}

void LineCollection::Clear()
{
   Util::ClearAndShrink(lines);
   Util::ClearAndShrink(colors);
}

void LineCollection::Refit()
{
   if (lines.size() == 0)
   {
      Util::ClearAndShrink(colors);
   }
   else if (colors.size() == 0)
   {
      colors.resize(lines.size(), Color::White());
   }
   else if (colors.size() < lines.size())
   {
      std::vector<Color> padColors(lines.size() - colors.size(), colors[colors.size() - 1]);
      colors.insert(colors.end(), padColors.begin(), padColors.end());
   }
   else if (colors.size() > lines.size())
   {
      colors.resize(lines.size());
   }
}

void LineCollection::UpdateGPUVertexData()
{
   if (lines.size() > 0)
   {
      if (gpuVertexData != nullptr)
      {
         std::size_t numLines = lines.size();
         std::size_t numVertices = numLines * 2;

         if (numVertices > 0)
         {
            gpuVertexData->Bind();
            gpuVertexData->Buffer(numVertices, GL_STATIC_DRAW);

            std::vector<GPUVertexDataStorage> vertData(numVertices);

            for (std::size_t lineIndex = 0, vertDataIndex = 0; lineIndex < numLines; ++lineIndex)
            {
               for (std::size_t linePointIndex = 0; linePointIndex < 2; ++linePointIndex)
               {
                  vertData[vertDataIndex].position[0] = static_cast<float>( (linePointIndex == 0) ? lines[lineIndex].first.x : lines[lineIndex].second.x );
                  vertData[vertDataIndex].position[1] = static_cast<float>( (linePointIndex == 0) ? lines[lineIndex].first.y : lines[lineIndex].second.y );
                  vertData[vertDataIndex].position[2] = static_cast<float>( (linePointIndex == 0) ? lines[lineIndex].first.z : lines[lineIndex].second.z );

                  vertData[vertDataIndex].color[0] = colors[lineIndex].r;
                  vertData[vertDataIndex].color[1] = colors[lineIndex].g;
                  vertData[vertDataIndex].color[2] = colors[lineIndex].b;
                  vertData[vertDataIndex].color[3] = colors[lineIndex].a;

                  ++vertDataIndex;
               }
            }

            gpuVertexData->BufferSub(0, numVertices, vertData.data());

            gpuVertexData->transferInfo.sendPositions = true;
            gpuVertexData->transferInfo.sendColors = true;
            gpuVertexData->transferInfo.sendNormals = false;
            gpuVertexData->transferInfo.sendTexCoords = false;

            gpuVertexData->drawMode = GL_LINES;
         }
      }
   }
}

}