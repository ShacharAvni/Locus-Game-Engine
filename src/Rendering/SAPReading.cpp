/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/SAPReading.h"

#include "Locus/Common/Parsing.h"

#include "Locus/FileSystem/FileOnDisk.h"
#include "Locus/FileSystem/File.h"

#include "RapidXML/rapidxml.hpp"

#include <cstdlib>

namespace Locus
{

static const char* const XML_SAP = "SAP";
static const char* const XML_Model = "Model";
static const char* const XML_Positions = "Positions";
static const char* const XML_Position = "Position";
static const char* const XML_TextureCoordinates = "TextureCoordinates";
static const char* const XML_TextureCoordinate = "TextureCoordinate";
static const char* const XML_X = "x";
static const char* const XML_Y = "y";
static const char* const XML_Z = "z";
static const char* const XML_TX = "tx";
static const char* const XML_TY = "ty";
static const char* const XML_Faces = "Faces";
static const char* const XML_Face = "Face";
static const char* const XML_Vertex = "Vertex";
static const char* const XML_PositionIndex = "PositionIndex";
static const char* const XML_TextureCoordinateIndex = "TextureCoordinateIndex";

static bool ParseSAPFile(rapidxml::xml_document<>& xmlDocument, std::vector<Mesh>& meshes);

bool ParseSAPFile(const std::string& fullFilePath, std::vector<Mesh>& meshes)
{
   FileOnDisk fileOnDisk(fullFilePath, DataStream::OpenOperation::Read);

   return ParseSAPFile(fileOnDisk, meshes);
}

bool ParseSAPFile(const MountedFilePath& mountedFilePath, std::vector<Mesh>& meshes)
{
   File file(mountedFilePath, DataStream::OpenOperation::Read);

   return ParseSAPFile(file, meshes);
}

bool ParseSAPFile(DataStream& sapDataStream, std::vector<Mesh>& meshes)
{
   if (!sapDataStream.Seek(0, DataStream::SeekType::Beginning))
   {
      return false;
   }

   std::size_t sizeInBytes = sapDataStream.SizeInBytes();

   std::vector<char> sapXML(sizeInBytes);

   if (sapDataStream.Read(sapXML, sizeInBytes, 0) != sizeInBytes)
   {
      return false;
   }

   sapXML.push_back(0);

   rapidxml::xml_document<> xmlDocument;

   try
   {
      xmlDocument.parse<0>(sapXML.data());
   }
   catch(rapidxml::parse_error&)
   {
      return false;
   }

   return ParseSAPFile(xmlDocument, meshes);
}

static bool ParseSAPFile(rapidxml::xml_document<>& xmlDocument, std::vector<Mesh>& meshes)
{
   #define CHECK_NODE(node) if (node == nullptr) return false;

   rapidxml::xml_node<>* rootNode = xmlDocument.first_node(XML_SAP);
   CHECK_NODE(rootNode)

   rapidxml::xml_node<>* modelNode = rootNode->first_node(XML_Model);
   CHECK_NODE(modelNode)

   //for now, assume parse order is the same as document order

   Vector3 positionFromFile;
   Mesh::face_t faceFromFile;
   TextureCoordinate textureCoordFromFile;

   do
   {
      Mesh mesh;

      rapidxml::xml_node<>* positionsNode = modelNode->first_node(XML_Positions);
      CHECK_NODE(positionsNode)

      rapidxml::xml_node<>* positionNode = positionsNode->first_node(XML_Position);
      CHECK_NODE(positionNode)

      //read positions
      do
      {
         //read X
         rapidxml::xml_node<>* coordNode = positionNode->first_node(XML_X);
         CHECK_NODE(coordNode)
         positionFromFile.x = static_cast<float>( std::atof(coordNode->value()) );

         //read Y
         coordNode = coordNode->next_sibling(XML_Y);
         CHECK_NODE(coordNode)
         positionFromFile.y = static_cast<float>( std::atof(coordNode->value()) );

         //read Z
         coordNode = coordNode->next_sibling(XML_Z);
         CHECK_NODE(coordNode)
         positionFromFile.z = static_cast<float>( std::atof(coordNode->value()) );

         mesh.AddPosition(positionFromFile);

         positionNode = positionNode->next_sibling(XML_Position);

      } while (positionNode != nullptr);

      rapidxml::xml_node<>* textureCoordsNode = modelNode->first_node(XML_TextureCoordinates);
      CHECK_NODE(textureCoordsNode)

      rapidxml::xml_node<>* textureCoordNode = textureCoordsNode->first_node(XML_TextureCoordinate);
      CHECK_NODE(textureCoordNode)

      //read texture coordinates
      do
      {
         //read tx
         rapidxml::xml_node<>* coordNode = textureCoordNode->first_node(XML_TX);
         CHECK_NODE(coordNode)
         textureCoordFromFile.x = static_cast<float>( std::atof(coordNode->value()) );

         //read tY
         coordNode = coordNode->next_sibling(XML_TY);
         CHECK_NODE(coordNode)
         textureCoordFromFile.y = static_cast<float>( std::atof(coordNode->value()) );

         mesh.AddTextureCoordinate(textureCoordFromFile);

         textureCoordNode = textureCoordNode->next_sibling(XML_TextureCoordinate);

      } while (textureCoordNode != nullptr);

      rapidxml::xml_node<>* facesNode = modelNode->first_node(XML_Faces);
      CHECK_NODE(facesNode)

      rapidxml::xml_node<>* faceNode = facesNode->first_node(XML_Face);
      CHECK_NODE(faceNode)

      //read faces
      do
      {
         faceFromFile.clear();

         rapidxml::xml_node<>* vertexNode = faceNode->first_node(XML_Vertex);
         CHECK_NODE(vertexNode)

         do
         {
            rapidxml::xml_node<>* positionIndexNode = vertexNode->first_node(XML_PositionIndex);
            CHECK_NODE(positionIndexNode)
            std::size_t positionIndex = static_cast<std::size_t>(std::stoul(positionIndexNode->value()));

            rapidxml::xml_node<>* textureCoordIndexNode = positionIndexNode->next_sibling(XML_TextureCoordinateIndex);
            CHECK_NODE(textureCoordIndexNode)
            std::size_t textureCoordIndex = static_cast<std::size_t>(std::stoul(textureCoordIndexNode->value()));

            faceFromFile.push_back( MeshVertexIndexer(positionIndex, textureCoordIndex, 0, 0) );

            vertexNode = vertexNode->next_sibling(XML_Vertex);

         } while (vertexNode != nullptr);

         mesh.AddFace(faceFromFile);

         faceNode = faceNode->next_sibling(XML_Face);

      } while (faceNode != nullptr);

      mesh.ComputeCentroid();
      mesh.ToModel();
      mesh.centroid = Vector3::ZeroVector();
      mesh.Triangulate();
      mesh.UpdateEdgeAdjacency();
      mesh.AssignNormals();

      meshes.push_back( std::move(mesh) );

      modelNode = modelNode->next_sibling(XML_Model);

   } while (modelNode != nullptr);

#undef CHECK_NODE

   return true;
}

}