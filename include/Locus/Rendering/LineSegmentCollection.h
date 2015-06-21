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

#include "Locus/Math/Vectors.h"

#include "Locus/Geometry/LineSegment.h"

#include "DefaultSingleDrawable.h"
#include "Color.h"

#include <vector>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API LineSegmentCollection : public DefaultSingleDrawable
{
public:
   struct ColoredLineSegment
   {
      LineSegment<FVector3> segment;
      Color color;
   };

   LineSegmentCollection();
   LineSegmentCollection(const std::vector<ColoredLineSegment>& lineSegments);
   LineSegmentCollection(std::vector<ColoredLineSegment>&& lineSegments);

   void SortAndRemoveDuplicateLineSegments();

   virtual void UpdateGPUVertexData() override;

   std::vector<ColoredLineSegment> lineSegments;
};

LOCUS_RENDERING_API bool operator==(const LineSegmentCollection::ColoredLineSegment& first, const LineSegmentCollection::ColoredLineSegment& second);
LOCUS_RENDERING_API bool operator!=(const LineSegmentCollection::ColoredLineSegment& first, const LineSegmentCollection::ColoredLineSegment& second);
LOCUS_RENDERING_API bool operator<(const LineSegmentCollection::ColoredLineSegment& first, const LineSegmentCollection::ColoredLineSegment& second);

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}