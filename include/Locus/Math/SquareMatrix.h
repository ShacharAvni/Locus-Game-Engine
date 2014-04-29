 /*********************************************************************************************************\
 *                                                                                                        *
 *   This file is part of the Locus Game Engine                                                           *
 *                                                                                                        *
 *   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
 *                                                                                                        *
 *   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
 *                                                                                                        *
 \*********************************************************************************************************/

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