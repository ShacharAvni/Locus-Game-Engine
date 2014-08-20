/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Math/Matrix.h"

#include "Locus/Common/Float.h"

#include <algorithm>

#include <cassert>

namespace Locus
{

template <typename ScalarType>
Matrix<ScalarType>::Matrix(unsigned int rows, unsigned int columns)
   : Rows(rows), Columns(columns), values(rows * columns)
{
   assert((Rows > 0) && (Columns > 0));
}

template <typename ScalarType>
Matrix<ScalarType>::~Matrix()
{
}

#define MATRIX_ACCESS_FUNCTION(row, col)\
   assert(row < Rows);\
   assert(col < Columns);\
   return values[(col * Rows) + row];

template <typename ScalarType>
ScalarType& Matrix<ScalarType>::At(unsigned int row, unsigned int col)
{
   MATRIX_ACCESS_FUNCTION(row, col)
}

template <typename ScalarType>
const ScalarType& Matrix<ScalarType>::At(unsigned int row, unsigned int col) const
{
   MATRIX_ACCESS_FUNCTION(row, col)
}

template <typename ScalarType>
ScalarType& Matrix<ScalarType>::operator()(unsigned int row, unsigned int col)
{
   MATRIX_ACCESS_FUNCTION(row, col)
}

template <typename ScalarType>
const ScalarType& Matrix<ScalarType>::operator()(unsigned int row, unsigned int col) const
{
   MATRIX_ACCESS_FUNCTION(row, col)
}

#undef MATRIX_ACCESS_FUNCTION

template <typename ScalarType>
unsigned int Matrix<ScalarType>::NumRows() const
{
   return Rows;
}

template <typename ScalarType>
unsigned int Matrix<ScalarType>::NumColumns() const
{
   return Columns;
}

template <typename ScalarType>
std::vector<ScalarType> Matrix<ScalarType>::operator*(const std::vector<ScalarType>& v) const
{
   assert(v.size() == Columns);

   std::vector<ScalarType> multiplied(Rows);

   for (unsigned int row = 0; row < Rows; ++row)
   {
      ScalarType multipliedValue = 0;

      for (unsigned int col = 0; col < Columns; ++col)
      {
         multipliedValue += At(row, col) * v[col];
      }

      multiplied[row] = multipliedValue;
   }

   return multiplied;
}

template <typename ScalarType>
Matrix<ScalarType> Matrix<ScalarType>::operator*(const Matrix<ScalarType>& matrix) const
{
   assert(Columns == matrix.Rows);

   Matrix<ScalarType> multiplied(Rows, matrix.Columns);

   for (unsigned int rowInResult = 0; rowInResult < Rows; ++rowInResult)
   {
      for (unsigned int columnInResult = 0; columnInResult < matrix.Columns; ++columnInResult)
      {
         ScalarType multipliedValue = 0;

         for (unsigned int commonDimensionIndex = 0; commonDimensionIndex < Columns; ++commonDimensionIndex)
         {
            multipliedValue += At(rowInResult, commonDimensionIndex) * matrix(commonDimensionIndex, columnInResult);
         }

         multiplied(rowInResult, columnInResult) = multipliedValue;
      }
   }

   return multiplied;
}

template <typename ScalarType>
void Matrix<ScalarType>::Fill(ScalarType value)
{
   std::fill(values.begin(), values.end(), value);
}

template <typename ScalarType>
void Matrix<ScalarType>::AddDimensions(unsigned int rows, unsigned int columns)
{
   ReDimension(Rows + rows, Columns + columns);
}

template <typename ScalarType>
void Matrix<ScalarType>::AddRows(unsigned int rows)
{
   ReDimension(Rows + rows, Columns);
}

template <typename ScalarType>
void Matrix<ScalarType>::AddColumns(unsigned int columns)
{
   ReDimension(Rows, Columns + columns);
}

template <typename ScalarType>
void Matrix<ScalarType>::RemoveDimensions(unsigned int rows, unsigned int columns)
{
   assert((rows < Rows) && (columns < Columns));

   ReDimension(Rows - rows, Columns - columns);
}

template <typename ScalarType>
void Matrix<ScalarType>::RemoveRows(unsigned int rows)
{
   assert(rows < Rows);

   ReDimension(Rows - rows, Columns);
}

template <typename ScalarType>
void Matrix<ScalarType>::RemoveColumns(unsigned int columns)
{
   assert(columns < Columns);

   ReDimension(Rows, Columns - columns);
}

template <typename ScalarType>
bool Matrix<ScalarType>::IsRowAllZero(unsigned int row, unsigned int upToColumn) const
{
   assert(row < Rows);

   for (unsigned int col = 0; col <= upToColumn; ++col)
   {
      if (Float::NotZero<ScalarType>(At(row, col)))
      {
         return false;
      }
   }

   return true;
}

template <typename ScalarType>
bool Matrix<ScalarType>::IsRowAllZero(unsigned int row) const
{
   return IsRowAllZero(row, Columns - 1);
}

template <typename ScalarType>
bool Matrix<ScalarType>::IsZeroMatrix() const
{
   for (unsigned int row = 0; row < Rows; ++row)
   {
      for (unsigned int col = 0; col < Columns; ++col)
      {
         if (Float::NotZero<ScalarType>(At(row, col)))
         {
            return false;
         }
      }
   }

   return true;
}

template <typename ScalarType>
Matrix<ScalarType> Matrix<ScalarType>::TransposedMatrix() const
{
   Matrix<ScalarType> transposed(*this);

   transposed.Transpose();

   return transposed;
}

template <typename ScalarType>
void Matrix<ScalarType>::Transpose()
{
   std::vector<ScalarType> newValues(Rows * Columns);

   std::size_t valueIndex = 0;

   for (unsigned int row = 0; row < Rows; ++row)
   {
      for (unsigned int col = 0; col < Columns; ++col)
      {
         newValues[valueIndex] = At(row, col);
         ++valueIndex;
      }
   }

   values = std::move(newValues);
   std::swap(Rows, Columns);
}

template <typename ScalarType>
Matrix<ScalarType> Matrix<ScalarType>::SubMatrix(unsigned int row, unsigned int col) const
{
   assert((row < Rows) && (col < Columns));
   assert((Rows > 1) && (Columns > 1));

   Matrix<ScalarType> subMatrix(Rows - 1, Columns - 1);

   unsigned int subRowIndex = 0;
   for (unsigned int rowIndex = 0; rowIndex < Rows; ++rowIndex)
   {
      if (rowIndex != row)
      {
         unsigned int subColumnIndex = 0;
         for (unsigned int columnIndex = 0; columnIndex < Columns; ++columnIndex)
         {
            if (columnIndex != col)
            {
               subMatrix(subRowIndex, subColumnIndex) = At(rowIndex, columnIndex);

               ++subColumnIndex;
            }
         }

         ++subRowIndex;
      }
   }

   return subMatrix;
}

template <typename ScalarType>
void Matrix<ScalarType>::MultMatrix(const Matrix<ScalarType>& otherMatrix)
{
   assert((otherMatrix.Rows == Columns) && (otherMatrix.Columns == Columns));

   std::vector<ScalarType> newValues(Rows * Columns);

   for (unsigned int rowInResult = 0; rowInResult < Rows; ++rowInResult)
   {
      for (unsigned int columnInResult = 0; columnInResult < Columns; ++columnInResult)
      {
         ScalarType multipliedValue = 0;

         for (unsigned int commonDimensionIndex = 0; commonDimensionIndex < Columns; ++commonDimensionIndex)
         {
            multipliedValue += At(rowInResult, commonDimensionIndex) * otherMatrix.At(commonDimensionIndex, columnInResult);
         }

         newValues[(columnInResult * Rows) + rowInResult] = multipliedValue;
      }
   }

   values = std::move(newValues);
}

template <typename ScalarType>
void Matrix<ScalarType>::SwapRows(unsigned int row1, unsigned int row2)
{
   assert(row1 < Rows);
   assert(row2 < Rows);

   for (unsigned int col = 0; col < Columns; ++col)
   {
      std::swap( At(row1, col), At(row2, col) );
   }
}

template <typename ScalarType>
void Matrix<ScalarType>::OperateOnRow(unsigned int rowToChange, ScalarType multipleOnRowToChange, unsigned int rowToUse, ScalarType multipleOnRowToUse)
{
   assert(rowToChange < Rows);
   assert(rowToUse < Rows);

   for (unsigned int col = 0; col < Columns; ++col)
   {
      At(rowToChange, col) = (multipleOnRowToChange * At(rowToChange, col)) + (multipleOnRowToUse * At(rowToUse, col));
   }
}

template <typename ScalarType>
void Matrix<ScalarType>::ScalarMultiplyRow(unsigned int rowToChange, ScalarType multipleOnRowToChange)
{
   assert(rowToChange < Rows);

   for (unsigned int col = 0; col < Columns; ++col)
   {
      At(rowToChange, col) *= multipleOnRowToChange;
   }
}

//{CodeReview:RowReduction}
template <typename ScalarType>
void Matrix<ScalarType>::MakeRowEchelon(bool reduce)
{
   unsigned int lastColumn = std::min(Rows, Columns) - 1;

   for (unsigned int col = 0; col <= lastColumn; ++col)
   {
      MakeRowEchelon(col, reduce);
   }

   if (reduce)
   {
      for (unsigned int col = 0; col <= lastColumn; ++col)
      {
         ReducePivot(col);
      }
   }
}

template <typename ScalarType>
const std::vector<ScalarType>& Matrix<ScalarType>::GetElements() const
{
   return values;
}

template <typename ScalarType>
const ScalarType* const Matrix<ScalarType>::Elements() const
{
   return values.data();
}

//{CodeReview:RowReduction}
template <typename ScalarType>
void Matrix<ScalarType>::MakeRowEchelon(unsigned int col, bool reduce)
{
   bool pivotInPlace = Float::NotZero<ScalarType>(At(col, col));

   if (!pivotInPlace)
   {
      for (unsigned int row = col + 1; row < Rows; ++row)
      {
         if (Float::NotZero<ScalarType>(At(row, col)))
         {
            SwapRows(col, row);

            pivotInPlace = true;
            break;
         }
      }
   }

   if (pivotInPlace)
   {
      unsigned int rowOfPivot = col;

      ScalarType pivot = At(rowOfPivot, col);

      if (reduce)
      {
         for (unsigned int row = 0; row < rowOfPivot; ++row)
         {
            ScalarType valueToMakeZero = At(row, col);

            if (Float::NotZero<ScalarType>(valueToMakeZero))
            {
               OperateOnRow(row, pivot, rowOfPivot, -valueToMakeZero);
            }
         }
      }

      for (unsigned int row = rowOfPivot + 1; row < Rows; ++row)
      {
         ScalarType valueToMakeZero = At(row, col);

         if (Float::NotZero<ScalarType>(valueToMakeZero))
         {
            OperateOnRow(row, pivot, rowOfPivot, -valueToMakeZero);
         }
      }
   }
}

template <typename ScalarType>
void Matrix<ScalarType>::ReducePivot(unsigned int col)
{
   ScalarType pivot = At(col, col);

   if (Float::NotZero<ScalarType>(pivot))
   {
      ScalarMultiplyRow(col, 1 / pivot);
   }
}

template <typename ScalarType>
void Matrix<ScalarType>::ReDimension(unsigned int newRows, unsigned int newColumns)
{
   Matrix<ScalarType>::ReDimensionFrom(values, *this, newRows, newColumns);

   Rows = newRows;
   Columns = newColumns;
}

template <typename ScalarType>
void Matrix<ScalarType>::ReDimensionFrom(std::vector<ScalarType>& matrixValues, const Matrix& matrix, unsigned int newRows, unsigned int newColumns)
{
   std::vector<ScalarType> newValues(newRows * newColumns);

   std::size_t valueIndex = 0;

   for (unsigned int columnIndex = 0; columnIndex < newColumns; ++columnIndex)
   {
      for (unsigned int rowIndex = 0; rowIndex < newRows; ++rowIndex)
      {
         if ((columnIndex < matrix.Columns) && (rowIndex < matrix.Rows))
         {
            newValues[valueIndex] = matrix.At(rowIndex, columnIndex);
         }

         ++valueIndex;
      }
   }

   matrixValues = std::move(newValues);
}

template <typename ScalarType>
std::vector<ScalarType> operator*(const std::vector<ScalarType>& v, const Matrix<ScalarType>& matrix)
{
   assert(v.size() == matrix.NumRows());

   unsigned int numRows = matrix.NumRows();
   unsigned int numColumns = matrix.NumColumns();

   std::vector<ScalarType> multiplied(numColumns);

   for (unsigned int col = 0; col < numColumns; ++col)
   {
      ScalarType multipliedValue = 0;

      for (unsigned int row = 0; row < numRows; ++row)
      {
         multipliedValue += matrix(row, col) * v[row];
      }

      multiplied[col] = multipliedValue;
   }

   return multiplied;
}

#ifdef LOCUS_MATH_DLL

template LOCUS_MATH_API std::vector<float> operator*<float>(const std::vector<float>& v, const Matrix<float>& matrix);
template LOCUS_MATH_API std::vector<double> operator*<double>(const std::vector<double>& v, const Matrix<double>& matrix);
template LOCUS_MATH_API std::vector<long double> operator*<long double>(const std::vector<long double>& v, const Matrix<long double>& matrix);

#endif

template class LOCUS_MATH_API Matrix<float>;
template class LOCUS_MATH_API Matrix<double>;
template class LOCUS_MATH_API Matrix<long double>;

}