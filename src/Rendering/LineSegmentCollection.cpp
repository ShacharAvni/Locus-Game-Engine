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
#include "Locus/Rendering/DefaultGPUVertexData.h"

#include <Locus/Rendering/Locus_glew.h>

#include "Locus/Common/Util.h"

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

void LineSegmentCollection::SortAndRemoveDuplicateLineSegments()
{
   for (ColoredLineSegment& coloredLineSegment : lineSegments)
   {
      if (!(coloredLineSegment.segment.P1 < coloredLineSegment.segment.P2))
      {
         std::swap(coloredLineSegment.segment.P1, coloredLineSegment.segment.P2);
      }
   }

   SortAndRemoveDuplicates(lineSegments);
}

void LineSegmentCollection::UpdateGPUVertexData()
{
   if (defaultGPUVertexData != nullptr)
   {
      std::size_t numLineSegments = lineSegments.size();

      std::size_t numVertices = numLineSegments * 2;

      defaultGPUVertexData->Bind();
      defaultGPUVertexData->Buffer(numVertices, GL_STATIC_DRAW);

      if (numVertices > 0)
      {
         std::vector<GPUVertexDataStorage> vertData(numVertices);

         for (std::size_t lineSegmentIndex = 0, vertDataIndex = 0; lineSegmentIndex < numLineSegments; ++lineSegmentIndex)
         {
            for (std::size_t linePointIndex = 0; linePointIndex < 2; ++linePointIndex)
            {
               const FVector3& point = (linePointIndex == 0) ? lineSegments[lineSegmentIndex].segment.P1 : lineSegments[lineSegmentIndex].segment.P2;

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

         defaultGPUVertexData->BufferSub(0, numVertices, vertData.data());
      }

      defaultGPUVertexData->transferInfo.sendPositions = true;
      defaultGPUVertexData->transferInfo.sendColors = true;
      defaultGPUVertexData->transferInfo.sendNormals = false;
      defaultGPUVertexData->transferInfo.sendTexCoords = false;

      defaultGPUVertexData->drawMode = GL_LINES;
   }
}

bool operator==(const LineSegmentCollection::ColoredLineSegment& first, const LineSegmentCollection::ColoredLineSegment& second)
{
   return ((first.color == second.color) && (first.segment.P1 == second.segment.P1) && (first.segment.P2 == second.segment.P2));
}

bool operator!=(const LineSegmentCollection::ColoredLineSegment& first, const LineSegmentCollection::ColoredLineSegment& second)
{
   return !(first == second);
}

bool operator<(const LineSegmentCollection::ColoredLineSegment& first, const LineSegmentCollection::ColoredLineSegment& second)
{
   if (first.segment.P1 != second.segment.P1)
   {
      return (first.segment.P1 < second.segment.P1);
   }

   if (first.segment.P2 != second.segment.P2)
   {
      return (first.segment.P2 < second.segment.P2);
   }

   return (first.color < second.color);
}

}