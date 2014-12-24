/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/XML/XMLParsing.h"
#include "Locus/XML/XMLTag.h"

#include "Locus/Common/Exception.h"

#include "Locus/FileSystem/FileOnDisk.h"
#include "Locus/FileSystem/File.h"

#include "RapidXML/rapidxml.hpp"

namespace Locus
{

static std::size_t CountAttributes(rapidxml::xml_node<>* node)
{
   std::size_t numAttributes = 0;

   rapidxml::xml_attribute<>* attribute = node->first_attribute();

   while (attribute != nullptr)
   {
      ++numAttributes;

      attribute = attribute->next_attribute();
   }

   return numAttributes;
}

static std::size_t CountSubTags(rapidxml::xml_node<>* node)
{
   std::size_t numSubTags = 0;

   rapidxml::xml_node<>* subNode = node->first_node();

   while (subNode != nullptr)
   {
      ++numSubTags;

      subNode = subNode->next_sibling();
   }

   return numSubTags;
}

static void R_ParseXMLTag(rapidxml::xml_node<>* node, XMLTag& tag)
{
   tag.name = node->name();
   tag.value = node->value();

   std::size_t numAttributes = CountAttributes(node);

   if (numAttributes > 0)
   {
      tag.attributes.resize(numAttributes);

      rapidxml::xml_attribute<>* attribute = node->first_attribute();

      for (XMLAttribute& xmlAttribute : tag.attributes)
      {
         xmlAttribute.name = attribute->name();
         xmlAttribute.value = attribute->value();

         attribute = attribute->next_attribute();
      }
   }
   
   std::size_t numSubTags = CountSubTags(node);

   if (numSubTags > 0)
   {
      tag.subTags.resize(numSubTags);

      rapidxml::xml_node<>* subNode = node->first_node();

      for (XMLTag& subTag : tag.subTags)
      {
         R_ParseXMLTag(subNode, subTag);

         subNode = subNode->next_sibling();
      }
   }
}

void ParseXMLFile(const std::string& fullFilePath, XMLTag& rootTag)
{
   FileOnDisk fileOnDisk(fullFilePath, DataStream::OpenMode::Read);

   ParseXMLFile(fileOnDisk, rootTag);
}

void ParseXMLFile(const MountedFilePath& mountedFilePath, XMLTag& rootTag)
{
   File file(mountedFilePath, DataStream::OpenMode::Read);

   ParseXMLFile(file, rootTag);
}

void ParseXMLFile(DataStream& xmlDataStream, XMLTag& rootTag)
{
   if (!xmlDataStream.Seek(0, DataStream::SeekType::Beginning))
   {
      throw Exception("ParseXMLFile: Failed to seek to the beginning of the data stream");
   }

   std::size_t sizeInBytes = xmlDataStream.SizeInBytes();

   std::vector<char> xmlRawChars(sizeInBytes);

   if (xmlDataStream.Read(xmlRawChars, sizeInBytes, 0) != sizeInBytes)
   {
      throw Exception("ParseXMLFile: Failed to read data stream");
   }

   xmlRawChars.push_back(0);

   rapidxml::xml_document<> xmlDocument;

   try
   {
      xmlDocument.parse<0>(xmlRawChars.data());
   }
   catch (rapidxml::parse_error& parseError)
   {
      throw Exception(std::string("ParseXMLFile: Failed to parse XML data stream. Underlying error: ") + parseError.what());
   }

   rapidxml::xml_node<>* rapidXMLRootNode = xmlDocument.first_node(0);

   if (rapidXMLRootNode == nullptr)
   {
      throw Exception("ParseXMLFile: Failed to find root XML node");
   }

   rootTag.attributes.clear();
   rootTag.subTags.clear();

   R_ParseXMLTag(rapidXMLRootNode, rootTag);
}

}