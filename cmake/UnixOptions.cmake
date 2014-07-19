###########################################################################################################
#                                                                                                         #
#    This file is part of the Locus Game Engine                                                           #
#                                                                                                         #
#    Copyright (c) 2014 Shachar Avni. All rights reserved.                                                #
#                                                                                                         #
#    Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    #
#                                                                                                         #
###########################################################################################################

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

    if(SetVisibility)
        if (BUILD_SHARED_LIBS)
            add_definitions(-fvisibility=hidden)
        endif()
    endif()
endif()
endfunction()
