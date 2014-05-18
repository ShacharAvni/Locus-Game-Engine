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

namespace Locus
{

Quad::Quad()
   : Mesh()
{
}

void Quad::Set(const Vector3& lowerLeftCorner, const Vector3& widthDirection, const Vector3& heightDirection, float width, float height, const Color& color)
{
   Vector3 v1 = Vector3::ZeroVector();
   Vector3 v2 = width * widthDirection;
   Vector3 v3 = v2 + height * heightDirection;
   Vector3 v4 = v1 + height * heightDirection;

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