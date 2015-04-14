/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Geometry/DualTransformation.h"

#include "Locus/Math/Vectors.h"

namespace Locus
{

void DualTransformation::SetToIdentity()
{
   transform.SetToIdentity();
   inverse.SetToIdentity();
}

void DualTransformation::TranslateBy(const FVector3& t)
{
   transform.TranslateBy(t);
   inverse.InverseTranslateBy(t);
}

void DualTransformation::RotateBy(const FVector3& rotation)
{
   transform.RotateBy(rotation);
   inverse.InverseRotateBy(rotation);
}

void DualTransformation::ScaleBy(const FVector3& scale)
{
   transform.ScaleBy(scale);
   inverse.InverseScaleBy(scale);
}

const Transformation& DualTransformation::GetTransformation() const
{
   return transform;
}

const Transformation& DualTransformation::GetInverse() const
{
   return inverse;
}

}