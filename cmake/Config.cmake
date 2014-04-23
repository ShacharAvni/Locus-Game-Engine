#Copied shamelessly from the SFML build system
#http://www.sfml-dev.org/

#SFML (Simple and Fast Multimedia Library) - Copyright (c) Laurent Gomila

#This software is provided 'as-is', without any express or implied warranty.
#In no event will the authors be held liable for any damages arising from
#the use of this software.

#Permission is granted to anyone to use this software for any purpose,
#including commercial applications, and to alter it and redistribute it
#freely, subject to the following restrictions:

#1. The origin of this software must not be misrepresented; you must not claim
#   that you wrote the original software. If you use this software in a product,
#   an acknowledgment in the product documentation would be appreciated but is
#   not required.

#2. Altered source versions must be plainly marked as such, and must not be
#   misrepresented as being the original software.

#3. This notice may not be removed or altered from any source distribution.

# detect the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS 1)

    # detect the architecture (note: this test won't work for cross-compilation)
    include(CheckTypeSize)
    check_type_size(void* SIZEOF_VOID_PTR)
    if("${SIZEOF_VOID_PTR}" STREQUAL "4")
        set(ARCH_32BITS 1)
    elseif("${SIZEOF_VOID_PTR}" STREQUAL "8")
        set(ARCH_64BITS 1)
    else()
        message(FATAL_ERROR "Unsupported architecture")
        return()
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    # FreeBSD compile path is the same as Linux
    set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX 1)

    # detect OS X version. (use '/usr/bin/sw_vers -productVersion' to extract V from '10.V.x'.)
    EXEC_PROGRAM(/usr/bin/sw_vers ARGS -productVersion OUTPUT_VARIABLE MACOSX_VERSION_RAW)
    STRING(REGEX REPLACE "10\\.([0-9]).*" "\\1" MACOSX_VERSION "${MACOSX_VERSION_RAW}")
    if(${MACOSX_VERSION} LESS 5)
        message(FATAL_ERROR "Unsupported version of OS X : ${MACOSX_VERSION_RAW}")
        return()
    endif()
else()
    message(FATAL_ERROR "Unsupported operating system")
    return()
endif()

# detect the compiler and its version
# Note: on some platforms (OS X), CMAKE_COMPILER_IS_GNUCXX is true
# even when CLANG is used, therefore the Clang test is done first
if(CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   # CMAKE_CXX_COMPILER_ID is an internal CMake variable subject to change,
   # but there is no other way to detect CLang at the moment
   set(COMPILER_CLANG 1)
   execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE CLANG_VERSION_OUTPUT)
   string(REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+).*" "\\1" CLANG_VERSION "${CLANG_VERSION_OUTPUT}")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(COMPILER_GCC 1)
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
    string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" GCC_VERSION "${GCC_VERSION_OUTPUT}")
elseif(MSVC)
    set(COMPILER_MSVC 1)
    if(MSVC_VERSION EQUAL 1400)
        set(MSVC_VERSION 2005)
    elseif(MSVC_VERSION EQUAL 1500)
        set(MSVC_VERSION 2008)
    elseif(MSVC_VERSION EQUAL 1600)
        set(MSVC_VERSION 2010)
    elseif(MSVC_VERSION EQUAL 1700)
        set(MSVC_VERSION 2011)
    endif()
else()
    message(FATAL_ERROR "Unsupported compiler")
    return()
endif()
