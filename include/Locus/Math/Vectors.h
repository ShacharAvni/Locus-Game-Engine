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

#include "LocusMathAPI.h"

#include "VectorsFwd.h"

#include <vector>

#include <cassert>

namespace Locus
{

template <typename ElementType>
struct LOCUS_MATH_API Vector2
{
   Vector2();
   Vector2(ElementType x, ElementType y);
   Vector2(const std::vector<ElementType>& elementsAsVector);

   void Set(ElementType x, ElementType y);

   ElementType& operator[](unsigned int index)
   {
      switch (index)
      {
      case 0:
         return x;

      case 1:
         return y;

      default:
         assert(0);
         return x;
      }
   }

   const ElementType& operator[](unsigned int index) const
   {
      return const_cast<Vector2<ElementType>*>(this)->operator[](index);
   }

   ElementType x;
   ElementType y;
};

template <typename ElementType>
Vector2<ElementType> operator+(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2);

template <typename ElementType>
Vector2<ElementType> operator-(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2);

template <typename ElementType>
Vector2<ElementType> operator*(const Vector2<ElementType>& v, ElementType s);

template <typename ElementType>
Vector2<ElementType> operator*(ElementType s, const Vector2<ElementType>& v);

template <typename ElementType>
Vector2<ElementType> operator/(const Vector2<ElementType>& v, ElementType d);

template <typename ElementType>
Vector2<ElementType> operator-(const Vector2<ElementType>& v);

template <typename ElementType>
Vector2<ElementType>& operator+=(Vector2<ElementType>& v1, const Vector2<ElementType>& v2);

template <typename ElementType>
Vector2<ElementType>& operator-=(Vector2<ElementType>& v1, const Vector2<ElementType>& v2);

template <typename ElementType>
Vector2<ElementType>& operator*=(Vector2<ElementType>& v, ElementType s);

template <typename ElementType>
Vector2<ElementType>& operator/=(Vector2<ElementType>& v, ElementType d);

template <typename ElementType>
bool operator==(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2);

template <typename ElementType>
bool operator!=(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2);

template <typename ElementType>
bool operator<(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2);

template <typename ElementType>
bool ApproximatelyEqual(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2, ElementType toleranceFactor = 1);

template <>
bool ApproximatelyEqual(const Vector2<int>& v1, const Vector2<int>& v2, int toleranceFactor);

template <typename ElementType>
void Serialize(const Vector2<ElementType>& v, ElementType* destination);

template <typename ElementType>
struct LOCUS_MATH_API Vector3
{
   Vector3();
   Vector3(ElementType x, ElementType y, ElementType z);
   Vector3(const std::vector<ElementType>& elementsAsVector);
   Vector3(const Vector2<ElementType>& vector2);

   void Set(ElementType x, ElementType y, ElementType z);

   ElementType& operator[](unsigned int index)
   {
      switch (index)
      {
      case 0:
         return x;

      case 1:
         return y;

      case 2:
         return z;

      default:
         assert(0);
         return x;
      }
   }

   const ElementType& operator[](unsigned int index) const
   {
      return const_cast<Vector3<ElementType>*>(this)->operator[](index);
   }

   ElementType x;
   ElementType y;
   ElementType z;
};

template <typename ElementType>
Vector3<ElementType> operator+(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2);

template <typename ElementType>
Vector3<ElementType> operator-(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2);

template <typename ElementType>
Vector3<ElementType> operator*(const Vector3<ElementType>& v, ElementType s);

template <typename ElementType>
Vector3<ElementType> operator*(ElementType s, const Vector3<ElementType>& v);

template <typename ElementType>
Vector3<ElementType> operator/(const Vector3<ElementType>& v, ElementType d);

template <typename ElementType>
Vector3<ElementType> operator-(const Vector3<ElementType>& v);

template <typename ElementType>
Vector3<ElementType>& operator+=(Vector3<ElementType>& v1, const Vector3<ElementType>& v2);

template <typename ElementType>
Vector3<ElementType>& operator-=(Vector3<ElementType>& v1, const Vector3<ElementType>& v2);

template <typename ElementType>
Vector3<ElementType>& operator*=(Vector3<ElementType>& v, ElementType s);

template <typename ElementType>
Vector3<ElementType>& operator/=(Vector3<ElementType>& v, ElementType d);

template <typename ElementType>
bool operator==(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2);

template <typename ElementType>
bool operator!=(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2);

template <typename ElementType>
bool operator<(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2);

template <typename ElementType>
bool ApproximatelyEqual(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2, ElementType toleranceFactor = 1);

template <>
bool ApproximatelyEqual(const Vector3<int>& v1, const Vector3<int>& v2, int toleranceFactor);

template <typename ElementType>
void Serialize(const Vector3<ElementType>& v, ElementType* destination);

template <typename ElementType>
struct LOCUS_MATH_API Vector4
{
   Vector4();
   Vector4(ElementType x, ElementType y, ElementType z, ElementType w);
   Vector4(const std::vector<ElementType>& elementsAsVector);
   Vector4(const Vector3<ElementType>& vector3);

