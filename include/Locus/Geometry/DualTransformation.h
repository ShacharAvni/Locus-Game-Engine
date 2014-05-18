/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#pragma once

#include "LocusGeometryAPI.h"

#include "Transformation.h"

namespace Locus
{

class Vector3;

class LOCUS_GEOMETRY_API DualTransformation
{
public:
   void TranslateBy(const Vector3& t);
   void RotateBy(const Vector3& rotation);
   void ScaleBy(const Vector3& scale);

   const Transformation& GetTransformation() const;
   const Transformation& GetInverse() const;

private:
   Transformation transform;
   Transformation inverse;
};

}