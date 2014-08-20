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

#include "Locus/Common/Float.h"
#include "Locus/Common/Util.h"

#include "Geometry.h"
#include "PointCloud.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Triangle.h"
#include "Line.h"
#include "LineSegment.h"
#include "Triangulation.h"

#include <algorithm>
#include <array>
#include <forward_list>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <functional>
#include <type_traits>

#include <cstdint>
#include <cassert>

namespace Locus
{

typedef std::pair<std::size_t, std::size_t> ModelEdge_t;

}

namespace std
{

template <> struct hash<Locus::ModelEdge_t>
{
   std::size_t operator()(const Locus::ModelEdge_t& edge) const
   {
      return hash<std::size_t>()(edge.first) ^ hash<std::size_t>()(edge.second);
   }
};

}

namespace Locus
{

struct ModelVertexIndexer
{
   ModelVertexIndexer()
      : positionID(0)
   {
   }

   ModelVertexIndexer(std::size_t positionID)
      : positionID(positionID)
   {
   }

   bool operator <(const ModelVertexIndexer& other) const
   {
      return positionID < other.positionID;
   }

   std::size_t positionID;
};

struct ModelVertex
{
   void Interpolate(const Vector3& interpolatePosition, const ModelVertex& /*faceVertex1*/, const ModelVertex& /*faceVertex2*/, const ModelVertex& /*faceVertex3*/, const Vector3& /*barycentricCoordinates*/)
   {
      position = interpolatePosition;
   }

   bool operator<(const ModelVertex& other) const
   {
      return position < other.position;
   }

   bool operator!=(const ModelVertex& other) const
   {
      return position != other.position;
   }

   Vector3 position;
};

template <class VertexIndexerType = ModelVertexIndexer, class VertexType = ModelVertex>
class Model : public PointCloud
{
public:
   typedef std::vector<VertexIndexerType> face_t;

   static_assert(std::is_base_of<ModelVertexIndexer, VertexIndexerType>::value, "VertexIndexerType must derive from ModelVertexIndexer");
   static_assert(std::is_base_of<ModelVertex, VertexType>::value, "VertexType must derive from ModelVertex");

   Model()
      : numTotalVertices(0)
   {
   }

   Model(const std::vector<std::vector<VertexType>>& faceTriangles)
   {
      std::vector<std::vector<VertexType>> faceTrianglesActual;
      Model::GetTriangles(faceTriangles, faceTrianglesActual);

      Construct(faceTrianglesActual);
   }

   virtual ~Model()
   {
   }

   std::size_t NumFaces() const
   {
      return faces.size();
   }

   std::vector<VertexType> GetFace(std::size_t faceIndex) const
   {
      return GetFace(faceIndex, CurrentModelTransformation());
   }

   std::vector<VertexType> GetFace(std::size_t faceIndex, const Transformation& modelTransformation) const
   {
      assert( faceIndex < faces.size() );

      std::vector<VertexType> face(3);

      for (int i = 0; i < 3; ++i)
      {
         face[i] = GetVertex(faceIndex, i);
         face[i].position = modelTransformation.MultVertex(positions[faces[faceIndex][i].positionID]);
      }

      return face;
   }

   Triangle3D_t GetFaceTriangle(std::size_t faceIndex) const
   {
      return GetFaceTriangle(faceIndex, CurrentModelTransformation());
   }

   Triangle3D_t GetFaceTriangle(std::size_t faceIndex, const Transformation& modelTransformation) const
   {
      assert( faceIndex < faces.size() );
         
      return Triangle3D_t
      (
         modelTransformation.MultVertex(positions[faces[faceIndex][0].positionID]),
         modelTransformation.MultVertex(positions[faces[faceIndex][1].positionID]),
         modelTransformation.MultVertex(positions[faces[faceIndex][2].positionID])
      );
   }

   std::vector<Triangle3D_t> GetIdentityFaceTriangles() const
   {
      std::size_t numFaces = faces.size();

      std::vector<Triangle3D_t> identityFaceTriangles;
      identityFaceTriangles.reserve(numFaces);

      for (std::size_t faceIndex = 0; faceIndex < numFaces; ++faceIndex)
      {
         identityFaceTriangles.push_back
         (
            Triangle3D_t
            (
               positions[faces[faceIndex][0].positionID],
               positions[faces[faceIndex][1].positionID],
               positions[faces[faceIndex][2].positionID]
            )
         );
      }

      return identityFaceTriangles;
   }

   void AddFace(const face_t& face)
   {
      numTotalVertices += face.size();

      faces.push_back(face);
   }

