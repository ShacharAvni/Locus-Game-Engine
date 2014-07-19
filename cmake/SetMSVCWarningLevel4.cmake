###########################################################################################################
#                                                                                                         #
#    This file is part of the Locus Game Engine                                                           #
#                                                                                                         #
#    Copyright (c) 2014 Shachar Avni. All rights reserved.                                                #
#                                                                                                         #
#    Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    #
#                                                                                                         #
###########################################################################################################

if (MSVC)
	set(CMAKE_CXX_WARNING_LEVEL 4)

	foreach (flag CMAKE_CXX_FLAGS
                  CMAKE_CXX_FLAGS_DEBUG
                  CMAKE_CXX_FLAGS_RELEASE
                  CMAKE_CXX_FLAGS_MINSIZEREL
                  CMAKE_CXX_FLAGS_RELWITHDEBINFO)

		if(${flag} MATCHES "/W[0-4]")
			string(REGEX REPLACE "/W[0-4]" "/W4" ${flag} "${${flag}}")
		else(${flag} MATCHES "/W[0-4]")
			set(${flag} "${${flag}} /W4")
		endif(${flag} MATCHES "/W[0-4]")
    endforeach()
endif()