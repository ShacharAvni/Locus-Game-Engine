 /********************************************************************************************************************************************************\
 *                                                                                                                                                        *
 *   This file is part of the Locus Game Engine                                                                                                           *
 *                                                                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                                                                *
 *                                                                                                                                                        *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),   *
 *   to deal in the Software without restriction, including without limitation the rights to use, modify, distribute, and to permit persons to whom the   *
 *   Software is furnished to do so, subject to the following conditions:                                                                                 *
 *                                                                                                                                                        *
 *   a) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                      *
 *   b) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the             *
 *      documentation and/or other materials provided with the distribution.                                                                              *
 *                                                                                                                                                        *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    *
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,            *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF            *
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,    *
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                                                   *
 *                                                                                                                                                        *
 \********************************************************************************************************************************************************/

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