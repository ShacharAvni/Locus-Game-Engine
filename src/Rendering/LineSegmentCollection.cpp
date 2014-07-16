/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/LineSegmentCollection.h"
#include "Locus/Rendering/GPUVertexData.h"

#include <Locus/Rendering/Locus_glew.h>

#include <cassert>

namespace Locus
{

LineSegmentCollection::LineSegmentCollection()
{
}

LineSegmentCollection::LineSegmentCollection(const std::vector<LineSegmentCollection::ColoredLineSegment>& lineSegments)
   : lineSegments(lineSegments)
{
}

LineSegmentCollection::LineSegmentCollection(std::vector<LineSegmentCollection::ColoredLineSegment>&& lineSegments)
   : lineSegments(lineSegments)
{
}

std::size_t LineSegmentCollection::NumLineSegments() const
{
   return lineSegments.size();
}

void LineSegmentCollection::Clear()
{
   lineSegments.clear();
}

LineSegmentCollection::ColoredLineSegment& LineSegmentCollection::operator[](std::size_t index)
{
   assert(index < NumLineSegments());

   return lineSegments[index];
}

const LineSegmentCollection::ColoredLineSegment& LineSegmentCollection::operator[](std::size_t index) const
{
   assert(index < NumLineSegments());

   return lineSegments[index];
}

void LineSegmentCollection::AddLineSegment(const LineSegmentCollection::ColoredLineSegment& lineSegment)
{
   lineSegments.push_back(lineSegment);
}

void LineSegmentCollection::InsertLineSegment(std::size_t index, const LineSegmentCollection::ColoredLineSegment& lineSegment)
{
   assert(index < NumLineSegments());

   lineSegments.insert(lineSegments.begin() + index, lineSegment);
}

void LineSegmentCollection::UpdateGPUVertexData()
{
   if (gpuVertexData != nullptr)
   {
      std::size_t numLineSegments = lineSegments.size();

      std::size_t numVertices = numLineSegments * 2;

      gpuVertexData->Bind();
      gpuVertexData->Buffer(numVertices, GL_STATIC_DRAW);

      if (numVertices > 0)
      {
         std::vector<GPUVertexDataStorage> vertData(numVertices);

         for (std::size_t lineSegmentIndex = 0, vertDataIndex = 0; lineSegmentIndex < numLineSegments; ++lineSegmentIndex)
         {
            for (std::size_t linePointIndex = 0; linePointIndex < 2; ++linePointIndex)
            {
               const Vector3& point = (linePointIndex == 0) ? lineSegments[lineSegmentIndex].segment.P1 : lineSegments[lineSegmentIndex].segment.P2;

               vertData[vertDataIndex].position[0] = static_cast<float>( point.x );
               vertData[vertDataIndex].position[1] = static_cast<float>( point.y );
               vertData[vertDataIndex].position[2] = static_cast<float>( point.z );

               const Color& color = lineSegments[lineSegmentIndex].color;

               vertData[vertDataIndex].color[0] = color.r;
               vertData[vertDataIndex].color[1] = color.g;
               vertData[vertDataIndex].color[2] = color.b;
               vertData[vertDataIndex].color[3] = color.a;

               ++vertDataIndex;
            }
         }

         gpuVertexData->BufferSub(0, numVertices, vertData.data());
      }

      gpuVertexData->transferInfo.sendPositions = true;
      gpuVertexData->transferInfo.sendColors = true;
      gpuVertexData->transferInfo.sendNormals = false;
      gpuVertexData->transferInfo.sendTexCoords = false;

      gpuVertexData->drawMode = GL_LINES;
   }
}

}