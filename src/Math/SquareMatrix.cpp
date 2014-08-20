/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Math/SquareMatrix.h"
#include "Locus/Math/SJTPermutations.h"
#include "Locus/Math/MByNIterations.h"

#include "Locus/Common/Float.h"

#include <set>
#include <algorithm>

#include <cassert>

namespace Locus
{

template <typename ScalarType>
SquareMatrix<ScalarType>::SquareMatrix(unsigned int dimension)
   : Matrix<ScalarType>(dimension, dimension)
{
   SetMainDiagonal(1);
}

template <typename ScalarType>
SquareMatrix<ScalarType>::SquareMatrix(unsigned int dimension, std::initializer_list<ScalarType> rowMajorElements)
   : Matrix<ScalarType>(dimension, dimension)
{
   assert(rowMajorElements.size() == (dimension * dimension));

   const ScalarType* inputElement = rowMajorElements.begin();

   for (unsigned int row = 0; row < this->Rows; ++row)
   {
      for (unsigned int col = 0; col < this->Columns; ++col)
      {
         this->At(row, col) = *inputElement;
         ++inputElement;
      }
   }
}

template <typename ScalarType>
SquareMatrix<ScalarType>::SquareMatrix(const Matrix<ScalarType>& matrix)
   : Matrix<ScalarType>(matrix)
{
   if (this->Rows < this->Columns)
   {
      Matrix<ScalarType>::ReDimension(this->Rows, this->Rows);
      this->Columns = this->Rows;
   }
   else if (this->Columns < this->Rows)
   {
      Matrix<ScalarType>::ReDimension(this->Columns, this->Columns);
      this->Rows = this->Columns;
   }
}

template <typename ScalarType>
SquareMatrix<ScalarType>& SquareMatrix<ScalarType>::operator=(const Matrix<ScalarType>& matrix)
{
   if (matrix.NumRows() == matrix.NumColumns())
   {
      this->values = matrix.GetElements();
      this->Rows = matrix.NumRows();
   }
   else if (matrix.NumRows() < matrix.NumColumns())
   {
      Matrix<ScalarType>::ReDimensionFrom(this->values, matrix, matrix.NumRows(), matrix.NumRows());
      this->Rows = matrix.NumRows();
   }
   else
   {
      Matrix<ScalarType>::ReDimensionFrom(this->values, matrix, matrix.NumColumns(), matrix.NumColumns());
      this->Rows = matrix.NumColumns();
   }

   this->Columns = this->Rows;

   return *this;
}

template <typename ScalarType>
void SquareMatrix<ScalarType>::CopyTo(Matrix<ScalarType>& matrix) const
{
   for (unsigned int row = 0; row < this->Rows; ++row)
   {
      for (unsigned int col = 0; col < this->Columns; ++col)
      {
         matrix(row, col) = this->At(row, col);
      }
   }
}

template <typename ScalarType>
void SquareMatrix<ScalarType>::SetMainDiagonal(ScalarType value)
{
   for (unsigned int dimensionIndex = 0; dimensionIndex < this->Rows; ++dimensionIndex)
   {
      this->At(dimensionIndex, dimensionIndex) = value;
   }
}

template <typename ScalarType>
void SquareMatrix<ScalarType>::SetToIdentity()
{
   this->Fill(0);
   SetMainDiagonal(1);
}

template <typename ScalarType>
void SquareMatrix<ScalarType>::AddDimensions(unsigned int rows, unsigned int columns)
{
   if (rows < columns)
   {
      Matrix<ScalarType>::AddDimensions(rows, rows);
   }
   else
   {
      Matrix<ScalarType>::AddDimensions(columns, columns);
   }
}

template <typename ScalarType>
void SquareMatrix<ScalarType>::AddRows(unsigned int rows)
{
   Matrix<ScalarType>::AddDimensions(rows, rows);
}

template <typename ScalarType>
void SquareMatrix<ScalarType>::AddColumns(unsigned int columns)
{
   Matrix<ScalarType>::AddDimensions(columns, columns);
}

template <typename ScalarType>
void SquareMatrix<ScalarType>::RemoveDimensions(unsigned int rows, unsigned int columns)
{
   if (rows < columns)
   {
      Matrix<ScalarType>::RemoveDimensions(rows, rows);
   }
   else
   {
      Matrix<ScalarType>::RemoveDimensions(columns, columns);
   }
}

template <typename ScalarType>
void SquareMatrix<ScalarType>::RemoveRows(unsigned int rows)
{
   Matrix<ScalarType>::RemoveDimensions(rows, rows);
}

template <typename ScalarType>
void SquareMatrix<ScalarType>::RemoveColumns(unsigned int columns)
{
   Matrix<ScalarType>::RemoveDimensions(columns, columns);
}

template <typename ScalarType>
bool SquareMatrix<ScalarType>::IsDiagonal() const
{
   for (unsigned int row = 0; row < this->Rows; ++row)
   {
      for (unsigned int col = 0; col < this->Columns; ++col)
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
ScalarType SquareMatrix<ScalarType>::Determinant() const
{
   SJTPermutations permutations(this->Rows);

   ScalarType determinant = 0;

   char signOfPermutation = 1;

   do
   {
      ScalarType determinantSubProduct = signOfPermutation;

      for (unsigned int dimensionIndex = 0; dimensionIndex < this->Rows; ++dimensionIndex)
      {
         determinantSubProduct *= this->At(dimensionIndex, permutations.GetElement(dimensionIndex));
      }

      determinant += determinantSubProduct;

      signOfPermutation *= -1;
   } while (permutations.GenerateNext());

   return determinant;
}

template <typename ScalarType>
bool SquareMatrix<ScalarType>::IsInvertible() const
{
   return FNotZero<ScalarType>(Determinant());
}

template <typename ScalarType>
SquareMatrix<ScalarType> SquareMatrix<ScalarType>::GetInverse() const
{
   SquareMatrix<ScalarType> inverse(*this);

   if (!inverse.Invert())
   {
      inverse.Fill(0);
   }
         
   return inverse;
}

template <typename ScalarType>
bool SquareMatrix<ScalarType>::Invert()
{
   Matrix<ScalarType> augmentedMatrix(this->Rows, this->Columns);
   CopyTo(augmentedMatrix);

   augmentedMatrix.AddColumns(this->Columns);

   for (unsigned int col = this->Columns; col < (2 * this->Columns); ++col)
   {
      augmentedMatrix(col - this->Columns, col) = 1;
   }

   augmentedMatrix.MakeRowEchelon(true);

   for (unsigned int col = 0; col < this->Columns; ++col)
   {
      if (FNotEqual<ScalarType>(augmentedMatrix(col, col), 1))
      {
         return false;
      }
   }

   for (unsigned int row = 0; row < this->Rows; ++row)
   {
      for (unsigned int col = 0; col < this->Columns; ++col)
      {
         this->At(row, col) = augmentedMatrix(row, this->Columns + col);
      }
   }

   return true;
}

template <typename ScalarType>
ScalarType SquareMatrix<ScalarType>::Trace() const
{
   ScalarType trace = 0;

   for (unsigned int dimensionIndex = 0; dimensionIndex < this->Rows; ++dimensionIndex)
   {
      trace += this->At(dimensionIndex, dimensionIndex);
   }

   return trace;
}

template <typename ScalarType>
Polynomial<ScalarType> SquareMatrix<ScalarType>::CharacteristicPolynomial() const
{
   const unsigned int Degree = this->Rows;

   Polynomial<ScalarType> characteristicPolynomial(Degree);

   if (Degree >= 2)
   {
      characteristicPolynomial[0] = Determinant();

      std::vector<ScalarType> traces(Degree);

      SquareMatrix<ScalarType> multipliedMatrix = *this;
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
bool SquareMatrix<ScalarType>::SolveEigenvalues(std::vector<ScalarType>& eigenValues) const
{
   if (IsDiagonal())
   {
      eigenValues.resize(this->Rows);

      for (unsigned int dimensionIndex = 0; dimensionIndex < this->Rows; ++dimensionIndex)
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
bool SquareMatrix<ScalarType>::SolveEigenvectors(std::vector<std::vector<ScalarType>>& eigenVectors) const
{
   std::vector<ScalarType> eigenvalues(this->Rows);

   if (SolveEigenvalues(eigenvalues))
   {
      eigenVectors.clear();
      eigenVectors.reserve(eigenvalues.size());

      std::set<ScalarType> uniqueEigenvalues;
      uniqueEigenvalues.insert(eigenvalues.begin(), eigenvalues.end());

      std::vector<unsigned int> nonZeroRows;
      nonZeroRows.reserve(this->Rows);

      std::vector<unsigned int> freeRows;
      freeRows.reserve(this->Rows);

      for (ScalarType eigenvalue : uniqueEigenvalues)
      {
         Matrix<ScalarType> augmentedMatrix = *this;

         augmentedMatrix.AddColumns(1);

         unsigned int numColumnsInAugmentedMatrix = augmentedMatrix.NumColumns();

         for (unsigned int row = 0; row < this->Rows; ++row)
         {
            augmentedMatrix(row, row) -= eigenvalue;
         }

         augmentedMatrix.MakeRowEchelon(false);

         nonZeroRows.clear();
         freeRows.clear();

         for (unsigned int row = 0; row < this->Rows; ++row)
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

         std::vector<ScalarType> singleEigenvector(this->Rows);

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
            singleEigenvector[this->Rows - 1] = 1;

            for (int row = this->Rows - 2; row >= 0; --row)
            {
               ScalarType eigenVectorTerm = augmentedMatrix(row, this->Columns);

               for (int col = this->Columns - 1; col > row; --col)
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
ScalarType SquareMatrix<ScalarType>::CharacteristicPolynomialCoefficientSubTerm(unsigned int subTerm, unsigned int M, const std::vector<ScalarType>& traces)
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

template class LOCUS_MATH_API SquareMatrix<float>;
template class LOCUS_MATH_API SquareMatrix<double>;
template class LOCUS_MATH_API SquareMatrix<long double>;

}