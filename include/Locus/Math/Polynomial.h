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

#include <vector>
#include <array>
#include <complex>
#include <type_traits>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

//{CodeReview:NewtonsMethod}
/*!
 * \brief A mathematical polynomial.
 *
 * \details supported types are float, double, and
 * long double.
 */
template <typename ScalarType>
class LOCUS_MATH_API Polynomial
{
public:
   static_assert(std::is_floating_point<ScalarType>::value, "ScalarType must be floating point");

   /*!
    * \brief Construct the Polynomial with the given
    * degree.
    *
    * \details The Polynomial's coefficients are
    * zero filled.
    */
   Polynomial(unsigned int degree);

   /*!
    * \return The coefficient at the given index.
    *
    * \details The coefficient at index 0 is the
    * constant term. The coefficient at index 1 is
    * the coefficient of x. The coefficient at index
    * 2 is the coefficient of x^2 and so on.
    */
   const ScalarType& operator[](unsigned int index) const;

   /// \sa Polynomial::operator[](unsigned int) const
   ScalarType& operator[](unsigned int index);

   /// \return The degree of the Polynomial.
   unsigned int GetDegree() const;

   /*!
    * \brief Sets the degree of the Polynomial.
    *
    * \details If the coefficient of the term
    * of the highest power of x is zero then it
    * is set to one.
    */
   void ForceDegree(unsigned int newDegree);

   /// \return true if all the coefficients are zero.
   bool IsNull() const;

   /// \return The Polynomial evaluated at the given value.
   std::complex<ScalarType> ValueAt(const std::complex<ScalarType>& x) const;

   /// \return The Polynomial evaluated at the given value.
   ScalarType ValueAt(ScalarType x) const;

   /// \return The derivative of the Polynomial.
   Polynomial<ScalarType> Derivative() const;

   /*!
    * \brief Solves for the (potentially) complex
    * roots of the Polynomial.
    *
    * \return true on success.
    */
   bool Solve(std::vector<std::complex<ScalarType>>& roots) const;

   /*!
    * \brief Solves for the non complex roots of
    * the Polynomial.
    *
    * \return true on success.
    */
   bool Solve(std::vector<ScalarType>& roots) const;

   //{CodeReview:NewtonsMethod}
   /*!
    * \brief Solves for the non complex roots of
    * the Polynomial with Newton's method.
    *
    * \return true on success.
    */
   bool SolveWithNewtonsMethod(std::vector<ScalarType>& roots) const;

private:
   static const std::array<std::complex<ScalarType>, 3>& CubicRootsOfUnity();

   bool SolveLinear(std::vector<std::complex<ScalarType>>& roots) const;
   bool SolveQuadratic(std::vector<std::complex<ScalarType>>& roots) const;
   bool SolveCubic(std::vector<std::complex<ScalarType>>& roots) const;
   bool SolveQuartic(std::vector<std::complex<ScalarType>>& roots) const;

   static bool SolveForSInQuarticFormula(ScalarType a, ScalarType p, ScalarType D0, const std::complex<ScalarType>& Q, std::complex<ScalarType>& S);

   unsigned int degree;

   std::vector<ScalarType> values;
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

#ifdef LOCUS_MATH_SHARED_IMPORTS

extern template class LOCUS_SHARED_IMPORTS Polynomial<float>;
extern template class LOCUS_SHARED_IMPORTS Polynomial<double>;
extern template class LOCUS_SHARED_IMPORTS Polynomial<long double>;

#endif

} // namespace Locus