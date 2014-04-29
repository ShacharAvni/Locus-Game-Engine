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

#include "LocusGeometryAPI.h"

#include "Model.h"
#include "Vector3.h"
#include "Triangle.h"

#include <vector>

namespace Locus
{

class LOCUS_GEOMETRY_API ModelUtility
{
public:
   static std::vector<Vector3> OctahedronPositions(float radius);
   static std::vector<Vector3> IcosahedronPositions(float mainLength);
   static std::vector<Vector3> CubePositions(float lengthOfOneSide);

   static Model_t MakeCube(float lengthOfOneSide);
   static Model_t MakeOctahedron(float radius);
   static Model_t MakeSphere(float radius, unsigned int subdivisions);

   static const unsigned int MAX_SPHERE_SUBDIVISIONS;

   static void SubdivideTriangle(const Triangle3D_t& triangle, std::vector<Triangle3D_t>& triangles, unsigned int subdivisionsLeft);
};

}