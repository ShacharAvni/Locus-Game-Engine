###########################################################################################################
#                                                                                                         #
#    This file is part of the Locus Game Engine                                                           #
#                                                                                                         #
#    Copyright (c) 2014 Shachar Avni. All rights reserved.                                                #
#                                                                                                         #
#    Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    #
#                                                                                                         #
###########################################################################################################

if(UNIX)
   if(NOT BUILD_ARCHITECTURE)
        # Set default BUILD_ARCHITECTURE to CurrentPlatformArchitecture
        set(BUILD_ARCHITECTURE CurrentPlatformArchitecture CACHE STRING "Architecture" FORCE)

        # Set the possible values of BUILD_ARCHITECTURE
        set_property(CACHE BUILD_ARCHITECTURE PROPERTY STRINGS "32 Bit" "64 Bit" "CurrentPlatformArchitecture")
    endif()

   if(NOT CMAKE_BUILD_TYPE)
        # Set default CMAKE_BUILD_TYPE to Release
        set(CMAKE_BUILD_TYPE Release CACHE STRING "CMake Build Type" FORCE)

        # Set the possible values of CMAKE_BUILD_TYPE
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    endif()
endif()

function(SetUnixOptions SetCpp11 SetVisibility)
if(UNIX)
   add_definitions(-Wall)

   if(SetCpp11)
      add_definitions(-std=c++1y)
   endif()

   if(SetVisibility AND BUILD_SHARED_LIBS)
      add_definitions(-fvisibility=hidden)
   endif()

   if(BUILD_ARCHITECTURE STREQUAL "32 Bit")
      add_definitions(-m32)
   elseif(BUILD_ARCHITECTURE STREQUAL "64 Bit")
      add_definitions(-m64)
   endif()
 
   if(STATIC_C_AND_CXX_RUNTIMES)
      add_definitions(-static-libgcc)

      if(SetCpp11)
         add_definitions(-static-libstdc++)
      endif()
   endif()
endif()
endfunction()