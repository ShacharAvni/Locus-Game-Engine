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

template <typename ScalarType>
class LOCUS_MATH_API Polynomial
{
public:
   static_assert(std::is_floating_point<ScalarType>::value, "ScalarType must be floating point");

   Polynomial(unsigned int degree);

   const ScalarType& operator[](unsigned int index) const;
   ScalarType& operator[](unsigned int index);

   unsigned int GetDegree() const;
   void ForceDegree(unsigned int newDegree);

   bool IsNull() const;

   std::complex<ScalarType> ValueAt(const std::complex<ScalarType>& x) const;
   ScalarType ValueAt(ScalarType x) const;

   Polynomial<ScalarType> Derivative() const;

   bool Solve(std::vector<std::complex<ScalarType>>& roots) const;
   bool Solve(std::vector<ScalarType>& roots) const;
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

}