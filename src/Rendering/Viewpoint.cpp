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

#include "Locus/Geometry/Vector3Geometry.h"

namespace Locus
{

Viewpoint::Viewpoint()
   :  forward(Vec3D::NegativeZAxis()), up(Vec3D::YAxis()), right(Vec3D::XAxis())
{
}

const FVector3& Viewpoint::GetPosition() const
{
   return position;
}

const FVector3& Viewpoint::GetRight() const
{
   return right;
}

const FVector3& Viewpoint::GetUp() const
{
   return up;
}

const FVector3& Viewpoint::GetForward() const
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

FVector3 Viewpoint::ToEyePosition(const FVector3& worldPosition) const
{
   return transformation.GetInverse().MultVertex( worldPosition );
}

void Viewpoint::TranslateBy(const FVector3& translation)
{
   transformation.TranslateBy(translation);

   position += translation.x * right;
   position += translation.y * up;
   position += -translation.z * forward;
}

void Viewpoint::RotateBy(const FVector3& rotation)
{
   transformation.RotateBy(rotation);
   rotations.RotateBy(rotation);

   forward = rotations.MultVector(Vec3D::NegativeZAxis());
   up = rotations.MultVector(Vec3D::YAxis());
   right = Cross(forward, up);
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