   void AddTriangle(VertexIndexerType v1, VertexIndexerType v2, VertexIndexerType v3)
   {
      numTotalVertices += 3;

      face_t face(3);
      face[0] = v1;
      face[1] = v2;
      face[2] = v3;

      faces.push_back(face);
   }

   void AddQuad(VertexIndexerType v1, VertexIndexerType v2, VertexIndexerType v3, VertexIndexerType v4)
   {
      numTotalVertices += 6;

      face_t face(3);
      face[0] = v1;
      face[1] = v2;
      face[2] = v3;

      faces.push_back(face);

      face[0] = v1;
      face[1] = v3;
      face[2] = v4;

      faces.push_back(face);
   }

   virtual void Clear()
   {
      PointCloud::Clear();

      ClearAndShrink(faces);

      edgeIndexMap.clear();

      ClearAndShrink(edgeAdjacency);

      numTotalVertices = 0;
   }

   void GetIntersection(const Model<VertexIndexerType,VertexType>& other, std::vector<std::vector<Vector3>>& intersectionPoints,  std::vector<std::vector<Vector3>>& intersectionTriangles) const
   {
      std::vector<Vector3> thisTransformedPositions = GetTransformedPositions();
      std::vector<Vector3> otherTransformedPositions = other.GetTransformedPositions();

      std::vector<Vector3> individualIntersection;
      std::vector<Vector3> thisTrianglePoints(3);
      std::vector<Vector3> otherTrianglePoints(3);

      for (const face_t& face : faces)
      {
         Triangle3D_t thisTriangle(thisTransformedPositions[face[0].positionID],
                                    thisTransformedPositions[face[1].positionID],
                                    thisTransformedPositions[face[2].positionID]);

         for (const face_t& otherFace : other.faces)
         {
            Triangle3D_t otherTriangle(otherTransformedPositions[otherFace[0].positionID],
                                       otherTransformedPositions[otherFace[1].positionID],
                                       otherTransformedPositions[otherFace[2].positionID]);

            IntersectionType intersectionType = thisTriangle.TriangleIntersection(otherTriangle, individualIntersection);

            if (intersectionType != IntersectionType::None)
            {
               intersectionPoints.push_back(individualIntersection);

               thisTrianglePoints[0] = thisTriangle[0];
               thisTrianglePoints[1] = thisTriangle[1];
               thisTrianglePoints[2] = thisTriangle[2];

               otherTrianglePoints[0] = otherTriangle[0];
               otherTrianglePoints[1] = otherTriangle[1];
               otherTrianglePoints[2] = otherTriangle[2];

               intersectionTriangles.push_back(thisTrianglePoints);
               intersectionTriangles.push_back(otherTrianglePoints);
            }
         }
      }
   }

   bool GetIntersection(const Model<VertexIndexerType, VertexType>& other, Triangle3D_t* intersectingTriangle1, Triangle3D_t* intersectingTriangle2) const
   {
      std::vector<Vector3> thisTransformedPositions = GetTransformedPositions();
      std::vector<Vector3> otherTransformedPositions = other.GetTransformedPositions();

      for (const face_t& face : faces)
      {
         Triangle3D_t thisTriangle(thisTransformedPositions[face[0].positionID],
                                    thisTransformedPositions[face[1].positionID],
                                    thisTransformedPositions[face[2].positionID]);

         for (const face_t& otherFace : other.faces)
         {
            Triangle3D_t otherTriangle(otherTransformedPositions[otherFace[0].positionID],
                                       otherTransformedPositions[otherFace[1].positionID],
                                       otherTransformedPositions[otherFace[2].positionID]);
               
            if (thisTriangle.TriangleIntersection(otherTriangle))
            {
               if (intersectingTriangle1 != nullptr)
               {
                  *intersectingTriangle1 = thisTriangle;
               }

               if (intersectingTriangle2 != nullptr)
               {
                  *intersectingTriangle2 = otherTriangle;
               }

               return true;
            }
         }
      }

      return false;
   }

   template <class OtherVertexIndexerType, class OtherVertexType>
   bool GetResolvedCollision(const Model<OtherVertexIndexerType,OtherVertexType>& other, const Transformation& thisTransformation, const Transformation& otherTransformation, const std::unordered_set<std::size_t>& thisIntersectionSet, const std::unordered_set<std::size_t>& otherIntersectionSet, Triangle3D_t& thisIntersectingTriangle, Triangle3D_t& otherIntersectingTriangle) const
   {
      std::vector<Triangle3D_t> otherTriangles;
      otherTriangles.reserve(otherIntersectionSet.size());

      for (std::size_t otherTriangleIndex : otherIntersectionSet)
      {
         otherTriangles.push_back(other.GetFaceTriangle(otherTriangleIndex, otherTransformation));
      }

      for (std::size_t thisTriangleIndex : thisIntersectionSet)
      {
         thisIntersectingTriangle = GetFaceTriangle(thisTriangleIndex, thisTransformation);

         for (const Triangle3D_t& otherTriangle : otherTriangles)
         {
            if (thisIntersectingTriangle.TriangleIntersection(otherTriangle))
            {
               otherIntersectingTriangle = otherTriangle;

               return true;
            }
         }
      }

      return false;
   }

