###########################################################################################################
#                                                                                                         #
#    This file is part of the Locus Game Engine                                                           #
#                                                                                                         #
#    Copyright (c) 2014 Shachar Avni. All rights reserved.                                                #
#                                                                                                         #
#    Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    #
#                                                                                                         #
###########################################################################################################

macro(ReplaceMSVCRuntimeFlag Flag)
if(${Flag} MATCHES "/MD")
    string(REGEX REPLACE "/MD" "/MT" ${Flag} "${${Flag}}")
endif()
if(${Flag} MATCHES "/MDd")
    string(REGEX REPLACE "/MDd" "/MTd" ${Flag} "${${Flag}}")
endif()
endmacro()

macro(SetMSVCRuntimeLibrarySettings SetCXXFlags)
if(MSVC AND STATIC_C_AND_CXX_RUNTIMES)
   if(${SetCXXFlags})
      foreach (flag CMAKE_CXX_FLAGS
                    CMAKE_CXX_FLAGS_DEBUG
                    CMAKE_CXX_FLAGS_RELEASE
                    CMAKE_CXX_FLAGS_MINSIZEREL
                    CMAKE_CXX_FLAGS_RELWITHDEBINFO)

         ReplaceMSVCRuntimeFlag(${flag})
      endforeach()
   else()
      foreach (flag CMAKE_C_FLAGS
                    CMAKE_C_FLAGS_DEBUG
                    CMAKE_C_FLAGS_RELEASE
                    CMAKE_C_FLAGS_MINSIZEREL
                    CMAKE_C_FLAGS_RELWITHDEBINFO)

         ReplaceMSVCRuntimeFlag(${flag})
     endforeach()
   endif()
endif()
endmacro()

macro(SetMSVCWarningLevel4)
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
endmacro()