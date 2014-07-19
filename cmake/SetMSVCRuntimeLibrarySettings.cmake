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
	if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
		foreach (flag CMAKE_CXX_FLAGS
					  CMAKE_CXX_FLAGS_DEBUG
					  CMAKE_CXX_FLAGS_RELEASE
					  CMAKE_CXX_FLAGS_MINSIZEREL
					  CMAKE_CXX_FLAGS_RELWITHDEBINFO)

				if (${flag} MATCHES "/MD")
					string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
				endif()
				if (${flag} MATCHES "/MDd")
					string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
				endif()
		endforeach()
	endif()
endif()