   bool IntersectsLine(const Line3D_t& line) const
   {
      std::vector<Vector3> thisTransformedPositions = GetTransformedPositions();

      std::vector<Vector3> intersectionPoints;

      for (const face_t& face : faces)
      {
         Triangle3D_t thisTriangle(thisTransformedPositions[face[0].positionID],
                                    thisTransformedPositions[face[1].positionID],
                                    thisTransformedPositions[face[2].positionID]);

         if (thisTriangle.LineIntersection(line, intersectionPoints) != IntersectionType::None)
         {
            return true;
         }
      }

      return false;
   }

   bool DetermineSplit(const Plane& plane, const Transformation& modelTransformation, Model<VertexIndexerType,VertexType>& modelOnPositiveSide) const
   {
      std::vector<std::vector<VertexType>> splitFaces;

      std::unordered_map<std::size_t, VertexType> edgeToVerticesOnBoundaryMap;

      std::vector<VertexType> newFaceVertices;

      //make the mesh on the positive side of the plane
      for (std::size_t faceIndex = 0, numFaces = faces.size(); faceIndex < numFaces; ++faceIndex)
      {
         std::vector<VertexType> face = GetFace(faceIndex, modelTransformation);
         Triangle3D_t faceTriangle(face[0].position, face[1].position, face[2].position);

         Plane::IntersectionQuery planeIntersectionQuery = plane.triangleIntersectionTest(faceTriangle);

         if (planeIntersectionQuery == Plane::IntersectionQuery::Positive)
         {
            splitFaces.push_back(face);
         }
         else if (planeIntersectionQuery == Plane::IntersectionQuery::None)
         {
            //some points are on the negative side of the plane

            newFaceVertices.clear();
            newFaceVertices.reserve(4);

            for (std::size_t vertexIndex = 0; vertexIndex < Triangle3D_t::NumPointsOnATriangle; ++vertexIndex)
            {
               const VertexType& vertex = face[vertexIndex];
               const VertexType& nextVertex = face[(vertexIndex + 1) % Triangle3D_t::NumPointsOnATriangle];

               LineSegment3D_t edge(vertex.position, nextVertex.position);

               std::function<void()> splitEdgeAndAddVertexToFace = [&]()
               {
                  std::unordered_map<ModelEdge_t, std::size_t>::const_iterator edgeIndexFind = edgeIndexMap.find( MakeEdge(faceIndex, vertexIndex) );
                  std::size_t edgeIndex = edgeIndexFind->second;

				  #ifdef __GNUC__
				     typename
			      #endif
                  std::unordered_map<std::size_t, VertexType>::iterator edgeToVertexFind = edgeToVerticesOnBoundaryMap.find(edgeIndex);

                  if (edgeToVertexFind != edgeToVerticesOnBoundaryMap.end())
                  {
                     newFaceVertices.push_back(edgeToVertexFind->second);
                  }
                  else
                  {
                     VertexType intersectionVertex;
                     if (SplitEdge(edge, plane, faceTriangle, face, intersectionVertex))
                     {
                        newFaceVertices.push_back(intersectionVertex);
                        edgeToVerticesOnBoundaryMap.insert( std::make_pair<std::size_t, VertexType>(std::move(edgeIndex), std::move(intersectionVertex)) );
                     }
                  }
               };

               if (plane.signedDistanceTo(vertex.position) > 0.0f)
               {
                  newFaceVertices.push_back(vertex);

                  if (plane.signedDistanceTo(nextVertex.position) < 0.0f)
                  {
                     splitEdgeAndAddVertexToFace();
                  }
               }
               else
               {
                  if (plane.signedDistanceTo(nextVertex.position) > 0.0f)
                  {
                     splitEdgeAndAddVertexToFace();
                  }
               }
            }

            std::vector<std::vector<VertexType>> newFaceVerticesAsTriangles;
            Model::GetTriangles(newFaceVertices, newFaceVerticesAsTriangles);

            for (const std::vector<VertexType>& triangle : newFaceVerticesAsTriangles)
            {
               splitFaces.push_back(triangle);
            }
         }
      }

      ClearAndShrink(newFaceVertices);

      std::vector<std::vector<VertexType>> connectedComponents;
      if (!DetermineConnectedComponents(edgeToVerticesOnBoundaryMap, connectedComponents))
      {
         return false;
      }

      edgeToVerticesOnBoundaryMap.clear();

      CollectBoundaryFaces(connectedComponents, -plane.getNormal(), splitFaces);

      ClearAndShrink(connectedComponents);

      modelOnPositiveSide.Construct(splitFaces);

      return true;
   }

