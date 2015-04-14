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

#include "LocusGeometryAPI.h"

#include "Locus/Math/VectorsFwd.h"

#include "Polygon.h"

#include <vector>

namespace Locus
{

LOCUS_GEOMETRY_API void Triangulate(const Polygon2D_t& polygon, std::vector<const FVector2*>& triangles);
LOCUS_GEOMETRY_API void Triangulate(const Polygon2D_t& polygon, const std::vector<const Polygon2D_t*>& innerPolygons, std::vector<const FVector2*>& triangles);
LOCUS_GEOMETRY_API void Triangulate(std::vector<Polygon2D_t>& polygons, PolygonWinding winding, std::vector<const FVector2*>& triangles);

}