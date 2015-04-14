/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/Quad.h"
#include "Locus/Rendering/RenderingState.h"

#include "Locus/Geometry/Vector3Geometry.h"

namespace Locus
{

Quad::Quad()
   : Mesh()
{
}

void Quad::Set(const FVector3& lowerLeftCorner, const FVector3& widthDirection, const FVector3& heightDirection, float width, float height, const Color& color)
{
   FVector3 v1 = Vec3D::ZeroVector();
   FVector3 v2 = width * widthDirection;
   FVector3 v3 = v2 + height * heightDirection;
   FVector3 v4 = v1 + height * heightDirection;

   Mesh::Clear();

   Mesh::SetColor(color);

   Mesh::AddPosition(v1); Mesh::AddTextureCoordinate( TextureCoordinate(0.0, 0.0) );
   Mesh::AddPosition(v2); Mesh::AddTextureCoordinate( TextureCoordinate(1.0, 0.0) );
   Mesh::AddPosition(v3); Mesh::AddTextureCoordinate( TextureCoordinate(1.0, 1.0) );
   Mesh::AddPosition(v4); Mesh::AddTextureCoordinate( TextureCoordinate(0.0, 1.0) );

   Mesh::AddQuad(MeshVertexIndexer(0, 0, 0, 0),
                 MeshVertexIndexer(1, 1, 0, 0),
                 MeshVertexIndexer(2, 2, 0, 0),
                 MeshVertexIndexer(3, 3, 0, 0));

   Moveable::Reset(lowerLeftCorner);
}

}