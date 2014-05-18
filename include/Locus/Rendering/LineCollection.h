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

#include "SingleDrawable.h"
#include "Color.h"

#include <vector>
#include <utility>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API LineCollection : public SingleDrawable
{
public:
   typedef std::pair<Vector3, Vector3> line_t;
   typedef std::vector< line_t > lines_t;

   LineCollection();
   LineCollection(const lines_t& lines);
   LineCollection(const lines_t& lines, const std::vector<Color>& colors);

   void AddLine(const line_t& line, const Color& color = Color::White());

   void Clear();

   virtual void UpdateGPUVertexData() override;

private:
   lines_t lines;
   std::vector<Color> colors;

   void Refit();
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}