   void DetermineSplit(const Plane& plane, const Transformation& modelTransformation, Model<VertexIndexerType,VertexType>& modelOnPositiveSide, Model<VertexIndexerType,VertexType>& modelOnNegativeSide) const
   {
      DetermineSplit(plane, modelTransformation, modelOnPositiveSide);
      DetermineSplit(Plane(plane.P, -plane.getNormal()), modelTransformation, modelOnNegativeSide);
   }

   void UpdateEdgeAdjacency()
   {
      edgeIndexMap.clear();

      std::unordered_set<ModelEdge_t> uniqueEdgeSet;

      for (std::size_t thisFaceIndex = 0, numFaces = faces.size(); thisFaceIndex < numFaces; ++thisFaceIndex)
      {
         for (std::size_t vertexIndex = 0; vertexIndex < Triangle3D_t::NumPointsOnATriangle; ++vertexIndex)
         {
            uniqueEdgeSet.insert( MakeEdge(thisFaceIndex, vertexIndex) );
         }
      }

      std::size_t edgeIndex = 0;
      for (const ModelEdge_t& edge : uniqueEdgeSet)
      {
         edgeIndexMap.insert( std::pair<ModelEdge_t, std::size_t>(edge, edgeIndex) );
         ++edgeIndex;
      }

      std::array<std::size_t, Max_Adjacent_Edges> initialAdjacencyForEdge = {No_Adjacency, No_Adjacency, No_Adjacency, No_Adjacency};
      edgeAdjacency.resize(uniqueEdgeSet.size(), initialAdjacencyForEdge);

      for (std::size_t thisFaceIndex = 0, numFaces = faces.size(); thisFaceIndex < numFaces; ++thisFaceIndex)
      {
         std::size_t edgeIndex1 = edgeIndexMap[ MakeEdge(thisFaceIndex, 0) ];
         std::size_t edgeIndex2 = edgeIndexMap[ MakeEdge(thisFaceIndex, 1) ];
         std::size_t edgeIndex3 = edgeIndexMap[ MakeEdge(thisFaceIndex, 2) ];

         AddAdjacentEdge(edgeIndex1, edgeIndex2);
         AddAdjacentEdge(edgeIndex1, edgeIndex3);

         AddAdjacentEdge(edgeIndex2, edgeIndex1);
         AddAdjacentEdge(edgeIndex2, edgeIndex3);

         AddAdjacentEdge(edgeIndex3, edgeIndex1);
         AddAdjacentEdge(edgeIndex3, edgeIndex2);
      }
   }

   void Triangulate()
   {
      numTotalVertices = 0;

      for (std::size_t iFace = 0, numFaces = faces.size(); iFace < numFaces; ++iFace)
      {
         if (faces[iFace].size() > 3)
         {
            face_t originalVertices = faces[iFace];

            std::size_t originalSize = originalVertices.size();

            std::size_t numTriangles = (originalSize - 4 + 2);

            face_t triangleVertices(3);

            std::size_t triangleIndeces[] = {0, 1, 2};

            std::size_t iTriangle = 0;

            do
            {
               triangleVertices[0] = originalVertices[triangleIndeces[0]];
               triangleVertices[1] = originalVertices[triangleIndeces[1]++];
               triangleVertices[2] = originalVertices[triangleIndeces[2]++];

               if (iTriangle == 0)
               {
                  faces[iFace] = triangleVertices;
               }
               else
               {
                  faces.push_back(triangleVertices);
               }

               ++iTriangle;
            } while (triangleIndeces[2] < originalSize);

            numTotalVertices += numTriangles * 3;
         }
         else
         {
            numTotalVertices += 3;
         }
      }
   }

   void ScaleModel(float scale)
   {
      if (Float::NotEqual<float>(scale, 1.0f))
      {
         for (Vector3& position : positions)
         {
            position *= scale;
         }
      }
   }

   void ToModel()
   {
      for (Vector3& position : positions)
      {
         position -= centroid;
      }
   }

protected:
   static const std::size_t Max_Adjacent_Edges = 4;

   std::vector<face_t> faces;

   std::unordered_map<ModelEdge_t, std::size_t> edgeIndexMap; //note: max number of faces: SIZE_MAX - 1
   std::vector< std::array<std::size_t, Max_Adjacent_Edges> > edgeAdjacency;

