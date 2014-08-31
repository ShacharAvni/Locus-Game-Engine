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

#include "Polynomial.h"

#include <vector>
#include <type_traits>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

//{CodeReview:RowReduction}
template <typename ScalarType>
class LOCUS_MATH_API Matrix
{
public:
   static_assert(std::is_floating_point<ScalarType>::value, "ScalarType must be floating point");

   Matrix(unsigned int rows, unsigned int columns);
   Matrix(unsigned int rows, unsigned int columns, std::initializer_list<ScalarType> rowMajorElements);

   ScalarType& At(unsigned int row, unsigned int col);
   const ScalarType& At(unsigned int row, unsigned int col) const;
   ScalarType& operator()(unsigned int row, unsigned int col);
   const ScalarType& operator()(unsigned int row, unsigned int col) const;

   unsigned int Rows() const;
   unsigned int Columns() const;

   bool IsSquare() const;

   const std::vector<ScalarType>& GetElements() const;

   const ScalarType* const Elements() const;

   void SetMainDiagonal(ScalarType value);
   void SetToIdentity();

   void Fill(ScalarType value);

   void AddDimensions(unsigned int rowsToAdd, unsigned int columnsToAdd);
   void AddRows(unsigned int rowsToAdd);
   void AddColumns(unsigned int columnsToAdd);

   void RemoveDimensions(unsigned int rowsToRemove, unsigned int columnsToRemove);
   void RemoveRows(unsigned int rowsToRemove);
   void RemoveColumns(unsigned int columnsToRemove);

   bool IsRowAllZero(unsigned int row, unsigned int upToColumn) const;
   bool IsRowAllZero(unsigned int row) const;
   bool IsZeroMatrix() const;

   Matrix<ScalarType> TransposedMatrix() const;
   void Transpose();

   Matrix<ScalarType> SubMatrix(unsigned int row, unsigned int col) const;

   void MultMatrix(const Matrix<ScalarType>& otherMatrix);

   void SwapRows(unsigned int row1, unsigned int row2);

   void MakeRowEchelon(bool reduce);

   bool IsDiagonal() const;

   ScalarType Determinant() const;
   bool IsInvertible() const;
   Matrix<ScalarType> GetInverse() const;
   bool Invert();

   ScalarType Trace() const;

   Polynomial<ScalarType> CharacteristicPolynomial() const;

   bool SolveEigenvalues(std::vector<ScalarType>& eigenValues) const;
   bool SolveEigenvectors(std::vector<std::vector<ScalarType>>& eigenVectors) const;

protected:
   unsigned int rows;
   unsigned int columns;
   std::vector<ScalarType> values;

   void ReDimension(unsigned int newRows, unsigned int newColumns);

   static void ReDimensionFrom(std::vector<ScalarType>& matrixValues, const Matrix& matrix, unsigned int newRows, unsigned int newColumns);

private:
   void MakeRowEchelon(unsigned int col, bool reduce);
   void ReducePivot(unsigned int col);
   void OperateOnRow(unsigned int rowToChange, ScalarType multipleOnRowToChange, unsigned int rowToUse, ScalarType multipleOnRowToUse);
   void ScalarMultiplyRow(unsigned int rowToChange, ScalarType multipleOnRowToChange);

   static ScalarType CharacteristicPolynomialCoefficientSubTerm(unsigned int subTerm, unsigned int M, const std::vector<ScalarType>& traces);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

template <typename ScalarType>
Matrix<ScalarType> operator*(const Matrix<ScalarType>& matrixLeft, const Matrix<ScalarType>& matrixRight);

template <typename ScalarType>
std::vector<ScalarType> operator*(const Matrix<ScalarType>& matrix, const std::vector<ScalarType>& v);

template <typename ScalarType>
std::vector<ScalarType> operator*(const std::vector<ScalarType>& v, const Matrix<ScalarType>& matrix);

#define LOCUS_MATRIX_EXTERN_TEMPLATE(Type) \
extern template class LOCUS_SHARED_IMPORTS Matrix<Type>;\
extern template LOCUS_SHARED_IMPORTS Matrix<Type> operator*<Type>(const Matrix<Type>& matrixLeft, const Matrix<Type>& matrixRight);\
extern template LOCUS_SHARED_IMPORTS std::vector<Type> operator*<Type>(const Matrix<Type>& matrix, const std::vector<Type>& v);\
extern template LOCUS_SHARED_IMPORTS std::vector<Type> operator*<Type>(const std::vector<Type>& v, const Matrix<Type>& matrix);

#ifdef LOCUS_MATH_SHARED_IMPORTS

LOCUS_MATRIX_EXTERN_TEMPLATE(float);
LOCUS_MATRIX_EXTERN_TEMPLATE(double);
LOCUS_MATRIX_EXTERN_TEMPLATE(long double);

#endif

#undef LOCUS_MATRIX_EXTERN_TEMPLATE

}