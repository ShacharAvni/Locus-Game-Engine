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
#include "Vector3.h"

namespace Locus
{

class LOCUS_GEOMETRY_API Moveable
{
public:
   Moveable();
   virtual ~Moveable();

   virtual void Translate(const Vector3& translation);
   virtual void Rotate(const Vector3& rotation);
   virtual void Scale(const Vector3& scale);

   virtual Vector3 Position() const;

   virtual void Reset(const Vector3& position, const Transformation& rotationTransformation = Transformation::Identity(), const Vector3& scale = Transformation::IdentityScale());

   const Vector3& CurrentScale() const;
   const Vector3& CurrentTranslation() const;
   const Transformation& CurrentRotation() const;

   const Transformation& CurrentModelTransformation() const;
   const Transformation& CurrentTranslationAndRotation() const;

private:
   Transformation modelRotation;
   Vector3 modelTranslation;
   Vector3 modelScale;

   Transformation modelTransformation;
   Transformation modelTranslationAndRotation;

   void UpdateTransformations();
};

}