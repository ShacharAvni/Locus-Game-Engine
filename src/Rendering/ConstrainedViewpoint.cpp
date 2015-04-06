/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Rendering/ConstrainedViewpoint.h"
#include "Locus/Rendering/TransformationStack.h"

#include "Locus/Common/Util.h"

#include "Locus/Common/Float.h"

#include "Locus/Geometry/Geometry.h"

namespace Locus
{

ConstrainedViewpoint::ConstrainedViewpoint()
   : angleAroundRight(0.0f), angleAroundUp(0.0f)
{
   UpdateTransformation();
}

const Vector3& ConstrainedViewpoint::GetPosition() const
{
   return position;
}

const Vector3& ConstrainedViewpoint::GetRight() const
{
   return right;
}

const Vector3& ConstrainedViewpoint::GetUp() const
{
   return up;
}

const Vector3& ConstrainedViewpoint::GetForward() const
{
   return forward;
}

const Transformation& ConstrainedViewpoint::GetRotation() const
{
   return rotations;
}

void ConstrainedViewpoint::GetPositionAndOrientation(Vector3& position, float& angleAroundRight, float& angleAroundUp) const
{
   position = this->position;
   angleAroundRight = this->angleAroundRight;
   angleAroundUp = this->angleAroundUp;
}

void ConstrainedViewpoint::SetPositionAndOrientation(const Vector3& position, float angleAroundRight, float angleAroundUp)
{
   this->position = position;
   this->angleAroundRight = Clamp(angleAroundRight, -HALF_PI, HALF_PI);
   this->angleAroundUp = angleAroundUp;

   UpdateTransformation();
}

Vector3 ConstrainedViewpoint::ToEyePosition(const Vector3& worldPosition) const
{
   return inverseTransform.MultVertex( worldPosition );
}

void ConstrainedViewpoint::TranslateBy(const Vector3& translation)
{
   position += translation.x * right;
   position += translation.y * up;
   position += translation.z * forward;

   UpdateTransformation();
}

void ConstrainedViewpoint::RotateBy(const Vector3& rotation)
{
   angleAroundRight += rotation.x;
   angleAroundRight = Clamp(angleAroundRight, -HALF_PI, HALF_PI);

   angleAroundUp += rotation.y;

   UpdateTransformation();
}

void ConstrainedViewpoint::UpdateTransformation()
{
   //TODO: Support other initial orientations

   rotations = Transformation::XRotation(HALF_PI) * Transformation::YRotation(angleAroundUp) * Transformation::XRotation(angleAroundRight);

   forward = rotations.MultVector(Vector3::NegativeZAxis());
   up = rotations.MultVector(Vector3::YAxis());
   right = forward.cross(up);

   Transformation inverseRotations;
   inverseRotations = Transformation::XRotation(-angleAroundRight) * Transformation::YRotation(-angleAroundUp) * Transformation::XRotation(-HALF_PI);

   inverseTransform = inverseRotations * Transformation::Translation(-position);
}

void ConstrainedViewpoint::Activate(TransformationStack& transformationStack) const
{
   transformationStack.Push( inverseTransform );
}

void ConstrainedViewpoint::Deactivate(TransformationStack& transformationStack) const
{
   transformationStack.Pop();
}

}