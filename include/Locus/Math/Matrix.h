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
   virtual ~Matrix();

   ScalarType& At(unsigned int row, unsigned int col);
   const ScalarType& At(unsigned int row, unsigned int col) const;
   ScalarType& operator()(unsigned int row, unsigned int col);
   const ScalarType& operator()(unsigned int row, unsigned int col) const;

   unsigned int NumRows() const;
   unsigned int NumColumns() const;

   std::vector<ScalarType> operator*(const std::vector<ScalarType>& v) const;
   Matrix<ScalarType> operator*(const Matrix<ScalarType>& matrix) const;

   void Fill(ScalarType value);

   void AddDimensions(unsigned int rows, unsigned int columns);
   void AddRows(unsigned int rows);
   void AddColumns(unsigned int columns);

   void RemoveDimensions(unsigned int rows, unsigned int columns);
   void RemoveRows(unsigned int rows);
   void RemoveColumns(unsigned int columns);

   bool IsRowAllZero(unsigned int row, unsigned int upToColumn) const;
   bool IsRowAllZero(unsigned int row) const;
   bool IsZeroMatrix() const;

   Matrix<ScalarType> TransposedMatrix() const;
   void Transpose();

   Matrix<ScalarType> SubMatrix(unsigned int row, unsigned int col) const;

   void MultMatrix(const Matrix<ScalarType>& otherMatrix);

   void SwapRows(unsigned int row1, unsigned int row2);
   void OperateOnRow(unsigned int rowToChange, ScalarType multipleOnRowToChange, unsigned int rowToUse, ScalarType multipleOnRowToUse);
   void ScalarMultiplyRow(unsigned int rowToChange, ScalarType multipleOnRowToChange);

   void MakeRowEchelon(bool reduce);

   const std::vector<ScalarType>& GetElements() const;
   const ScalarType* const Elements() const;

private:
   void MakeRowEchelon(unsigned int col, bool reduce);
   void ReducePivot(unsigned int col);

protected:
   unsigned int Rows;
   unsigned int Columns;
   std::vector<ScalarType> values;

   virtual void ReDimension(unsigned int newRows, unsigned int newColumns);
   static void ReDimensionFrom(std::vector<ScalarType>& matrixValues, const Matrix& matrix, unsigned int newRows, unsigned int newColumns);
};

#include "Locus/Preprocessor/EndSilenceDLLInterfaceWarnings"

template <typename ScalarType>
std::vector<ScalarType> operator*(const std::vector<ScalarType>& v, const Matrix<ScalarType>& matrix);

#ifdef LOCUS_MATH_SHARED_IMPORTS

extern template LOCUS_SHARED_IMPORTS std::vector<float> operator*<float>(const std::vector<float>& v, const Matrix<float>& matrix);
extern template LOCUS_SHARED_IMPORTS std::vector<double> operator*<double>(const std::vector<double>& v, const Matrix<double>& matrix);
extern template LOCUS_SHARED_IMPORTS std::vector<long double> operator*<long double>(const std::vector<long double>& v, const Matrix<long double>& matrix);

extern template class LOCUS_SHARED_IMPORTS Matrix<float>;
extern template class LOCUS_SHARED_IMPORTS Matrix<double>;
extern template class LOCUS_SHARED_IMPORTS Matrix<long double>;

#endif

}