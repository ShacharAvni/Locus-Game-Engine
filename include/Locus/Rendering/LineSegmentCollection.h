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

#include "Locus/Geometry/Vector3.h"
#include "Locus/Geometry/LineSegment.h"

#include "SingleDrawable.h"
#include "Color.h"

#include <vector>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API LineSegmentCollection : public SingleDrawable
{
public:
   struct ColoredLineSegment
   {
      LineSegment<Vector3> segment;
      Color color;
   };

   LineSegmentCollection();
   LineSegmentCollection(const std::vector<ColoredLineSegment>& lineSegments);
   LineSegmentCollection(std::vector<ColoredLineSegment>&& lineSegments);

   std::size_t NumLineSegments() const;

   void Clear();

   ColoredLineSegment& operator[](std::size_t index);
   const ColoredLineSegment& operator[](std::size_t index) const;

   void AddLineSegment(const ColoredLineSegment& lineSegment);

   void InsertLineSegment(std::size_t index, const ColoredLineSegment& lineSegment);

   virtual void UpdateGPUVertexData() override;

private:
   std::vector<ColoredLineSegment> lineSegments;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}