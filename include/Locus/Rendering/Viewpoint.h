 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

#pragma once

#include "LocusRenderingAPI.h"

#include "Locus/Geometry/Vector3.h"
#include "Locus/Geometry/DualTransformation.h"
#include "Locus/Geometry/Transformation.h"

namespace Locus
{

class TransformationStack;

class LOCUS_RENDERING_API Viewpoint
{
public:
   Viewpoint();

   const Vector3& GetPosition() const;
   const Vector3& GetRight() const;
   const Vector3& GetUp() const;
   const Vector3& GetForward() const;
   const Transformation& GetRotation() const;
   const DualTransformation& GetTransformation() const;

   void TranslateBy(const Vector3& translation);
   void RotateBy(const Vector3& rotation);

   void Activate(TransformationStack& transformationStack) const;
   void Deactivate(TransformationStack& transformationStack) const;

private:
   DualTransformation transformation;
   Transformation rotations;

   Vector3 position;

   Vector3 forward;
   Vector3 up;
   Vector3 right;
};

}