Locus Game Engine
=================

##Description

Locus is a cross-platform 3D game engine written in C++ and OpenGL. It is a hobby project so it is not meant to compete
with AAA game engines though some of its code may prove useful to other projects.

Locus is split up into the following modules, each compiled as a separate library:

* Locus_Common: A collection of data structures and other common code used throughout the Locus codebase
* Locus_FileSystem: File and archive reading (mainly a wrapper around PhysFS)
* Locus_Audio: Audio file loading and playback (mainly a wrapper around OpenAL)
* Locus_Math: Matrix, polynomials and other algebra routines
* Locus_Geometry: 3D and 2D geometry and collision detection
* Locus_Rendering: 3D rendering
* Locus_Simulation: Window and user interaction (mainly a wrapper around GLFW)

##Supported Platforms

* Windows (compiles with Visual Studio 2013)
* Linux (compiles with GCC 4.9.1)
* Mac support and support for other compilers is forthcoming

##License

Locus uses a BSD-style license. See LICENSE.txt for more details.

##Compilation

Locus uses the [CMake build system](http://www.cmake.org/ "CMake Homepage"). The root project directory should be set as
the CMake source directory.

Locus ships with all its third-party dependencies.

Locus supports both static and dynamic linking. However, due to certain design decisions, including the use of the
C++ standard library at the API boundary and the use of C++ exceptions, great care is necessary for dynamic linking with Locus.
Namely, the application and all Locus modules should be compiled with the same compiler (and standard library) and should
use the same runtime library.

##Examples

Sample applications using Locus are planned to be included with the Locus project.

##Credits

Locus Game Engine Copyright (c) 2014 Shachar Avni. All rights reserved.

##Third-Party Software

Locus makes use of the following third-party software whose licenses
are available in the accompanying THIRD-PARTY-LICENSE.txt:

* [GLFW](http://www.glfw.org/ "GLFW Homepage") 3.0.3  
  Copyright (c) 2002-2006 Marcus Geelnard  
  Copyright (c) 2006-2011 Camilla Berglund

* [The OpenGL Extension Wrangler Library (GLEW)](http://glew.sourceforge.net/ "GLEW Homepage") 1.10.0  
  Copyright (c) 2002-2007, Milan Ikits  
  Copyright (c) 2002-2007, Marcelo E. Magallon  
  Copyright (c) 2002, Lev Povalahev

* [Open Audio Library (OpenAL)](http://www.openal.org/ "OpenAL Homepage")  
  Copyright (c) 1991, 1999 Free Software Foundation

* [Ogg Vorbis](http://www.vorbis.com/ "Ogg Vorbis Homepage")  
  libogg 1.3.1  
  libvorbis 1.3.4  
  Copyright (c) 2002, Xiph.org Foundation  
  Copyright (c) 2002-2008 Xiph.org Foundation

* [The FreeType Project](http://www.freetype.org/ "FreeType Project Homepage") 2.5.3  
  Portions of this software are copyright (c) 2014 The FreeType
  Project (www.freetype.org).  All rights reserved.
 
* [PhysicsFS](https://icculus.org/physfs/ "PhysicsFS Homepage") 2.0.3  
  Copyright (c) 2001-2012 Ryan C. Gordon and others.

* [RapidXML](http://rapidxml.sourceforge.net/ "RapidXML Homepage") 1.13  
  Copyright (c) 2006, 2007 Marcin Kalicinski

* [stb_image](http://nothings.org/stb_image.c "stb_image Homepage") 1.33  
  Public Domain