   std::size_t numTotalVertices;

   static const std::size_t No_Adjacency = SIZE_MAX;

   virtual VertexType GetVertex(std::size_t /*faceIndex*/, int /*vertexIndex*/) const
   {
      return VertexType();
   }

   static void GetTriangles(const std::vector<std::vector<VertexType>>& faceCoords, std::vector<std::vector<VertexType>>& triangles)
   {
      for (const std::vector<VertexType>& face : faceCoords)
      {
         Model::GetTriangles(face, triangles);
      }
   }

   static void GetTriangles(const std::vector<VertexType>& face, std::vector<std::vector<VertexType>>& faceTriangles)
   {
      std::size_t numPoints = face.size();

      std::size_t triangleIndeces[] = {0, 1, 2};

      std::vector<VertexType> faceTriangle(3);
      do
      {
         faceTriangle[0] = face[triangleIndeces[0]];
         faceTriangle[1] = face[triangleIndeces[1]++];
         faceTriangle[2] = face[triangleIndeces[2]++];

         faceTriangles.push_back(faceTriangle);
      } while (triangleIndeces[2] < numPoints);
   }

   bool IsDegenerateFace(const face_t& face)
   {
      return ( (face[0].positionID == face[1].positionID) ||
               (face[0].positionID == face[2].positionID) ||
               (face[1].positionID == face[2].positionID) ||
               !Triangle3D_t::IsValidTriangle(positions[face[0].positionID], positions[face[1].positionID], positions[face[2].positionID], NO_TOLERANCE) );
   }

   virtual void Construct(const std::vector<std::vector<VertexType>>& faceTriangles, std::vector<std::size_t>* degenerateFaceIndices = nullptr)
   {
      positions.clear();
      faces.clear();

      numTotalVertices = 0;

      if (degenerateFaceIndices != nullptr)
      {
         degenerateFaceIndices->clear();
      }

      std::size_t numFaces = faceTriangles.size();

      std::vector<Vector3> vertPositions;
      vertPositions.reserve(numFaces * Triangle3D_t::NumPointsOnATriangle);

      for (const std::vector<VertexType>& triangle : faceTriangles)
      {
         for (std::size_t i = 0; i < Triangle3D_t::NumPointsOnATriangle; ++i)
         {
            vertPositions.push_back(triangle[i].position);
         }
      }

      //NOTE: may cause degenerate faces
      std::vector<std::size_t> sortedPositionIndices;
      GetUniqueItems<Vector3>(vertPositions, positions, sortedPositionIndices);

      //construct faces
      std::size_t numDegenerateFaces = 0;

      VertexIndexerType vertex;
      face_t face(Triangle3D_t::NumPointsOnATriangle);

      faces.reserve(numFaces);

      std::vector<face_t> degenerateFaces;
      degenerateFaces.reserve(numFaces);

      for (std::size_t faceIndex = 0; faceIndex < numFaces; ++faceIndex)
      {
         for (std::size_t i = 0; i < Triangle3D_t::NumPointsOnATriangle; ++i)
         {
            vertex.positionID = sortedPositionIndices[numTotalVertices];

            face[i] = vertex;
            ++numTotalVertices;
         }

         if (!IsDegenerateFace(face))
         {
            faces.push_back(face);
         }
         else
         {
            ++numDegenerateFaces;

            degenerateFaces.push_back(face);

            if (degenerateFaceIndices != nullptr)
            {
               degenerateFaceIndices->push_back(faceIndex);
            }
         }
      }

      numTotalVertices -= numDegenerateFaces * Triangle3D_t::NumPointsOnATriangle;

      ComputeCentroid();
      ToModel();

      UpdateEdgeAdjacency();
   }

private:
   ModelEdge_t MakeEdge(std::size_t faceIndex, std::size_t vertexIndex) const
   {
      ModelEdge_t edge;

      edge.first = faces[faceIndex][vertexIndex].positionID;
      edge.second = faces[faceIndex][(vertexIndex + 1) % Triangle3D_t::NumPointsOnATriangle].positionID;

      if (edge.first > edge.second)
      {
         std::swap(edge.first, edge.second);
      }

      return edge;
   }

   void AddAdjacentEdge(std::size_t edgeIndex, std::size_t adjacentEdgeIndex)
   {
      for (std::size_t i = 0; i < Max_Adjacent_Edges; ++i)
      {
         if (edgeAdjacency[edgeIndex][i] == No_Adjacency)
         {
            edgeAdjacency[edgeIndex][i] = adjacentEdgeIndex;
            break;
         }
      }
   }