   void Set(ElementType x, ElementType y, ElementType z, ElementType w);

   ElementType& operator[](unsigned int index)
   {
      switch (index)
      {
      case 0:
         return x;

      case 1:
         return y;

      case 2:
         return z;

      case 3:
         return w;

      default:
         assert(0);
         return x;
      }
   }

   const ElementType& operator[](unsigned int index) const
   {
      return const_cast<Vector4<ElementType>*>(this)->operator[](index);
   }

   ElementType x;
   ElementType y;
   ElementType z;
   ElementType w;
};

template <typename ElementType>
Vector4<ElementType> operator+(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2);

template <typename ElementType>
Vector4<ElementType> operator-(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2);

template <typename ElementType>
Vector4<ElementType> operator*(const Vector4<ElementType>& v, ElementType s);

template <typename ElementType>
Vector4<ElementType> operator*(ElementType s, const Vector4<ElementType>& v);

template <typename ElementType>
Vector4<ElementType> operator/(const Vector4<ElementType>& v, ElementType d);

template <typename ElementType>
Vector4<ElementType> operator-(const Vector4<ElementType>& v);

template <typename ElementType>
Vector4<ElementType>& operator+=(Vector4<ElementType>& v1, const Vector4<ElementType>& v2);

template <typename ElementType>
Vector4<ElementType>& operator-=(Vector4<ElementType>& v1, const Vector4<ElementType>& v2);

template <typename ElementType>
Vector4<ElementType>& operator*=(Vector4<ElementType>& v, ElementType s);

template <typename ElementType>
Vector4<ElementType>& operator/=(Vector4<ElementType>& v, ElementType d);

template <typename ElementType>
bool operator==(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2);

template <typename ElementType>
bool operator!=(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2);

template <typename ElementType>
bool operator<(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2);

template <typename ElementType>
bool ApproximatelyEqual(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2, ElementType toleranceFactor = 1);

template <>
bool ApproximatelyEqual(const Vector4<int>& v1, const Vector4<int>& v2, int toleranceFactor);

template <typename ElementType>
void Serialize(const Vector4<ElementType>& v, ElementType* destination);

#define LOCUS_VECTORS_EXTERN_TEMPLATE_PER_ELEMENT_TYPE(VectorType, ElementType) \
   extern template struct LOCUS_SHARED_IMPORTS VectorType<ElementType>;\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType> operator+(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType> operator-(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType> operator*(const VectorType<ElementType>& v, ElementType s);\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType> operator*(ElementType s, const VectorType<ElementType>& v);\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType> operator/(const VectorType<ElementType>& v, ElementType d);\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType> operator-(const VectorType<ElementType>& v);\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType>& operator+=(VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType>& operator-=(VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType>& operator*=(VectorType<ElementType>& v, ElementType s);\
   extern template LOCUS_SHARED_IMPORTS VectorType<ElementType>& operator/=(VectorType<ElementType>& v, ElementType d);\
   extern template LOCUS_SHARED_IMPORTS bool operator==(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   extern template LOCUS_SHARED_IMPORTS bool operator!=(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   extern template LOCUS_SHARED_IMPORTS bool operator<(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   extern template LOCUS_SHARED_IMPORTS bool ApproximatelyEqual(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2, ElementType toleranceFactor);\
   extern template LOCUS_SHARED_IMPORTS void Serialize(const VectorType<ElementType>& v, ElementType* destination);

#define LOCUS_VECTORS_EXTERN_TEMPLATE(VectorType) \
   LOCUS_VECTORS_EXTERN_TEMPLATE_PER_ELEMENT_TYPE(VectorType, float)\
   LOCUS_VECTORS_EXTERN_TEMPLATE_PER_ELEMENT_TYPE(VectorType, double)\
   LOCUS_VECTORS_EXTERN_TEMPLATE_PER_ELEMENT_TYPE(VectorType, long double)\
   LOCUS_VECTORS_EXTERN_TEMPLATE_PER_ELEMENT_TYPE(VectorType, int)

#ifdef LOCUS_MATH_SHARED_IMPORTS

LOCUS_VECTORS_EXTERN_TEMPLATE(Vector2)
LOCUS_VECTORS_EXTERN_TEMPLATE(Vector3)
LOCUS_VECTORS_EXTERN_TEMPLATE(Vector4)

#endif

#undef LOCUS_VECTORS_EXTERN_TEMPLATE_PER_ELEMENT_TYPE

#undef LOCUS_VECTORS_EXTERN_TEMPLATE

} // namespace Locus