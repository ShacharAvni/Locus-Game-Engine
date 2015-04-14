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

#include "Locus/Math/Vectors.h"

namespace Locus
{

class LOCUS_GEOMETRY_API Moveable
{
public:
   Moveable();
   virtual ~Moveable();

   virtual void Translate(const FVector3& translation);
   virtual void Rotate(const FVector3& rotation);
   virtual void Scale(const FVector3& scale);

   virtual FVector3 Position() const;

   virtual void Reset(const FVector3& position, const Transformation& rotationTransformation = Transformation::Identity(), const FVector3& scale = Transformation::IdentityScale());

   const FVector3& CurrentScale() const;
   const FVector3& CurrentTranslation() const;
   const Transformation& CurrentRotation() const;

   const Transformation& CurrentModelTransformation() const;
   const Transformation& CurrentTranslationAndRotation() const;

private:
   Transformation modelRotation;
   FVector3 modelTranslation;
   FVector3 modelScale;

   Transformation modelTransformation;
   Transformation modelTranslationAndRotation;

   void UpdateTransformations();
};

}