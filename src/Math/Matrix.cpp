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
#include "Locus/Math/MByNIterations.h"
#include "Locus/Math/SJTPermutations.h"

#include "Locus/Common/Float.h"

#include <set>
#include <algorithm>

#include <cassert>

namespace Locus
{

template <typename ScalarType>
Matrix<ScalarType>::Matrix(unsigned int rows, unsigned int columns)
   : rows(rows), columns(columns), values(rows * columns)
{
   assert((rows > 0) && (columns > 0));
}

template <typename ScalarType>
Matrix<ScalarType>::Matrix(unsigned int rows, unsigned int columns, std::initializer_list<ScalarType> rowMajorElements)
   : Matrix<ScalarType>(rows, columns)
{
   assert(rowMajorElements.size() == (rows * columns));

   const ScalarType* inputElement = rowMajorElements.begin();

   for (unsigned int row = 0; row < this->rows; ++row)
   {
      for (unsigned int col = 0; col < this->columns; ++col)
      {
         this->At(row, col) = *inputElement;
         ++inputElement;
      }
   }
}

template <typename ScalarType>
const std::vector<ScalarType>& Matrix<ScalarType>::GetElements() const
{
   return values;
}

template <typename ScalarType>
unsigned int Matrix<ScalarType>::Rows() const
{
   return rows;
}

template <typename ScalarType>
unsigned int Matrix<ScalarType>::Columns() const
{
   return columns;
}

#define MATRIX_ACCESS_FUNCTION(row, col)\
   assert(row < rows);\
   assert(col < columns);\
   return values[(col * rows) + row];

template <typename ScalarType>
ScalarType& Matrix<ScalarType>::At(unsigned int rowIndex, unsigned int colIndex)
{
   MATRIX_ACCESS_FUNCTION(rowIndex, colIndex)
}

template <typename ScalarType>
const ScalarType& Matrix<ScalarType>::At(unsigned int rowIndex, unsigned int colIndex) const
{
   MATRIX_ACCESS_FUNCTION(rowIndex, colIndex)
}

template <typename ScalarType>
ScalarType& Matrix<ScalarType>::operator()(unsigned int rowIndex, unsigned int colIndex)
{
   MATRIX_ACCESS_FUNCTION(rowIndex, colIndex)
}

template <typename ScalarType>
const ScalarType& Matrix<ScalarType>::operator()(unsigned int rowIndex, unsigned int colIndex) const
{
   MATRIX_ACCESS_FUNCTION(rowIndex, colIndex)
}

#undef MATRIX_ACCESS_FUNCTION

template <typename ScalarType>
bool Matrix<ScalarType>::IsSquare() const
{
   return (rows == columns);
}

template <typename ScalarType>
void Matrix<ScalarType>::SetMainDiagonal(ScalarType value)
{
   for (unsigned int dimensionIndex = 0; dimensionIndex < this->rows; ++dimensionIndex)
   {
      this->At(dimensionIndex, dimensionIndex) = value;
   }
}

template <typename ScalarType>
void Matrix<ScalarType>::SetToIdentity()
{
   this->Fill(0);
   SetMainDiagonal(1);
}

template <typename ScalarType>
void Matrix<ScalarType>::Fill(ScalarType value)
{
   std::fill(values.begin(), values.end(), value);
}

template <typename ScalarType>
void Matrix<ScalarType>::AddDimensions(unsigned int rowsToAdd, unsigned int columnsToAdd)
{
   ReDimension(rows + rowsToAdd, columns + columnsToAdd);
}

template <typename ScalarType>
void Matrix<ScalarType>::AddRows(unsigned int rowsToAdd)
{
   ReDimension(rows + rowsToAdd, columns);
}

template <typename ScalarType>
void Matrix<ScalarType>::AddColumns(unsigned int columnsToAdd)
{
   ReDimension(rows, columns + columnsToAdd);
}

template <typename ScalarType>
void Matrix<ScalarType>::RemoveDimensions(unsigned int rowsToRemove, unsigned int columnsToRemove)
{
   assert((rowsToRemove < rows) && (columnsToRemove < columns));

   ReDimension(rows - rowsToRemove, columns - columnsToRemove);
}

template <typename ScalarType>
void Matrix<ScalarType>::RemoveRows(unsigned int rowsToRemove)
{
   assert(rowsToRemove < rows);

   ReDimension(rows - rowsToRemove, columns);
}

template <typename ScalarType>
void Matrix<ScalarType>::RemoveColumns(unsigned int columnsToRemove)
{
   assert(columnsToRemove < columns);

   ReDimension(rows, columns - columnsToRemove);
}

template <typename ScalarType>
bool Matrix<ScalarType>::IsRowAllZero(unsigned int rowIndex, unsigned int upToColumnIndex) const
{
   assert(rowIndex < rows);

   for (unsigned int col = 0; col <= upToColumnIndex; ++col)
   {
      if (FNotZero<ScalarType>(At(rowIndex, col)))
      {
         return false;
      }
   }

   return true;
}

template <typename ScalarType>
bool Matrix<ScalarType>::IsRowAllZero(unsigned int rowIndex) const
{
   return IsRowAllZero(rowIndex, columns - 1);
}

template <typename ScalarType>
bool Matrix<ScalarType>::IsZeroMatrix() const
{
   for (unsigned int row = 0; row < rows; ++row)
   {
      for (unsigned int col = 0; col < columns; ++col)
      {
         if (FNotZero<ScalarType>(At(row, col)))
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
   std::vector<ScalarType> newValues(rows * columns);

   std::size_t valueIndex = 0;

   for (unsigned int row = 0; row < rows; ++row)
   {
      for (unsigned int col = 0; col < columns; ++col)
      {
         newValues[valueIndex] = At(row, col);
         ++valueIndex;
      }
   }

   values = std::move(newValues);
   std::swap(rows, columns);
}

template <typename ScalarType>
Matrix<ScalarType> Matrix<ScalarType>::SubMatrix(unsigned int rowIndexToRemove, unsigned int colIndexToRemove) const
{
   assert((rowIndexToRemove < rows) && (colIndexToRemove < columns));
   assert((rows > 1) && (columns > 1));

   Matrix<ScalarType> subMatrix(rows - 1, columns - 1);

   unsigned int subRowIndex = 0;
   for (unsigned int rowIndex = 0; rowIndex < rows; ++rowIndex)
   {
      if (rowIndex != rowIndexToRemove)
      {
         unsigned int subColumnIndex = 0;
         for (unsigned int columnIndex = 0; columnIndex < columns; ++columnIndex)
         {
            if (columnIndex != colIndexToRemove)
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
   assert((otherMatrix.rows == columns) && (otherMatrix.columns == columns));

   std::vector<ScalarType> newValues(rows * columns);

   for (unsigned int rowInResult = 0; rowInResult < rows; ++rowInResult)
   {
      for (unsigned int columnInResult = 0; columnInResult < columns; ++columnInResult)
      {
         ScalarType multipliedValue = 0;

         for (unsigned int commonDimensionIndex = 0; commonDimensionIndex < columns; ++commonDimensionIndex)
         {
            multipliedValue += At(rowInResult, commonDimensionIndex) * otherMatrix.At(commonDimensionIndex, columnInResult);
         }

         newValues[(columnInResult * rows) + rowInResult] = multipliedValue;
      }
   }

   values = std::move(newValues);
}

template <typename ScalarType>
void Matrix<ScalarType>::SwapRows(unsigned int rowIndex1, unsigned int rowIndex2)
{
   assert(rowIndex1 < rows);
   assert(rowIndex2 < rows);

   for (unsigned int col = 0; col < columns; ++col)
   {
      std::swap( At(rowIndex1, col), At(rowIndex2, col) );
   }
}

template <typename ScalarType>
void Matrix<ScalarType>::OperateOnRow(unsigned int rowToChange, ScalarType multipleOnRowToChange, unsigned int rowToUse, ScalarType multipleOnRowToUse)
{
   assert(rowToChange < rows);
   assert(rowToUse < rows);

   for (unsigned int col = 0; col < columns; ++col)
   {
      At(rowToChange, col) = (multipleOnRowToChange * At(rowToChange, col)) + (multipleOnRowToUse * At(rowToUse, col));
   }
}

template <typename ScalarType>
void Matrix<ScalarType>::ScalarMultiplyRow(unsigned int rowToChange, ScalarType multipleOnRowToChange)
{
   assert(rowToChange < rows);

   for (unsigned int col = 0; col < columns; ++col)
   {
      At(rowToChange, col) *= multipleOnRowToChange;
   }
}

//{CodeReview:RowReduction}
template <typename ScalarType>
void Matrix<ScalarType>::MakeRowEchelon(bool reduce)
{
   unsigned int lastColumn = std::min(rows, columns) - 1;

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

//{CodeReview:RowReduction}
template <typename ScalarType>
void Matrix<ScalarType>::MakeRowEchelon(unsigned int col, bool reduce)
{
   bool pivotInPlace = FNotZero<ScalarType>(At(col, col));

   if (!pivotInPlace)
   {
      for (unsigned int row = col + 1; row < rows; ++row)
      {
         if (FNotZero<ScalarType>(At(row, col)))
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

            if (FNotZero<ScalarType>(valueToMakeZero))
            {
               OperateOnRow(row, pivot, rowOfPivot, -valueToMakeZero);
            }
         }
      }

      for (unsigned int row = rowOfPivot + 1; row < rows; ++row)
      {
         ScalarType valueToMakeZero = At(row, col);

         if (FNotZero<ScalarType>(valueToMakeZero))
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

   if (FNotZero<ScalarType>(pivot))
   {
      ScalarMultiplyRow(col, 1 / pivot);
   }
}

template <typename ScalarType>
bool Matrix<ScalarType>::IsDiagonal() const
{
   for (unsigned int row = 0; row < this->rows; ++row)
   {
      for (unsigned int col = 0; col < this->columns; ++col)
      {
         if (row != col)
         {
            if (FNotZero<ScalarType>(this->At(row, col)))
            {
               return false;
            }
         }
      }
   }

   return true;
}

template <typename ScalarType>
ScalarType Matrix<ScalarType>::Determinant() const
{
   assert(IsSquare());

   ScalarType determinant = 0;

   SJTPermutations permutations(this->rows);

   char signOfPermutation = 1;

   do
   {
      ScalarType determinantSubProduct = signOfPermutation;

      for (unsigned int dimensionIndex = 0; dimensionIndex < this->rows; ++dimensionIndex)
      {
         determinantSubProduct *= this->At(dimensionIndex, permutations.GetElement(dimensionIndex));
      }

      determinant += determinantSubProduct;

      signOfPermutation *= -1;
   } while (permutations.GenerateNext());

   return determinant;
}

template <typename ScalarType>
bool Matrix<ScalarType>::IsInvertible() const
{
   return ( IsSquare() && FNotZero<ScalarType>(Determinant()) );
}

template <typename ScalarType>
Matrix<ScalarType> Matrix<ScalarType>::GetInverse() const
{
   assert(IsSquare());

   Matrix<ScalarType> inverse(*this);

   if (!inverse.Invert())
   {
      inverse.Fill(0);
   }
         
   return inverse;
}

template <typename ScalarType>
bool Matrix<ScalarType>::Invert()
{
   assert(IsSquare());

   Matrix<ScalarType> augmentedMatrix = *this;

   augmentedMatrix.AddColumns(this->columns);

   for (unsigned int col = this->columns; col < (2 * this->columns); ++col)
   {
      augmentedMatrix(col - this->columns, col) = 1;
   }

   augmentedMatrix.MakeRowEchelon(true);

   for (unsigned int col = 0; col < this->columns; ++col)
   {
      if (FNotEqual<ScalarType>(augmentedMatrix(col, col), 1))
      {
         return false;
      }
   }

   for (unsigned int row = 0; row < this->rows; ++row)
   {
      for (unsigned int col = 0; col < this->columns; ++col)
      {
         this->At(row, col) = augmentedMatrix(row, this->columns + col);
      }
   }

   return true;
}

template <typename ScalarType>
ScalarType Matrix<ScalarType>::Trace() const
{
   assert(IsSquare());

   ScalarType trace = 0;

   for (unsigned int dimensionIndex = 0; dimensionIndex < this->rows; ++dimensionIndex)
   {
      trace += this->At(dimensionIndex, dimensionIndex);
   }

   return trace;
}

template <typename ScalarType>
Polynomial<ScalarType> Matrix<ScalarType>::CharacteristicPolynomial() const
{
   assert(IsSquare());

   const unsigned int Degree = this->rows;

   Polynomial<ScalarType> characteristicPolynomial(Degree);

   if (Degree >= 2)
   {
      characteristicPolynomial[0] = Determinant();

      std::vector<ScalarType> traces(Degree);

      Matrix<ScalarType> multipliedMatrix = *this;

      for (unsigned int traceIndex = 0; traceIndex < Degree; ++traceIndex)
      {
         traces[traceIndex] = multipliedMatrix.Trace();
         multipliedMatrix.MultMatrix(*this);
      }

      const ScalarType minusOneToTheN = static_cast<ScalarType>( (Degree % 2 == 0) ? 1 : -1 );

      for (unsigned int coefficientIndex = Degree - 2; coefficientIndex >= 1; --coefficientIndex)
      {
         const unsigned int M = Degree - coefficientIndex;

         characteristicPolynomial[coefficientIndex] = -traces[M - 1] / M;

         unsigned int denominator = 1;
         int sign = -1;
         for (unsigned int subTerm = 2; subTerm < M; ++subTerm)
         {
            denominator *= subTerm;
            sign *= -1;

            characteristicPolynomial[coefficientIndex] += ( (sign * CharacteristicPolynomialCoefficientSubTerm(subTerm, M, traces)) / denominator );
         }

         denominator *= M;
         sign *= -1;

         characteristicPolynomial[coefficientIndex] += ( (sign * static_cast<ScalarType>(pow(traces[0], M))) / denominator );

         characteristicPolynomial[coefficientIndex] *= minusOneToTheN;
      }

      characteristicPolynomial[Degree - 1] = (minusOneToTheN * -1) * traces[0];
      characteristicPolynomial[Degree] = minusOneToTheN;
   }

   return characteristicPolynomial;
}

template <typename ScalarType>
bool Matrix<ScalarType>::SolveEigenvalues(std::vector<ScalarType>& eigenValues) const
{
   assert(IsSquare());

   if (IsDiagonal())
   {
      eigenValues.resize(this->rows);

      for (unsigned int dimensionIndex = 0; dimensionIndex < this->rows; ++dimensionIndex)
      {
         eigenValues[dimensionIndex] = this->At(dimensionIndex, dimensionIndex);
      }

      std::sort(eigenValues.begin(), eigenValues.end());

      return true;
   }
   else if (CharacteristicPolynomial().Solve(eigenValues))
   {
      std::sort(eigenValues.begin(), eigenValues.end());

      return true;
   }

   return false;
}

template <typename ScalarType>
bool Matrix<ScalarType>::SolveEigenvectors(std::vector<std::vector<ScalarType>>& eigenVectors) const
{
   assert(IsSquare());

   std::vector<ScalarType> eigenvalues(this->rows);

   if (SolveEigenvalues(eigenvalues))
   {
      eigenVectors.clear();
      eigenVectors.reserve(eigenvalues.size());

      std::set<ScalarType> uniqueEigenvalues;
      uniqueEigenvalues.insert(eigenvalues.begin(), eigenvalues.end());

      std::vector<unsigned int> nonZeroRows;
      nonZeroRows.reserve(this->rows);

      std::vector<unsigned int> freeRows;
      freeRows.reserve(this->rows);

      for (ScalarType eigenvalue : uniqueEigenvalues)
      {
         Matrix<ScalarType> augmentedMatrix = *this;

         augmentedMatrix.AddColumns(1);

         unsigned int numColumnsInAugmentedMatrix = augmentedMatrix.Columns();

         for (unsigned int row = 0; row < this->rows; ++row)
         {
            augmentedMatrix(row, row) -= eigenvalue;
         }

         augmentedMatrix.MakeRowEchelon(false);

         nonZeroRows.clear();
         freeRows.clear();

         for (unsigned int row = 0; row < this->rows; ++row)
         {
            if (augmentedMatrix.IsRowAllZero(row, numColumnsInAugmentedMatrix - 2))
            {
               if (FNotZero<ScalarType>(augmentedMatrix(row, numColumnsInAugmentedMatrix - 1)))
               {
                  //Here we have zero = (something not zero) so we have no solutions
                  return false;
               }

               freeRows.push_back(row);
            }
            else
            {
               nonZeroRows.push_back(row);
            }
         }

         std::vector<ScalarType> singleEigenvector(this->rows);

         std::size_t numFreeRows = freeRows.size();

         if (numFreeRows > 0)
         {
            for (std::size_t newEigenvectorIndex = 0; newEigenvectorIndex < numFreeRows; ++newEigenvectorIndex)
            {
               for (unsigned int freeRow : freeRows)
               {
                  singleEigenvector[freeRow] = 1;
               }

               if (newEigenvectorIndex > 0)
               {
                  singleEigenvector[freeRows[newEigenvectorIndex - 1]] = -1;
               }

               eigenVectors.push_back(singleEigenvector);
            }
         }
         else
         {
            singleEigenvector[this->rows - 1] = 1;

            for (int row = this->rows - 2; row >= 0; --row)
            {
               ScalarType eigenVectorTerm = augmentedMatrix(row, this->columns);

               for (int col = this->columns - 1; col > row; --col)
               {
                  eigenVectorTerm -= augmentedMatrix(row, col) * singleEigenvector[col];
               }

               singleEigenvector[row] = eigenVectorTerm / augmentedMatrix(row, row);
            }

            eigenVectors.push_back(singleEigenvector);
         }
      }

      return true;
   }

   return false;
}

template <typename ScalarType>
ScalarType Matrix<ScalarType>::CharacteristicPolynomialCoefficientSubTerm(unsigned int subTerm, unsigned int M, const std::vector<ScalarType>& traces)
{
   ScalarType coefficientSubTerm = 0;

   MByNIterations iterations(subTerm, M - subTerm + 1);

   do
   {
      std::size_t oneBasedIndexSum = 0;
      std::size_t oneBasedIndexProduct = 1;
      ScalarType traceProduct = 1;

      for (unsigned int elementIndex = 0; elementIndex < subTerm; ++elementIndex)
      {
         std::size_t element = iterations.GetElement(elementIndex);

         oneBasedIndexSum += (element + 1);
         oneBasedIndexProduct *= (element + 1);
         traceProduct *= traces[element];
      }

      if (oneBasedIndexSum == M)
      {
         coefficientSubTerm += (traceProduct / oneBasedIndexProduct);
      }
   } while (iterations.GenerateNext());

   return coefficientSubTerm;
}

template <typename ScalarType>
void Matrix<ScalarType>::ReDimension(unsigned int newRows, unsigned int newColumns)
{
   Matrix<ScalarType>::ReDimensionFrom(values, *this, newRows, newColumns);

   rows = newRows;
   columns = newColumns;
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
         if ((columnIndex < matrix.columns) && (rowIndex < matrix.rows))
         {
            newValues[valueIndex] = matrix.At(rowIndex, columnIndex);
         }

         ++valueIndex;
      }
   }

   matrixValues = std::move(newValues);
}

template <typename ScalarType>
Matrix<ScalarType> operator*(const Matrix<ScalarType>& matrixLeft, const Matrix<ScalarType>& matrixRight)
{
   assert(matrixLeft.Columns() == matrixRight.Rows());

   unsigned int numRowsLeft = matrixLeft.Rows();
   unsigned int numColumnsLeft = matrixLeft.Columns();

   unsigned int numColumnsRight = matrixRight.Columns();

   Matrix<ScalarType> multiplied(numRowsLeft, numColumnsRight);

   for (unsigned int rowInResult = 0; rowInResult < numRowsLeft; ++rowInResult)
   {
      for (unsigned int columnInResult = 0; columnInResult < numColumnsRight; ++columnInResult)
      {
         ScalarType multipliedValue = 0;

         for (unsigned int commonDimensionIndex = 0; commonDimensionIndex < numColumnsLeft; ++commonDimensionIndex)
         {
            multipliedValue += matrixLeft(rowInResult, commonDimensionIndex) * matrixRight(commonDimensionIndex, columnInResult);
         }

         multiplied(rowInResult, columnInResult) = multipliedValue;
      }
   }

   return multiplied;
}

template <typename ScalarType>
std::vector<ScalarType> operator*(const Matrix<ScalarType>& matrix, const std::vector<ScalarType>& v)
{
   assert(v.size() == matrix.Columns());

   unsigned int numRows = matrix.Rows();
   unsigned int numColumns = matrix.Columns();

   std::vector<ScalarType> multiplied(numRows);

   for (unsigned int row = 0; row < numRows; ++row)
   {
      ScalarType multipliedValue = 0;

      for (unsigned int col = 0; col < numColumns; ++col)
      {
         multipliedValue += matrix(row, col) * v[col];
      }

      multiplied[row] = multipliedValue;
   }

   return multiplied;
}

template <typename ScalarType>
std::vector<ScalarType> operator*(const std::vector<ScalarType>& v, const Matrix<ScalarType>& matrix)
{
   assert(v.size() == matrix.Rows());

   unsigned int numRows = matrix.Rows();
   unsigned int numColumns = matrix.Columns();

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

#define LOCUS_MATRIX_TEMPLATE_INSTANTION(Type) \
template class LOCUS_MATH_API Matrix<Type>;\
template LOCUS_MATH_API Matrix<Type> operator*<Type>(const Matrix<Type>& matrixLeft, const Matrix<Type>& matrixRight);\
template LOCUS_MATH_API std::vector<Type> operator*<Type>(const Matrix<Type>& matrix, const std::vector<Type>& v);\
template LOCUS_MATH_API std::vector<Type> operator*<Type>(const std::vector<Type>& v, const Matrix<Type>& matrix);

LOCUS_MATRIX_TEMPLATE_INSTANTION(float);
LOCUS_MATRIX_TEMPLATE_INSTANTION(double);
LOCUS_MATRIX_TEMPLATE_INSTANTION(long double);

#undef LOCUS_MATRIX_TEMPLATE_INSTANTION

}