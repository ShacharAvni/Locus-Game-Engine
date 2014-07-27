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
* Locus_FileSystem: File and archive reading (mainly a wrapper around PhysFS)
* Locus_Audio: Audio file loading and playback (mainly a wrapper around OpenAL)
* Locus_Math: Matrix, polynomials and other algebra routines
* Locus_Geometry: 3D and 2D geometry and collision detection
* Locus_Rendering: 3D rendering (using OpenGL)
* Locus_Simulation: Window and user interaction (mainly a wrapper around GLFW)

4) Code to Review

This section points out the actual classes, methods, or functions that may be of interest
to review, organized by Locus Module. I have chosen these examples because I believe they
each show a clean, elegant solution to a complex problem.

To find the given examples in the code base, I have given them each a tag in the form
//{CodeReview:X}. For each example, search for its corresponding tag in the include and
src directories. This will be easier if you have downloaded the Locus code base to disk.

4.1) Geometry

4.1.1) Broad-Phase Collision Detection

* Code Tag: //{CodeReview:BroadPhaseCollisions}
* Class: CollisionManager
* Files: include/Locus/Geometry/CollisionManager.h, src/Geometry/CollisionManager.cpp
* Description: This class manages the broad-phase collision detection of a collection of
  Collidable objects.

* Methods to Review:
  void UpdateCollisions()
  void TransmitCollisions()

4.1.2) Narrow-Phase Collsion Detection

* Code Tag: //{CodeReview:NarrowPhaseCollisions}
* Class: BoundingVolumeHierarchy
* Files: include/Locus/Geometry/BoundingVolumeHierarchy.h, src/Geometry/BoundingVolumeHierarchy.cpp
* Description: This class is an octree decomposition of a mesh into an Oriented Bounding Box
  Tree, a Sphere Tree, or an Axis Aligned Bounding Box Tree, depending on the template
  parameter.

* Methods to Review:
  BoundingVolumeHierarchy<BoundingVolume>::Node::Node()
  void BoundingVolumeHierarchy<BoundingVolume>::GetIntersection(const Moveable& thisMoveable, const BoundingVolumeHierarchy<BoundingVolume>& otherBoundingVolumeHierarchy, const Moveable& otherMoveable, std::unordered_set<std::size_t>& thisIntersectionSet, std::unordered_set<std::size_t>& otherIntersectionSet) const
  void BoundingVolumeHierarchy<BoundingVolume>::GetIntersection(const Moveable& thisMoveable, const OrientedBox& orientedBox, std::unordered_set<std::size_t>& thisIntersectionSet) const

4.1.3) Triangulating Polygon Hierarchies of Arbitrary Depth using Ear Clipping

* Code Tag: //{CodeReview:Triangulation}
* Files: include/Locus/Geometry/Triangulation.h, src/Geometry/Triangulation.cpp
* Description: This an implementation of the algorithm described here
  http://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf

* Functions to Review:
  void Triangulate(const Polygon2D_t& polygon, const std::vector<const Polygon2D_t*>& innerPolygons, std::vector<const Vector2*>& triangles)
  void Triangulate(std::vector<Polygon2D_t>& polygons, PolygonWinding winding, std::vector<const Vector2*>& triangles)
  void Triangulate_R(AugmentedVertexList& augmentedVertices, std::forward_list< AugmentedVertexList::iterator >& ears, PolygonWinding winding, std::vector<const Vector2*>& triangles)

4.2) Rendering

4.2.1) Shader Generation for Point Lights

* Code Tag: //{CodeReview:ShaderGeneration}
* Files: include/Locus/Rendering/ShaderSourceStore.h, src/Rendering/ShaderSourceStore.cpp
* Description: These are functions for generating shader code for N point light sources.

* Functions to Review:
  std::string Vert_1_30(bool textured, unsigned int numLights)
  std::string Frag_1_30(bool textured, unsigned int numLights)
  std::string Vert_Pre_1_30(GLInfo::GLSLVersion version, bool textured, unsigned int numLights)
  std::string Frag_Pre_1_30(GLInfo::GLSLVersion version, bool textured, unsigned int numLights)

4.3) Math

4.3.1) Solving Polynomials of Arbitrary Degree Using Newton's Method

* Code Tag: //{CodeReview:NewtonsMethod}
* Class: Polynomial
* Files: include/Locus/Math/Polynomial.h, src/Math/Polynomial.cpp
* Description: This method solves for the real roots of an arbitrary degree Polynomial.

* Method to Review:
  bool Polynomial<ScalarType>::SolveWithNewtonsMethod(std::vector<ScalarType>& roots) const

4.3.2) Row Reducing a Matrix

* Code Tag: //{CodeReview:RowReduction}
* Class: Matrix
* Files: include/Locus/Math/Matrix.h, src/Math/Matrix.cpp
* Description: This method turns the Matrix into row echelon form.

* Method to Review:
  void Matrix<ScalarType>::MakeRowEchelon(bool reduce)

4.3.3) Steinhaus�Johnson�Trotter Permutation Algorithm with Even's Speedup

* Code Tag: //{CodeReview:SJTPermutations}
* Class: SJTPermutations
* Files: include/Locus/Math/SJTPermutations.h, src/Math/SJTPermutations.cpp
* Description: Generate the next permutation of alternating parity of the integers
  {0, 1, 2, ..., N} given their current order. This is an implementation of the
  algorithm described here http://en.wikipedia.org/wiki/Steinhaus%E2%80%93Johnson%E2%80%93Trotter_algorithm

* Method to Review:
  bool SJTPermutations::GenerateNext()