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

#include "Matrix.h"
#include "Polynomial.h"

#include <vector>
#include <initializer_list>

namespace Locus
{

template <typename ScalarType>
class LOCUS_MATH_API SquareMatrix : public Matrix<ScalarType>
{
public:
   SquareMatrix(unsigned int dimension);
   SquareMatrix(unsigned int dimension, std::initializer_list<ScalarType> rowMajorElements);
   SquareMatrix(const Matrix<ScalarType>& matrix);

   SquareMatrix& operator=(const Matrix<ScalarType>& matrix);

   void CopyTo(Matrix<ScalarType>& matrix) const;

   void SetMainDiagonal(ScalarType value);
   void SetToIdentity();

   void AddDimensions(unsigned int rows, unsigned int columns);
   void AddRows(unsigned int rows);
   void AddColumns(unsigned int columns);

   void RemoveDimensions(unsigned int rows, unsigned int columns);
   void RemoveRows(unsigned int rows);
   void RemoveColumns(unsigned int columns);

   bool IsDiagonal() const;

   ScalarType Determinant() const;
   bool IsInvertible() const;
   SquareMatrix<ScalarType> GetInverse() const;
   bool Invert();

   ScalarType Trace() const;

   Polynomial<ScalarType> CharacteristicPolynomial() const;

   bool SolveEigenvalues(std::vector<ScalarType>& eigenValues) const;
   bool SolveEigenvectors(std::vector<std::vector<ScalarType>>& eigenVectors) const;

private:
   static ScalarType CharacteristicPolynomialCoefficientSubTerm(unsigned int subTerm, unsigned int M, const std::vector<ScalarType>& traces);
};

#ifdef LOCUS_MATH_SHARED_IMPORTS

extern template class LOCUS_SHARED_IMPORTS SquareMatrix<float>;
extern template class LOCUS_SHARED_IMPORTS SquareMatrix<double>;
extern template class LOCUS_SHARED_IMPORTS SquareMatrix<long double>;

#endif

}