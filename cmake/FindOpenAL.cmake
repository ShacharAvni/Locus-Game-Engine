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

set(OPENAL_DIR ${PROJECT_SOURCE_DIR}/third-party/OpenAL)

if (WINDOWS)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${OPENAL_DIR}/include")
    if(COMPILER_GCC)
        set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${OPENAL_DIR}/lib/mingw")
        if(ARCH_32BITS)
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${OPENAL_DIR}/bin/x86")
        elseif(ARCH_64BITS)
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${OPENAL_DIR}/bin/x64")
        endif()
    elseif(COMPILER_MSVC)
        if(ARCH_32BITS)
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${OPENAL_DIR}/lib/msvc/x86")
        elseif(ARCH_64BITS)
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${OPENAL_DIR}/lib/msvc/x64")
        endif()
    endif()
elseif(MACOSX)
	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${OPENAL_DIR}/include")
endif()

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${OPENAL_DIR}/include/AL")