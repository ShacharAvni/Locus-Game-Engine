 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#include "Locus/Rendering/MeshUtility.h"

#include "Locus/Geometry/ModelUtility.h"
#include "Locus/Geometry/Triangle.h"
#include "Locus/Geometry/Geometry.h"
#include "Locus/Geometry/Plane.h"

#include "Locus/Common/Float.h"

#include <algorithm>

#include <math.h>

namespace Locus
{

TextureCoordinate MeshUtility::SphericalUVMapping(const Vector3& pointOnSphere)
{
   //see http://en.wikipedia.org/wiki/UV_mapping

   Vector3 d = -pointOnSphere;
   d.normalize();

   return TextureCoordinate( 0.5f - ( atan2(d.z, d.x) / (2 * PI) ), 0.5f - 2 * ( asin(d.y) / (2 * PI) ) );
}

void MeshUtility::AddFaceToMesh(Mesh& mesh, std::size_t p1Index, std::size_t p2Index, std::size_t p3Index)
{
   AddFaceToMesh(mesh, p1Index, p2Index, p3Index, p1Index, p2Index, p3Index);
}

void MeshUtility::AddFaceToMesh(Mesh& mesh, std::size_t p1PositionIndex, std::size_t p2PositionIndex, std::size_t p3PositionIndex, std::size_t p1TextureCoordIndex, std::size_t p2TextureCoordIndex, std::size_t p3TextureCoordIndex)
{
   Mesh::face_t face;

   face.push_back( MeshVertexIndexer(p1PositionIndex, p1TextureCoordIndex, 0, 0) );
   face.push_back( MeshVertexIndexer(p2PositionIndex, p2TextureCoordIndex, 0, 0) );
   face.push_back( MeshVertexIndexer(p3PositionIndex, p3TextureCoordIndex, 0, 0) );

   mesh.AddFace(face);
}

//"radius" is the length from the centroid
//to the pointy bits at the end of the octahedron.
//"radius" is not the correct term for this
Mesh MeshUtility::MakeOctahedron(float radius)
{
   Mesh octahedron;

   octahedron.AddPositions( ModelUtility::OctahedronPositions(radius) );

   for (const Vector3& octohedronVertex : octahedron.GetPositions())
   {
      octahedron.AddTextureCoordinate(SphericalUVMapping(octohedronVertex));
   }

   AddFaceToMesh(octahedron, 0, 1, 4);
   AddFaceToMesh(octahedron, 1, 2, 4);
   AddFaceToMesh(octahedron, 2, 3, 4);
   AddFaceToMesh(octahedron, 3, 0, 4);

   AddFaceToMesh(octahedron, 0, 3, 5);
   AddFaceToMesh(octahedron, 3, 2, 5);
   AddFaceToMesh(octahedron, 2, 1, 5);
   AddFaceToMesh(octahedron, 1, 0, 5);

   octahedron.UpdateEdgeAdjacency();

   return octahedron;
}

//"radius" is the length from the centroid to the vertices
//of the Icosahedron. "radius" is not the correct term for this.
Mesh MeshUtility::MakeIcosahedron(float radius)
{
   Mesh icosahedron;

   icosahedron.AddPositions( ModelUtility::IcosahedronPositions(radius) );

   for (const Vector3& icosahedronVertex : icosahedron.GetPositions())
   {
      icosahedron.AddTextureCoordinate(SphericalUVMapping(icosahedronVertex));
   }

   AddFaceToMesh(icosahedron, 10, 9, 2);
   AddFaceToMesh(icosahedron, 10, 1, 9);
   AddFaceToMesh(icosahedron, 10, 4, 1);
   AddFaceToMesh(icosahedron, 10, 7, 4);
   AddFaceToMesh(icosahedron, 10, 2, 7);
   AddFaceToMesh(icosahedron, 4, 0, 1);
   AddFaceToMesh(icosahedron, 4, 11, 0);
   AddFaceToMesh(icosahedron, 4, 7, 11);
   AddFaceToMesh(icosahedron, 9, 1, 5);
   AddFaceToMesh(icosahedron, 9, 5, 6);
   AddFaceToMesh(icosahedron, 9, 6, 2);
   AddFaceToMesh(icosahedron, 8, 3, 6);
   AddFaceToMesh(icosahedron, 8, 6, 5);
   AddFaceToMesh(icosahedron, 8, 5, 0);
   AddFaceToMesh(icosahedron, 3, 7, 2);
   AddFaceToMesh(icosahedron, 8, 11, 3);
   AddFaceToMesh(icosahedron, 8, 0, 11);
   AddFaceToMesh(icosahedron, 6, 3, 2);
   AddFaceToMesh(icosahedron, 0, 5, 1);
   AddFaceToMesh(icosahedron, 3, 11, 7);

   icosahedron.UpdateEdgeAdjacency();

   return icosahedron;
}

Mesh MeshUtility::MakeSphere(float radius, unsigned int subdivisions)
{
   if (subdivisions > ModelUtility::MAX_SPHERE_SUBDIVISIONS)
   {
      subdivisions = ModelUtility::MAX_SPHERE_SUBDIVISIONS;
   }

   Mesh octahedron = MeshUtility::MakeOctahedron(radius);

   std::vector<Triangle3D_t> initialTriangles;

   Transformation identity;

   for (std::size_t iFace = 0, numFaces = octahedron.NumFaces(); iFace < numFaces; ++iFace)
   {
      initialTriangles.push_back(octahedron.GetFaceTriangle(iFace, identity));
   }

   std::vector<Triangle3D_t> subdividedTriangles;

   for (const Triangle3D_t& triangle : initialTriangles)
   {
      ModelUtility::SubdivideTriangle(triangle, subdividedTriangles, subdivisions);
   }

   std::vector<std::vector<MeshVertex>> trianglesOnSphere;
   trianglesOnSphere.reserve(subdividedTriangles.size());

   std::vector<MeshVertex> triangleOnSphere(3);

   Plane fixSmearPlane(Vector3::ZeroVector(), Vector3::NegativeZAxis());

   for (const Triangle3D_t& triangle : subdividedTriangles)
   {
      for (std::size_t i = 0; i < Triangle3D_t::NumPointsOnATriangle; ++i)
      {
         triangleOnSphere[i].position = triangle[i].normVector() * radius;
         triangleOnSphere[i].textureCoordinate = MeshUtility::SphericalUVMapping(triangleOnSphere[i].position);
      }

      //
      //HACK: fixing smearing that occurs at the boundary where U = 0.0 or 1.0. The
      //spherical UV mapping algorithm always returns U = 1.0 at the boundary. Here
      //we find if the triangle that was just generated is at this boundary. The "real"
      //way to do it would be to unwrap the sphere or make a UV sphere.
      //
      Plane::IntersectionQuery intersectionQuery = fixSmearPlane.triangleIntersectionTest(triangle);
      if ((intersectionQuery == Plane::IntersectionQuery::None) || (intersectionQuery == Plane::IntersectionQuery::Positive))
      {
         for (std::size_t i = 0; i < Triangle3D_t::NumPointsOnATriangle; ++i)
         {
            if (Float::FEqual<float>(triangleOnSphere[i].textureCoordinate.x, 1.0f))
            {
               triangleOnSphere[i].textureCoordinate.x = 0.0f;
            }
         }
      }

      trianglesOnSphere.push_back(triangleOnSphere);
   }

   return Mesh(trianglesOnSphere);
}

Mesh MeshUtility::MakeCube(float lengthOfOneSide)
{
   //float_t halfLength = lengthOfOneSide / 2;

   Mesh cube;

   cube.AddPositions( ModelUtility::CubePositions(lengthOfOneSide) );

   cube.AddTextureCoordinate(TextureCoordinate(0, 0));
   cube.AddTextureCoordinate(TextureCoordinate(1, 0));
   cube.AddTextureCoordinate(TextureCoordinate(1, 1));
   cube.AddTextureCoordinate(TextureCoordinate(0, 1));

   //front
   AddFaceToMesh(cube, 0, 1, 2, 0, 1, 2);
   AddFaceToMesh(cube, 0, 2, 3, 0, 2, 3);

   //back
   AddFaceToMesh(cube, 5, 4, 7, 0, 1, 2);
   AddFaceToMesh(cube, 5, 7, 6, 0, 2, 3);

   //right
   AddFaceToMesh(cube, 1, 5, 6, 0, 1, 2);
   AddFaceToMesh(cube, 1, 6, 2, 0, 2, 3);

   //left
   AddFaceToMesh(cube, 4, 0, 3, 0, 1, 2);
   AddFaceToMesh(cube, 4, 3, 7, 0, 2, 3);

   //top
   AddFaceToMesh(cube, 3, 2, 6, 0, 1, 2);
   AddFaceToMesh(cube, 3, 6, 7, 0, 2, 3);

   //bottom
   AddFaceToMesh(cube, 4, 5, 1, 0, 1, 2);
   AddFaceToMesh(cube, 4, 1, 0, 0, 2, 3);

   cube.UpdateEdgeAdjacency();

   return cube;
}

}