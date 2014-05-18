/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/Moveable.h"

namespace Locus
{

Moveable::Moveable()
   : modelScale(1, 1, 1)
{
}

Moveable::~Moveable()
{
}

void Moveable::Translate(const Vector3& translation)
{
   modelTranslation += translation;

   UpdateTransformations();
}

void Moveable::Rotate(const Vector3& rotation)
{
   modelRotation.RotateBy(rotation);

   UpdateTransformations();
}

void Moveable::Scale(const Vector3& scale)
{
   modelScale.x *= scale.x;
   modelScale.y *= scale.y;
   modelScale.z *= scale.z;

   UpdateTransformations();
}

void Moveable::UpdateTransformations()
{
   modelTranslationAndRotation = Transformation::Translation(modelTranslation);
   modelTranslationAndRotation.MultMatrix(modelRotation);

   modelTransformation = modelTranslationAndRotation;
   modelTransformation.MultMatrix( Transformation::Scale(modelScale) );
}

Vector3 Moveable::Position() const
{
   return modelTranslation;
}

const Vector3& Moveable::CurrentTranslation() const
{
   return modelTranslation;
}

const Transformation& Moveable::CurrentRotation() const
{
   return modelRotation;
}

void Moveable::Reset(const Vector3& position, const Transformation& rotationTransformation, const Vector3& scale)
{
   modelRotation = rotationTransformation;
   modelTranslation = position;
   modelScale = scale;

   UpdateTransformations();
}

const Vector3& Moveable::CurrentScale() const
{
   return modelScale;
}

const Transformation& Moveable::CurrentModelTransformation() const
{
   return modelTransformation;
}

const Transformation& Moveable::CurrentTranslationAndRotation() const
{
   return modelTranslationAndRotation;
}

}