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

/// Wraps a transformation and its inverse.
class LOCUS_GEOMETRY_API DualTransformation
{
public:
   /*!
    * \details Transformation::TranslateBy 
    * is called on the underlying transformation.
    * Transformation::InverseTranslateBy is
    * called on the inverse.
    *
    * \sa Transformation::TranslateBy Transformation::InverseTranslateBy
    */
   void TranslateBy(const Vector3& t);

   /*!
    * \details Transformation::RotateBy 
    * is called on the underlying transformation.
    * Transformation::InverseRotateBy is
    * called on the inverse.
    *
    * \sa Transformation::RotateBy Transformation::InverseRotateBy
    */
   void RotateBy(const Vector3& rotation);

   /*!
    * \details Transformation::ScaleBy 
    * is called on the underlying transformation.
    * Transformation::InverseScaleBy is
    * called on the inverse.
    *
    * \sa Transformation::ScaleBy Transformation::InverseScaleBy
    */
   void ScaleBy(const Vector3& scale);

   /*!
    * \return The underlying transformation.
    *
    * \note The underlying transformation is
    * the identity by default.
    */
   const Transformation& GetTransformation() const;

   /*!
    * \return The inverse of the underlying
    * transformation.
    *
    * \note The inverse is the identity by
    * default.
    */
   const Transformation& GetInverse() const;

private:
   Transformation transform;
   Transformation inverse;
};

}