/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Math/Vectors.h"

#include "Locus/Common/Float.h"

#include <cassert>

namespace Locus
{

///////////////////////////////////////////////// Vector2 /////////////////////////////////////////////////

template <typename ElementType>
Vector2<ElementType>::Vector2()
   : x(), y()
{
}

template <typename ElementType>
Vector2<ElementType>::Vector2(ElementType x, ElementType y)
   : x(x), y(y)
{
}

template <typename ElementType>
Vector2<ElementType>::Vector2(const std::vector<ElementType>& elementsAsVector)
{
   assert(elementsAsVector.size() == 2);

   x = elementsAsVector[0];
   y = elementsAsVector[1];
}

template <typename ElementType>
void Vector2<ElementType>::Set(ElementType x, ElementType y)
{
   this->x = x;
   this->y = y;
}

template <typename ElementType>
Vector2<ElementType> operator+(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2)
{
   return Vector2<ElementType>(v1.x + v2.x, v1.y + v2.y);
}

template <typename ElementType>
Vector2<ElementType> operator-(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2)
{
   return Vector2<ElementType>(v1.x - v2.x, v1.y - v2.y);
}

template <typename ElementType>
Vector2<ElementType> operator*(const Vector2<ElementType>& v, ElementType s)
{
   return Vector2<ElementType>(v.x * s, v.y * s);
}

template <typename ElementType>
Vector2<ElementType> operator*(ElementType s, const Vector2<ElementType>& v)
{
   return (v * s);
}

template <typename ElementType>
Vector2<ElementType> operator/(const Vector2<ElementType>& v, ElementType d)
{
   assert(d != ElementType());

   return Vector2<ElementType>(v.x / d, v.y / d);
}

template <typename ElementType>
Vector2<ElementType> operator-(const Vector2<ElementType>& v)
{
   return Vector2<ElementType>(-v.x, -v.y);
}

template <typename ElementType>
Vector2<ElementType>& operator+=(Vector2<ElementType>& v1, const Vector2<ElementType>& v2)
{
   v1.x += v2.x;
   v1.y += v2.y;

   return v1;
}

template <typename ElementType>
Vector2<ElementType>& operator-=(Vector2<ElementType>& v1, const Vector2<ElementType>& v2)
{
   v1.x -= v2.x;
   v1.y -= v2.y;

   return v1;
}

template <typename ElementType>
Vector2<ElementType>& operator*=(Vector2<ElementType>& v, ElementType s)
{
   v.x *= s;
   v.y *= s;

   return v;
}

template <typename ElementType>
Vector2<ElementType>& operator/=(Vector2<ElementType>& v, ElementType d)
{
   assert(d != ElementType());

   v.x /= d;
   v.y /= d;

   return v;
}

template <typename ElementType>
bool operator==(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2)
{
   return ((v1.x == v2.x) && (v1.y == v2.y));
}

template <typename ElementType>
bool operator!=(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2)
{
   return !(v1 == v2);
}

template <typename ElementType>
bool ApproximatelyEqual(const Vector2<ElementType>& v1, const Vector2<ElementType>& v2, ElementType toleranceFactor)
{
   return (FEqual<ElementType>(v1.x, v2.x, toleranceFactor) && FEqual<ElementType>(v1.y, v2.y, toleranceFactor));
}

template <>
bool ApproximatelyEqual(const Vector2<int>& v1, const Vector2<int>& v2, int /*toleranceFactor*/)
{
   return (v1 == v2);
}

///////////////////////////////////////////////// Vector3 /////////////////////////////////////////////////

template <typename ElementType>
Vector3<ElementType>::Vector3()
   : x(), y(), z()
{
}

template <typename ElementType>
Vector3<ElementType>::Vector3(ElementType x, ElementType y, ElementType z)
   : x(x), y(y), z(z)
{
}

template <typename ElementType>
Vector3<ElementType>::Vector3(const std::vector<ElementType>& elementsAsVector)
{
   assert(elementsAsVector.size() == 3);

   x = elementsAsVector[0];
   y = elementsAsVector[1];
   z = elementsAsVector[2];
}

template <typename ElementType>
Vector3<ElementType>::Vector3(const Vector2<ElementType>& vector2)
   : x(vector2.x), y(vector2.y), z()
{
}

template <typename ElementType>
void Vector3<ElementType>::Set(ElementType x, ElementType y, ElementType z)
{
   this->x = x;
   this->y = y;
   this->z = z;
}

template <typename ElementType>
Vector3<ElementType> operator+(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2)
{
   return Vector3<ElementType>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template <typename ElementType>
Vector3<ElementType> operator-(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2)
{
   return Vector3<ElementType>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template <typename ElementType>
Vector3<ElementType> operator*(const Vector3<ElementType>& v, ElementType s)
{
   return Vector3<ElementType>(v.x * s, v.y * s, v.z * s);
}

template <typename ElementType>
Vector3<ElementType> operator*(ElementType s, const Vector3<ElementType>& v)
{
   return (v * s);
}

template <typename ElementType>
Vector3<ElementType> operator/(const Vector3<ElementType>& v, ElementType d)
{
   assert(d != ElementType());

   return Vector3<ElementType>(v.x / d, v.y / d, v.z / d);
}

template <typename ElementType>
Vector3<ElementType> operator-(const Vector3<ElementType>& v)
{
   return Vector3<ElementType>(-v.x, -v.y, -v.z);
}

template <typename ElementType>
Vector3<ElementType>& operator+=(Vector3<ElementType>& v1, const Vector3<ElementType>& v2)
{
   v1.x += v2.x;
   v1.y += v2.y;
   v1.z += v2.z;

   return v1;
}

template <typename ElementType>
Vector3<ElementType>& operator-=(Vector3<ElementType>& v1, const Vector3<ElementType>& v2)
{
   v1.x -= v2.x;
   v1.y -= v2.y;
   v1.z -= v2.z;

   return v1;
}

template <typename ElementType>
Vector3<ElementType>& operator*=(Vector3<ElementType>& v, ElementType s)
{
   v.x *= s;
   v.y *= s;
   v.z *= s;

   return v;
}

template <typename ElementType>
Vector3<ElementType>& operator/=(Vector3<ElementType>& v, ElementType d)
{
   assert(d != ElementType());

   v.x /= d;
   v.y /= d;
   v.z /= d;

   return v;
}

template <typename ElementType>
bool operator==(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2)
{
   return ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z));
}

template <typename ElementType>
bool operator!=(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2)
{
   return !(v1 == v2);
}

template <typename ElementType>
bool ApproximatelyEqual(const Vector3<ElementType>& v1, const Vector3<ElementType>& v2, ElementType toleranceFactor)
{
   return (FEqual<ElementType>(v1.x, v2.x, toleranceFactor) && FEqual<ElementType>(v1.y, v2.y, toleranceFactor) && FEqual<ElementType>(v1.z, v2.z, toleranceFactor));
}

template <>
bool ApproximatelyEqual(const Vector3<int>& v1, const Vector3<int>& v2, int /*toleranceFactor*/)
{
   return (v1 == v2);
}

///////////////////////////////////////////////// Vector4 /////////////////////////////////////////////////

template <typename ElementType>
Vector4<ElementType>::Vector4()
   : x(), y(), z(), w()
{
}

template <typename ElementType>
Vector4<ElementType>::Vector4(ElementType x, ElementType y, ElementType z, ElementType w)
   : x(x), y(y), z(z), w(w)
{
}

template <typename ElementType>
Vector4<ElementType>::Vector4(const std::vector<ElementType>& elementsAsVector)
{
   assert(elementsAsVector.size() == 4);

   x = elementsAsVector[0];
   y = elementsAsVector[1];
   z = elementsAsVector[2];
   w = elementsAsVector[3];
}

template <typename ElementType>
Vector4<ElementType>::Vector4(const Vector3<ElementType>& vector3)
   : x(vector3.x), y(vector3.y), z(vector3.z), w()
{
}

template <typename ElementType>
void Vector4<ElementType>::Set(ElementType x, ElementType y, ElementType z, ElementType w)
{
   this->x = x;
   this->y = y;
   this->z = z;
   this->w = w;
}

template <typename ElementType>
Vector4<ElementType> operator+(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2)
{
   return Vector4<ElementType>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

template <typename ElementType>
Vector4<ElementType> operator-(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2)
{
   return Vector4<ElementType>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

template <typename ElementType>
Vector4<ElementType> operator*(const Vector4<ElementType>& v, ElementType s)
{
   return Vector4<ElementType>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template <typename ElementType>
Vector4<ElementType> operator*(ElementType s, const Vector4<ElementType>& v)
{
   return (v * s);
}

template <typename ElementType>
Vector4<ElementType> operator/(const Vector4<ElementType>& v, ElementType d)
{
   assert(d != ElementType());

   return Vector4<ElementType>(v.x / d, v.y / d, v.z / d, v.w / d);
}

template <typename ElementType>
Vector4<ElementType> operator-(const Vector4<ElementType>& v)
{
   return Vector4<ElementType>(-v.x, -v.y, -v.z, -v.w);
}

template <typename ElementType>
Vector4<ElementType>& operator+=(Vector4<ElementType>& v1, const Vector4<ElementType>& v2)
{
   v1.x += v2.x;
   v1.y += v2.y;
   v1.z += v2.z;
   v1.w += v2.w;

   return v1;
}

template <typename ElementType>
Vector4<ElementType>& operator-=(Vector4<ElementType>& v1, const Vector4<ElementType>& v2)
{
   v1.x -= v2.x;
   v1.y -= v2.y;
   v1.z -= v2.z;
   v1.w -= v2.w;

   return v1;
}

template <typename ElementType>
Vector4<ElementType>& operator*=(Vector4<ElementType>& v, ElementType s)
{
   v.x *= s;
   v.y *= s;
   v.z *= s;
   v.w *= s;

   return v;
}

template <typename ElementType>
Vector4<ElementType>& operator/=(Vector4<ElementType>& v, ElementType d)
{
   assert(d != ElementType());

   v.x /= d;
   v.y /= d;
   v.z /= d;
   v.w /= d;

   return v;
}

template <typename ElementType>
bool operator==(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2)
{
   return ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w));
}

template <typename ElementType>
bool operator!=(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2)
{
   return !(v1 == v2);
}

template <typename ElementType>
bool ApproximatelyEqual(const Vector4<ElementType>& v1, const Vector4<ElementType>& v2, ElementType toleranceFactor)
{
   return (FEqual<ElementType>(v1.x, v2.x, toleranceFactor) && FEqual<ElementType>(v1.y, v2.y, toleranceFactor) &&
           FEqual<ElementType>(v1.z, v2.z, toleranceFactor) && FEqual<ElementType>(v1.w, v2.w, toleranceFactor));
}

template <>
bool ApproximatelyEqual(const Vector4<int>& v1, const Vector4<int>& v2, int /*toleranceFactor*/)
{
   return (v1 == v2);
}

#define LOCUS_VECTORS_TEMPLATE_INSTANTIATION_PER_ELEMENT_TYPE(VectorType, ElementType) \
   template struct LOCUS_MATH_API VectorType<ElementType>;\
   template LOCUS_MATH_API VectorType<ElementType> operator+(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   template LOCUS_MATH_API VectorType<ElementType> operator-(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   template LOCUS_MATH_API VectorType<ElementType> operator*(const VectorType<ElementType>& v, ElementType s);\
   template LOCUS_MATH_API VectorType<ElementType> operator*(ElementType s, const VectorType<ElementType>& v);\
   template LOCUS_MATH_API VectorType<ElementType> operator/(const VectorType<ElementType>& v, ElementType d);\
   template LOCUS_MATH_API VectorType<ElementType> operator-(const VectorType<ElementType>& v);\
   template LOCUS_MATH_API VectorType<ElementType>& operator+=(VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   template LOCUS_MATH_API VectorType<ElementType>& operator-=(VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   template LOCUS_MATH_API VectorType<ElementType>& operator*=(VectorType<ElementType>& v, ElementType s);\
   template LOCUS_MATH_API VectorType<ElementType>& operator/=(VectorType<ElementType>& v, ElementType d);\
   template LOCUS_MATH_API bool operator==(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   template LOCUS_MATH_API bool operator!=(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2);\
   template LOCUS_MATH_API bool ApproximatelyEqual(const VectorType<ElementType>& v1, const VectorType<ElementType>& v2, ElementType toleranceFactor);

#define LOCUS_VECTORS_TEMPLATE_INSTANTIATION(VectorType) \
   LOCUS_VECTORS_TEMPLATE_INSTANTIATION_PER_ELEMENT_TYPE(VectorType, float)\
   LOCUS_VECTORS_TEMPLATE_INSTANTIATION_PER_ELEMENT_TYPE(VectorType, double)\
   LOCUS_VECTORS_TEMPLATE_INSTANTIATION_PER_ELEMENT_TYPE(VectorType, long double)\
   LOCUS_VECTORS_TEMPLATE_INSTANTIATION_PER_ELEMENT_TYPE(VectorType, int)

LOCUS_VECTORS_TEMPLATE_INSTANTIATION(Vector2)
LOCUS_VECTORS_TEMPLATE_INSTANTIATION(Vector3)
LOCUS_VECTORS_TEMPLATE_INSTANTIATION(Vector4)

#undef LOCUS_VECTORS_TEMPLATE_INSTANTIATION_PER_ELEMENT_TYPE

#undef LOCUS_VECTORS_TEMPLATE_INSTANTIATION

}