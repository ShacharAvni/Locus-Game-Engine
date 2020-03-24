Locus Game Engine - Code Review
=================

1) Introduction

Thank you for taking the time to review the Locus Game Engine code.

The purpose of this document is to point you towards the more interesting
aspects of the Locus code base.


2) Locus First-Party Code

All Locus first-party code is contained in the following directories:

include - Contains the Locus public headers
src - Contains the Locus implementation
examples - Contains code for sample executables that use Locus


3) Locus Modules

Locus is split up into the following modules, each compiled as a separate library:

* Locus_Common: A collection of data structures and other common code used throughout
  the Locus codebase
* Locus_XML: XML parsing (mainly a wrapper around RapidXML)
* Locus_FileSystem: File and archive reading (mainly a wrapper around PhysFS)
* Locus_Audio: Audio file loading and playback (mainly a wrapper around OpenAL)
* Locus_Math: Matrix, polynomials and other algebra routines
* Locus_Geometry: 3D and 2D geometry and collision detection
* Locus_Rendering: 3D rendering (using OpenGL)
* Locus_Simulation: Window and user interaction (mainly a wrapper around GLFW)


4) Code to Review

This section points out code examples that may be of interest to review, organized by
Locus Module. I have chosen these examples because I believe they each show a clean,
elegant solution to a complex problem.

I have given each code example a tag in the form //{CodeReview:X}

The tags will point you towards the pertinent method definitions, method declarations and
class declarations to review. If you have downloaded the Locus code base to disk, then
search for the code tags in the include and src directories. Links to the code files are
also given.


4.1) Geometry

- Broad-Phase Collision Detection

* Code Tag: //{CodeReview:BroadPhaseCollisions}
* Class: CollisionManager
* Files: include/Locus/Geometry/CollisionManager.h, src/Geometry/CollisionManager.cpp
* Description: This class manages the broad-phase collision detection of a collection of
  Collidable objects.
* Links:
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/include/Locus/Geometry/CollisionManager.h
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/src/Geometry/CollisionManager.cpp


- Narrow-Phase Collsion Detection

* Code Tag: //{CodeReview:NarrowPhaseCollisions}
* Class: BoundingVolumeHierarchy
* Files: include/Locus/Geometry/BoundingVolumeHierarchy.h, src/Geometry/BoundingVolumeHierarchy.cpp
* Description: This class is an octree decomposition of a mesh into an Oriented Bounding Box
  Tree, a Sphere Tree, or an Axis Aligned Bounding Box Tree, depending on the template
  parameter.
* Links:
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/include/Locus/Geometry/BoundingVolumeHierarchy.h
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/src/Geometry/BoundingVolumeHierarchy.cpp


- Triangulating Polygon Hierarchies Using Ear Clipping

* Code Tag: //{CodeReview:Triangulation}
* Files: src/Geometry/Triangulation.cpp, src/Geometry/EarClipper.cpp
* Description: This an implementation of the algorithm described here
  http://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf
* Links:
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/src/Geometry/Triangulation.cpp
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/src/Geometry/EarClipper.cpp


4.2) Rendering

- Shader Generation for Point Lights

* Code Tag: //{CodeReview:ShaderGeneration}
* Files: include/Locus/Rendering/ShaderSourceStore.h, src/Rendering/ShaderSourceStore.cpp
* Description: These are functions for generating shader code for N point light sources.
* Links:
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/include/Locus/Rendering/ShaderSourceStore.h
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/src/Rendering/ShaderSourceStore.cpp


4.3) Math

- Solving Polynomials Using Newton's Method

* Code Tag: //{CodeReview:NewtonsMethod}
* Class: Polynomial
* Files: include/Locus/Math/Polynomial.h, src/Math/Polynomial.cpp
* Description: This method solves for the real roots of a polynomial with arbitrary degree.
* Links:
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/include/Locus/Math/Polynomial.h
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/src/Math/Polynomial.cpp


- Matrix Row Reduction

* Code Tag: //{CodeReview:RowReduction}
* Class: Matrix
* Files: include/Locus/Math/Matrix.h, src/Math/Matrix.cpp
* Description: This method turns the Matrix into row echelon form.
* Links:
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/include/Locus/Math/Matrix.h
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/src/Math/Matrix.cpp


- Steinhaus–Johnson–Trotter Permutation Algorithm with Even's Speedup

* Code Tag: //{CodeReview:SJTPermutations}
* Class: SJTPermutations
* Files: include/Locus/Math/SJTPermutations.h, src/Math/SJTPermutations.cpp
* Description: Generate the next permutation of alternating parity of the integers
  {0, 1, 2, ..., N} given their current order. This is an implementation of the
  algorithm described here http://en.wikipedia.org/wiki/Steinhaus%E2%80%93Johnson%E2%80%93Trotter_algorithm
* Links:
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/include/Locus/Math/SJTPermutations.h
  * https://github.com/ShacharAvni/Locus-Game-Engine/blob/master/src/Math/SJTPermutations.cpp