###########################################################################################################
#                                                                                                         #
#    This file is part of the Locus Game Engine                                                           #
#                                                                                                         #
#    Copyright (c) 2014 Shachar Avni. All rights reserved.                                                #
#                                                                                                         #
#    Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    #
#                                                                                                         #
###########################################################################################################

cmake_minimum_required(VERSION 2.8)

include(${PROJECT_SOURCE_DIR}/cmake/GlobalProjectOptions.cmake)

if(BUILD_SHARED_LIBS)
	add_definitions(-DLOCUS_XML_SHARED)
	add_definitions(-DLOCUS_SHARED)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/UnixOptions.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/MSVCOptions.cmake)

SetUnixOptions(TRUE TRUE)
SetMSVCRuntimeLibrarySettings(TRUE)
SetMSVCWarningLevel4()

set(LOCUS_INCLUDE ${PROJECT_SOURCE_DIR}/include)
set(LOCUS_XML_INCLUDE ${LOCUS_INCLUDE}/Locus/XML)
set(THIRD_PARTY_DIR ${LOCUS_DIR}/third-party)

include_directories(${LOCUS_INCLUDE}
                    ${THIRD_PARTY_DIR}/RapidXML/include)

add_library(Locus_XML
            XMLAttribute.cpp
            XMLParsing.cpp
            XMLTag.cpp
            ${LOCUS_XML_INCLUDE}/LocusXMLAPI.h
            ${LOCUS_XML_INCLUDE}/XMLAttribute.h
            ${LOCUS_XML_INCLUDE}/XMLParsing.h
            ${LOCUS_XML_INCLUDE}/XMLTag.h)

target_link_libraries(Locus_XML Locus_Common)
target_link_libraries(Locus_XML Locus_FileSystem)