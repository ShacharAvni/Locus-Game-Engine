 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusRenderingAPI.h"

#include "Locus/Geometry/PointCloud.h"

#include "SingleDrawable.h"
#include "Color.h"

#include <vector>

namespace Locus
{

class ShaderController;

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

class LOCUS_RENDERING_API DrawablePointCloud : public PointCloud, public SingleDrawable
{
public:
   DrawablePointCloud();
   DrawablePointCloud(const std::vector<Vector3>& pointCloudPositions, const std::vector<Color>& pointCloudColors);
      
   void Set(const std::vector<Vector3>& pointCloudPositions, const std::vector<Color>& pointCloudColors);

   virtual void AddPosition(const Vector3& v, const Color& color);
   virtual void Clear();

   virtual void UpdateGPUVertexData();

private:
   std::vector<Color> colors;

   void Refit();
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

}