   bool EdgesAreAdjacent(std::size_t edgeIndex1, std::size_t edgeIndex2) const
   {
      if (edgeIndex1 == edgeIndex2)
      {
         return true;
      }
      else
      {
         for (std::size_t checkEdge = 0; checkEdge < Max_Adjacent_Edges; ++checkEdge)
         {
            if (edgeAdjacency[edgeIndex1][checkEdge] == edgeIndex2)
            {
               return true;
            }
         }

         return false;
      }
   }

   bool DetermineConnectedComponents(const std::unordered_map<std::size_t, VertexType>& edgeToIndexMap, std::vector<std::vector<VertexType>>& connectedComponents) const
   {
      std::size_t numVertices = edgeToIndexMap.size();

      if (numVertices > 0)
      {
         std::vector<VertexType> vertices(numVertices);
         std::vector<std::size_t> edgesForVertices(numVertices);

         {
            std::size_t vertexIndex = 0;
            for (const typename std::unordered_map<std::size_t, VertexType>::value_type& edgeVertexPair : edgeToIndexMap)
            {
               vertices[vertexIndex] = edgeVertexPair.second;
               edgesForVertices[vertexIndex] = edgeVertexPair.first;

               ++vertexIndex;
            }
         }

         std::vector<std::vector<std::size_t>> connectedComponentsAsIndices;
         connectedComponentsAsIndices.push_back( std::vector<std::size_t>(1, 0) );

         std::vector<std::size_t> componentsVertexIsConnectedTo;
         for (std::size_t vertexIndex = 1; vertexIndex < numVertices; ++vertexIndex)
         {
            std::size_t numComponents = connectedComponentsAsIndices.size();

            componentsVertexIsConnectedTo.clear();
            componentsVertexIsConnectedTo.reserve(numComponents);

            for (std::size_t componentIndex = 0; componentIndex < numComponents; ++componentIndex)
            {
               for (std::size_t checkVertexIndex = 0, numVerticesInConnectedComponent = connectedComponentsAsIndices[componentIndex].size(); checkVertexIndex < numVerticesInConnectedComponent; ++checkVertexIndex)
               {
                  if (EdgesAreAdjacent(edgesForVertices[vertexIndex], edgesForVertices[ connectedComponentsAsIndices[componentIndex][checkVertexIndex] ]))
                  {
                     componentsVertexIsConnectedTo.push_back(componentIndex);
                     break;
                  }
               }
            }

            std::size_t numConnected = componentsVertexIsConnectedTo.size();

            if (numConnected == 0)
            {
               connectedComponentsAsIndices.push_back( std::vector<std::size_t>(1, vertexIndex) );
            }
            else if (numConnected == 1)
            {
               connectedComponentsAsIndices[ componentsVertexIsConnectedTo[0] ].push_back(vertexIndex);
            }
            else
            {
               std::vector<std::size_t>& firstConnectedComponent = connectedComponentsAsIndices[ componentsVertexIsConnectedTo[0] ];
               for (std::size_t connectedComponentIndex = 1; connectedComponentIndex < numConnected; ++connectedComponentIndex)
               {
                  std::vector<std::size_t>& connectedConnectedComponent = connectedComponentsAsIndices[componentsVertexIsConnectedTo[connectedComponentIndex]];
                  firstConnectedComponent.insert(firstConnectedComponent.end(), connectedConnectedComponent.begin(), connectedConnectedComponent.end());

                  connectedConnectedComponent.clear();
               }

               firstConnectedComponent.push_back(vertexIndex);
            }
         }

         ClearAndShrink(componentsVertexIsConnectedTo);

         std::vector<std::vector<VertexType>> connectedComponentsUnordered;
         std::vector<std::vector<std::size_t>> connectedComponentsAsIndicesUnordered;

         for (const std::vector<std::size_t>& connectedComponent : connectedComponentsAsIndices)
         {
            std::size_t numVerticesInComponent = connectedComponent.size();

            if (numVerticesInComponent > 0)
            {
               std::vector<VertexType> currentComponent(numVerticesInComponent);
               std::vector<std::size_t> currentComponentAsIndices(numVerticesInComponent);

               for (std::size_t vertexIndexIndex = 0; vertexIndexIndex < numVerticesInComponent; ++vertexIndexIndex)
               {
                  currentComponent[vertexIndexIndex] = vertices[ connectedComponent[vertexIndexIndex] ];
                  currentComponentAsIndices[vertexIndexIndex] = connectedComponent[vertexIndexIndex];
               }

               connectedComponentsUnordered.push_back( std::move(currentComponent) );
               connectedComponentsAsIndicesUnordered.push_back( std::move(currentComponentAsIndices) );
            }
         }

         ClearAndShrink(connectedComponentsAsIndices);

         std::size_t numConnectedComponents = connectedComponentsUnordered.size();

         connectedComponents.resize(numConnectedComponents);

         for (std::size_t connectedComponentIndex = 0; connectedComponentIndex < numConnectedComponents; ++connectedComponentIndex)
         {
            std::size_t numVerticesInConnectedComponent = connectedComponentsUnordered[connectedComponentIndex].size();

            connectedComponents[connectedComponentIndex].resize(numVerticesInConnectedComponent);

            connectedComponents[connectedComponentIndex][0] = connectedComponentsUnordered[connectedComponentIndex][0];

            if (numVerticesInConnectedComponent > 1)
            {
               for (std::size_t currentIndex = 0; currentIndex < (numVerticesInConnectedComponent - 1); ++currentIndex)
               {
                  bool missed = true;

                  for (std::size_t checkIndex = currentIndex + 1; checkIndex < numVerticesInConnectedComponent; ++checkIndex)
                  {
                     if (EdgesAreAdjacent(edgesForVertices[ connectedComponentsAsIndicesUnordered[connectedComponentIndex][currentIndex] ], edgesForVertices[ connectedComponentsAsIndicesUnordered[connectedComponentIndex][checkIndex] ]))
                     {
                        connectedComponents[connectedComponentIndex][currentIndex + 1] = connectedComponentsUnordered[connectedComponentIndex][checkIndex];

                        if (currentIndex + 1 != checkIndex)
                        {
                           std::swap(connectedComponentsUnordered[connectedComponentIndex][currentIndex + 1], connectedComponentsUnordered[connectedComponentIndex][checkIndex]);
                           std::swap(connectedComponentsAsIndicesUnordered[connectedComponentIndex][currentIndex + 1], connectedComponentsAsIndicesUnordered[connectedComponentIndex][checkIndex]);
                        }

                        missed = false;
                        break;
                     }
                  }

                  if (missed)
                  {
                     return false;
                  }
               }
            }
         }
      }

      return true;
   }

