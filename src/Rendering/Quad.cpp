 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

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