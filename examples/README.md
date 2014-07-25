Locus Examples
=================

##Description

Locus Examples contains sample programs that showcase a particular feature of the Locus Game Engine.

##Collisions

The Collisions example shows dynamic rigid body collision detection and response. Meshes are randomly generated
and collide with each other within a box.

###Controls

* Moving the mouse: rotates the viewpoint

* w: move forward
* s: move backward
* f: move right
* a: move left
* e: move up
* d: move down

The above keys may be combined. For instance, pressing w and e at the same time will move the viewpoint diagonally up and right.

* ESC: quit the demo

##Triangulation

The Triangulation example shows triangulation of polygon hierarchies of arbitrary depth using Ear Clipping.

###Controls

* Left Mouse Button: add a point to the current polygon
* Right or Middle Mouse Button: add a point to the current polygon and close the polygon if possible

* t: triangulate the current set of polygons

If there are any self-intersecting polygons or intersecting pairs of polygons then the input is thrown away at triangulation time.

##License

Locus Game Engine uses a BSD-style license. See LICENSE.txt for more details.

##Credits

Locus Game Engine Copyright (c) 2014 Shachar Avni. All rights reserved.

##Third-Party Software

Locus makes use of the following third-party software whose licenses
are available in the accompanying LICENSE.txt:

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