   static void DetermineUniquePointsToKeepInConnectedComponents(const std::vector<std::vector<VertexType>>& connectedComponents, std::vector<std::vector<bool>>& keepPoints)
   {
      std::size_t numConnectedComponents = connectedComponents.size();

      keepPoints.clear();
      keepPoints.resize(numConnectedComponents);

      struct PointCheck
      {
         const VertexType* vertex;
         std::size_t locationInComponent;

         bool operator <(const PointCheck& otherPoint) const
         {
            return vertex->position < otherPoint.vertex->position;
         }
      };

      for (std::size_t connectedComponentIndex = 0; (connectedComponentIndex < numConnectedComponents); ++connectedComponentIndex)
      {
         std::size_t numPointsInComponent = connectedComponents[connectedComponentIndex].size();

         if (numPointsInComponent > 0)
         {
            keepPoints[connectedComponentIndex].resize(numPointsInComponent, true);

            if (numPointsInComponent > 1)
            {
               std::vector<PointCheck> checkPointsInComponent(numPointsInComponent);

               for (std::size_t pointIndex = 0; pointIndex < numPointsInComponent; ++pointIndex)
               {
                  checkPointsInComponent[pointIndex].vertex = &connectedComponents[connectedComponentIndex][pointIndex];
                  checkPointsInComponent[pointIndex].locationInComponent = pointIndex;
               }

               std::sort(checkPointsInComponent.begin(), checkPointsInComponent.end());

               std::forward_list<std::size_t> listOfSameVertices;
               std::size_t numSameVertices = 1;
               listOfSameVertices.push_front(checkPointsInComponent[0].locationInComponent);

               auto FindMaxUniqueVertexAndDiscardRest = [&keepPoints, &connectedComponentIndex](const std::forward_list<std::size_t>& listOfSameVertices)
               {
                  std::size_t maxLocation = (*listOfSameVertices.begin());

                  for (std::size_t location : listOfSameVertices)
                  {
                     if (location > maxLocation)
                     {
                        maxLocation = location;
                     }
                  }

                  for (std::size_t location : listOfSameVertices)
                  {
                     if (location < maxLocation)
                     {
                        keepPoints[connectedComponentIndex][location] = false;
                     }
                  }
               };

               for (std::size_t pointIndex = 1; pointIndex < numPointsInComponent; ++pointIndex)
               {
                  if (! (checkPointsInComponent[pointIndex].vertex)->position.PreciselyEqualTo((checkPointsInComponent[pointIndex - 1].vertex)->position) )
                  {
                     if (numSameVertices > 1)
                     {
                        FindMaxUniqueVertexAndDiscardRest(listOfSameVertices);
                     }

                     listOfSameVertices.clear();
                     numSameVertices = 0;
                  }

                  listOfSameVertices.push_front(checkPointsInComponent[pointIndex].locationInComponent);
                  ++numSameVertices;
               }

               if (numSameVertices > 1)
               {
                  FindMaxUniqueVertexAndDiscardRest(listOfSameVertices);
               }
            }
         }
      }
   }

