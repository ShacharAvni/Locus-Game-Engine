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
#include "Locus/Geometry/Vector3.h"

namespace Locus
{

void DualTransformation::TranslateBy(const Vector3& t)
{
   transform.TranslateBy(t);
   inverse.InverseTranslateBy(t);
}

void DualTransformation::RotateBy(const Vector3& rotation)
{
   transform.RotateBy(rotation);
   inverse.InverseRotateBy(rotation);
}

void DualTransformation::ScaleBy(const Vector3& scale)
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