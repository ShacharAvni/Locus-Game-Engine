/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/ModelUtility.h"

#include <cmath>

namespace Locus
{

const unsigned int ModelUtility::MAX_SPHERE_SUBDIVISIONS = 7;

//"radius" is the length from the centroid
//to the pointy bits at the end of the octahedron.
//"radius" is not the correct term for this
std::vector<FVector3> ModelUtility::OctahedronPositions(float radius)
{
   return
   {
      {  radius,   0.0f ,   0.0f  },
      {   0.0f ,   0.0f , -radius },
      { -radius,   0.0f ,   0.0f  },
      {   0.0f ,   0.0f ,  radius },
      {   0.0f ,  radius,   0.0f  },
      {   0.0f , -radius,   0.0f  }
   };
}

//"mainLength" is the length from the centroid to the vertices
//of the Icosahedron.
std::vector<FVector3> ModelUtility::IcosahedronPositions(float mainLength)
{
   //'a' and 'b' are half the length and width of a golden rectangle, respectively
   //(when radius is 1). The vertices of three orthogonal golden rectangles form an
   //icosahedron. Here, 'a' and 'b' are set such that the distance between (0, 0, 0)
   //and the extrema of the icosahedron is radius units
   const float a = mainLength * ( std::sqrt(4.0f - (16.0f / (10.0f + 2.0f * std::sqrt(5.0f)))) / 2.0f );
   const float b = mainLength * ( std::sqrt(16.0f / (10.0f + 2.0f * std::sqrt(5.0f))) / 2.0f );

   return
   {
      {  a, -b,  0 },
      {  a,  b,  0 },
      { -a,  b,  0 },
      { -a, -b,  0 },
      {  b,  0,  a },
      {  b,  0, -a },
      { -b,  0, -a },
      { -b,  0,  a },
      {  0, -a, -b },
      {  0,  a, -b },
      {  0,  a,  b },
      {  0, -a,  b }
   };
}

std::vector<FVector3> ModelUtility::CubePositions(float lengthOfOneSide)
{
   const float h = lengthOfOneSide / 2;

   return
   {
      { -h, -h,  h },
      {  h, -h,  h },
      {  h,  h,  h },
      { -h,  h,  h },
      { -h, -h, -h },
      {  h, -h, -h },
      {  h,  h, -h },
      { -h,  h, -h }
   };
}

Model_t ModelUtility::MakeCube(float lengthOfOneSide)
{
   Model_t cube;

   cube.AddPositions( CubePositions(lengthOfOneSide) );

   //front
   cube.AddQuad(0, 1, 2, 3);

   //back
   cube.AddQuad(5, 4, 7, 6);

   //right
   cube.AddQuad(1, 5, 6, 2);

   //left
   cube.AddQuad(4, 0, 3, 7);

   //top
   cube.AddQuad(3, 2, 6, 7);

   //bottom
   cube.AddQuad(4, 5, 1, 0);

   cube.UpdateEdgeAdjacency();

   return cube;
}

Model_t ModelUtility::MakeOctahedron(float radius)
{
   Model_t octahedron;

   octahedron.AddPositions( OctahedronPositions(radius) );

   octahedron.AddTriangle(0, 1, 4);
   octahedron.AddTriangle(1, 2, 4);
   octahedron.AddTriangle(2, 3, 4);
   octahedron.AddTriangle(3, 0, 4);

   octahedron.AddTriangle(0, 3, 5);
   octahedron.AddTriangle(3, 2, 5);
   octahedron.AddTriangle(2, 1, 5);
   octahedron.AddTriangle(1, 0, 5);

   octahedron.UpdateEdgeAdjacency();

   return octahedron;
}

void ModelUtility::SubdivideTriangle(const Triangle3D_t& triangle, std::vector<Triangle3D_t>& triangles, unsigned int subdivisionsLeft)
{
   if (subdivisionsLeft == 0)
   {
      triangles.push_back(triangle);
   }
   else
   {
      FVector3 midpoints[3] = { triangle.MidpointOfEdge(0), triangle.MidpointOfEdge(1), triangle.MidpointOfEdge(2) };

      //Create Triforce, then subdivide each triangle in the Triforce
      ModelUtility::SubdivideTriangle( Triangle3D_t( triangle[0], midpoints[0], midpoints[2]), triangles, subdivisionsLeft - 1 );
      ModelUtility::SubdivideTriangle( Triangle3D_t(midpoints[0],  triangle[1], midpoints[1]), triangles, subdivisionsLeft - 1 );
      ModelUtility::SubdivideTriangle( Triangle3D_t(midpoints[2], midpoints[1],  triangle[2]), triangles, subdivisionsLeft - 1 );
      ModelUtility::SubdivideTriangle( Triangle3D_t(midpoints[0], midpoints[1], midpoints[2]), triangles, subdivisionsLeft - 1 );
   }
}

Model_t ModelUtility::MakeSphere(float radius, unsigned int subdivisions)
{
   if (subdivisions > ModelUtility::MAX_SPHERE_SUBDIVISIONS)
   {
      subdivisions = ModelUtility::MAX_SPHERE_SUBDIVISIONS;
   }

   Model_t octahedron = ModelUtility::MakeOctahedron(radius);

   std::vector<Triangle3D_t> initialTriangles;

   Transformation identity;

   for (std::size_t faceIndex = 0, numFaces = octahedron.NumFaces(); faceIndex < numFaces; ++faceIndex)
   {
      initialTriangles.push_back(octahedron.GetFaceTriangle(faceIndex, identity));
   }

   std::vector<Triangle3D_t> subdividedTriangles;
   subdividedTriangles.reserve( initialTriangles.size() * static_cast<std::size_t>(std::pow(4, subdivisions)) );

   for (const Triangle3D_t& triangle : initialTriangles)
   {
      ModelUtility::SubdivideTriangle(triangle, subdividedTriangles, subdivisions);
   }

   std::vector<std::vector<ModelVertex>> trianglesOnSphere;
   trianglesOnSphere.reserve(subdividedTriangles.size());

   for (const Triangle3D_t& triangle : subdividedTriangles)
   {
      trianglesOnSphere.emplace_back( std::vector<ModelVertex>{ {NormVector(triangle[0]) * radius}, {NormVector(triangle[1]) * radius}, {NormVector(triangle[2]) * radius} } );
   }

   return Model_t(trianglesOnSphere);
}

}