   static void CollectBoundaryFaces(const std::vector<std::vector<VertexType>>& connectedComponents, const Vector3& normal, std::vector<std::vector<VertexType>>& faces)
   {
      std::size_t numConnectedComponents = connectedComponents.size();

      if (numConnectedComponents > 0)
      {
         std::vector<std::vector<bool>> keepPoints;
         DetermineUniquePointsToKeepInConnectedComponents(connectedComponents, keepPoints);

         std::array<Vector3, 2> coordSystemPoints;

         std::size_t numCoordSystemPoints = 0;

         for (std::size_t connectedComponentIndex = 0; (connectedComponentIndex < numConnectedComponents) && (numCoordSystemPoints < 2); ++connectedComponentIndex)
         {
            for (std::size_t pointIndex = 0, numPointsInComponent = connectedComponents[connectedComponentIndex].size(); (pointIndex < numPointsInComponent) && (numCoordSystemPoints < 2); ++pointIndex)
            {
               if (keepPoints[connectedComponentIndex][pointIndex])
               {
                  coordSystemPoints[numCoordSystemPoints++] = connectedComponents[connectedComponentIndex][pointIndex].position;
               }
            }
         }

         if (numCoordSystemPoints == 2)
         {
            Vector3 xAxis = coordSystemPoints[1] - coordSystemPoints[0];
            xAxis.normalize();

            Vector3 yAxis = xAxis;
            yAxis.rotateAround(normal, 90 * TO_RADIANS);

            std::vector<Polygon2D_t> polygons(numConnectedComponents);

            struct KeyEquality
            {
               bool operator()(const Vector2& first, const Vector2& second) const
               {
                  return first.PreciselyEqualTo(second);
               }
            };

            std::unordered_map<Vector2, const VertexType*, std::hash<Vector2>, KeyEquality> twoDToThreeDLookUpTable;

            for (std::size_t connectedComponentIndex = 0; connectedComponentIndex < numConnectedComponents; ++connectedComponentIndex)
            {
               for (std::size_t vertexIndex = 0, numVerticesInComponent = connectedComponents[connectedComponentIndex].size(); vertexIndex < numVerticesInComponent; ++vertexIndex)
               {
                  if (keepPoints[connectedComponentIndex][vertexIndex])
                  {
                     Vector3 pointInCoordSystem = connectedComponents[connectedComponentIndex][vertexIndex].position - coordSystemPoints[0];

                     Vector2 pointOnPlane(pointInCoordSystem.dot(xAxis), pointInCoordSystem.dot(yAxis));

                     polygons[connectedComponentIndex].AddPoint(pointOnPlane);

                     twoDToThreeDLookUpTable[pointOnPlane] = &(connectedComponents[connectedComponentIndex][vertexIndex]);
                  }
               }
            }

            std::vector<const Vector2*> triangles;
            Locus::Triangulate(polygons, PolygonWinding::CounterClockwise, triangles);

            std::vector<VertexType> triangleToAdd(3);

            for (std::size_t triangleIndex = 0, numTriangles = triangles.size() / 3; triangleIndex < numTriangles; ++triangleIndex)
            {
               for (std::size_t pointIndex = 0; pointIndex < Triangle2D_t::NumPointsOnATriangle; ++pointIndex)
               {
                  triangleToAdd[pointIndex] = *twoDToThreeDLookUpTable[*triangles[triangleIndex * 3 + pointIndex]];
               }

               faces.push_back(triangleToAdd);
            }
         }
      }
   }

   static bool SplitEdge(const LineSegment3D_t& edge, const Plane& plane, const Triangle3D_t& faceTriangle, const std::vector<VertexType>& face, VertexType& intersectionVertex)
   {
      Vector3 intersectionPoint;
      if (plane.getLineSegmentIntersection(edge, intersectionPoint) == Plane::IntersectionQuery::Intersects)
      {
         Vector3 barycentric = faceTriangle.ComputeBarycentricCoordinates(intersectionPoint);

         intersectionVertex.Interpolate(intersectionPoint, face[0], face[1], face[2], barycentric);

         return true;
      }

      return false;
   }

};


typedef Model<> Model_t;

}