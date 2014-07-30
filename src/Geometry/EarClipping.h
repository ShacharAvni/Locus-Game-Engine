/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/PolygonFwd.h"

#include <vector>

namespace Locus
{

class Vector2;

namespace EarClipping
{

extern const float EXPERIMENTAL_TOLERANCE;

void Triangulate(const Polygon2D_t& polygon, std::vector<const Vector2*>& triangles);
void Triangulate(const Polygon2D_t& polygon, const std::vector<const Polygon2D_t*>& innerPolygons, std::vector<const Vector2*>& triangles);

}

}