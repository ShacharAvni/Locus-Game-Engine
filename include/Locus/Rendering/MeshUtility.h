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

#include "Locus/Math/VectorsFwd.h"

#include "Mesh.h"
#include "TextureCoordinate.h"

#include <memory>

#include <cstddef>

namespace Locus
{

class LOCUS_RENDERING_API MeshUtility
{
public:
   static std::unique_ptr<Mesh> MakeOctahedron(float radius);
   static std::unique_ptr<Mesh> MakeIcosahedron(float radius);
   static std::unique_ptr<Mesh> MakeSphere(float radius, unsigned int subdivisions);
   static std::unique_ptr<Mesh> MakeCube(float lengthOfOneSide);

   static TextureCoordinate SphericalUVMapping(const FVector3& pointOnSphere);

private:
   static void AddFaceToMesh(Mesh& mesh, std::size_t p1Index, std::size_t p2Index, std::size_t p3Index);
   static void AddFaceToMesh(Mesh& mesh, std::size_t p1PositionIndex, std::size_t p2PositionIndex, std::size_t p3PositionIndex, std::size_t p1TextureCoordIndex, std::size_t p2TextureCoordIndex, std::size_t p3TextureCoordIndex);
};

}