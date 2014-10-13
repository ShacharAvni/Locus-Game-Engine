###########################################################################################################
#                                                                                                         #
#    This file is part of the Locus Game Engine                                                           #
#                                                                                                         #
#    Copyright (c) 2014 Shachar Avni. All rights reserved.                                                #
#                                                                                                         #
#    Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    #
#                                                                                                         #
###########################################################################################################

option(ARCH_32_BIT "Compile as 32 Bit" ON)

macro(SetDefaultCMakeBuildType)
    if(NOT CMAKE_BUILD_TYPE)
        # Set default build type to Release
        set(CMAKE_BUILD_TYPE Release CACHE STRING "CMake Build Type" FORCE)

        # Set the possible values of build type for cmake-gui
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    endif()
endmacro()

function(SetUnixOptions SetCpp11 SetVisibility)
if (UNIX)
   SetDefaultCMakeBuildType()

   add_definitions(-Wall)

   if(SetCpp11)
      add_definitions(-std=c++1y)
   endif()

   if(SetVisibility AND BUILD_SHARED_LIBS)
      add_definitions(-fvisibility=hidden)
   endif()

   if(ARCH_32_BIT)
      add_definitions(-m32)
   else()
      add_definitions(-m64)
   endif()
 
   if(STATIC_C_AND_CXX_RUNTIMES)
      add_definitions(-static-libstdc++)
      add_definitions(-static-libgcc)
   endif()
endif()
endfunction()