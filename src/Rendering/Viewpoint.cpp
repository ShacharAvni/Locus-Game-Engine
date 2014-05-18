/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/Viewpoint.h"
#include "Locus/Rendering/TransformationStack.h"

namespace Locus
{

Viewpoint::Viewpoint()
   :  forward(Vector3::NegativeZAxis()), up(Vector3::YAxis()), right(Vector3::XAxis())
{
}

const Vector3& Viewpoint::GetPosition() const
{
   return position;
}

const Vector3& Viewpoint::GetRight() const
{
   return right;
}

const Vector3& Viewpoint::GetUp() const
{
   return up;
}

const Vector3& Viewpoint::GetForward() const
{
   return forward;
}

const Transformation& Viewpoint::GetRotation() const
{
   return rotations;
}

const DualTransformation& Viewpoint::GetTransformation() const
{
   return transformation;
}

void Viewpoint::TranslateBy(const Vector3& translation)
{
   transformation.TranslateBy(translation);

   position += translation.x * right;
   position += translation.y * up;
   position += -translation.z * forward;
}

void Viewpoint::RotateBy(const Vector3& rotation)
{
   transformation.RotateBy(rotation);
   rotations.RotateBy(rotation);

   forward = rotations.MultVector(Vector3::NegativeZAxis());
   up = rotations.MultVector(Vector3::YAxis());
   right = forward.cross(up);
}

void Viewpoint::Activate(TransformationStack& transformationStack) const
{
   transformationStack.Push( transformation.GetInverse() );
}

void Viewpoint::Deactivate(TransformationStack& transformationStack) const
{
   transformationStack.Pop();
}

}