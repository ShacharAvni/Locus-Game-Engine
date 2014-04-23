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