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
/*!
 * \brief A mathematical matrix.
 *
 * \details supported types are float, double, and
 * long double.
 */
template <typename ScalarType>
class LOCUS_MATH_API Matrix
{
public:
   static_assert(std::is_floating_point<ScalarType>::value, "ScalarType must be floating point");

   /// \details the matrix is zero filled.
   Matrix(unsigned int rows, unsigned int columns);

   /*!
    * \brief constructs the matrix given all the elements in
    * row major order
    *
    * \code{.cpp}
    * //the following constructs a 2 x 3 float matrix with the
    * //following values:
    * //1 in row 0 column 0
    * //2 in row 0 column 1
    * //3 in row 0 column 2
    * //4 in row 1 column 0
    * //5 in row 1 column 1
    * //6 in row 1 column 2
    *
    * Matrix<float> matrix(2, 3, { 1, 2, 3,
    *                              4, 5, 6 });
    * \endcode
    */
   Matrix(unsigned int rows, unsigned int columns, std::initializer_list<ScalarType> rowMajorElements);

   /*!
    * \brief gets the elements of the matrix as a single
    * dimensional vector.
    *
    * \details the elements are returned in column major
    * format. For example, say the matrix has two rows
    * and three columns. Then elements[0] would be at
    * row 0 and column 0, elements[1] would be at row
    * 1 column 0, elements[2] would be at row 0 column
    * 1, elements[3] would be at row 1 column 1,
    * elements[4] would be at row 0 column 2, and
    * elements[5] would be at row 1 column 2.
    */
   const std::vector<ScalarType>& GetElements() const;

   /// \return the number of rows in the matrix.
   unsigned int Rows() const;

   /// \return the number of columns in the matrix.
   unsigned int Columns() const;

   /// \return the element at row rowIndex and column colIndex.
   ScalarType& At(unsigned int rowIndex, unsigned int colIndex);

   /// \return the element at row rowIndex and column colIndex.
   const ScalarType& At(unsigned int rowIndex, unsigned int colIndex) const;

   /// \return the element at row rowIndex and column colIndex.
   ScalarType& operator()(unsigned int rowIndex, unsigned int colIndex);

   /// \return the element at row rowIndex and column colIndex.
   const ScalarType& operator()(unsigned int rowIndex, unsigned int colIndex) const;

   /// \return true if the matrix is square.
   bool IsSquare() const;

   /// Sets all the elements on the main diagonal to have the given value.
   void SetMainDiagonal(ScalarType value);

   /*!
    * \brief Sets the matrix to be the identity matrix.
    *
    * \details For rectangular matrices, all elements
    * on the main diagonal are set to one and all other
    * elements are set to zero.
    */
   void SetToIdentity();

   /// Sets all elements to the given value.
   void Fill(ScalarType value);

   /// Adds rows at the bottom and columns at the right of the current matrix, filled with zeroes.
   void AddDimensions(unsigned int rowsToAdd, unsigned int columnsToAdd);

   /// Adds rows at the bottom of the current matrix, filled with zeroes.
   void AddRows(unsigned int rowsToAdd);

   /// Adds columns at the right of the current matrix, filled with zeroes.
   void AddColumns(unsigned int columnsToAdd);

   /// Removes rows from the bottom and columns from the right of the current matrix.
   void RemoveDimensions(unsigned int rowsToRemove, unsigned int columnsToRemove);

   /// Removes rows from the bottom of the current matrix.
   void RemoveRows(unsigned int rowsToRemove);

   /// Removes columns from the right of the current matrix.
   void RemoveColumns(unsigned int columnsToRemove);

   /*!
    * \return true if the elements in the given row are zero
    *
    * \details the columns checked are from zero up to and
    * including upToColumnIndex.
    */
   bool IsRowAllZero(unsigned int rowIndex, unsigned int upToColumnIndex) const;

   /// \return true if all the elements in the given row are zero.
   bool IsRowAllZero(unsigned int rowIndex) const;

   /// \return true if all the elements in the matrix are zero.
   bool IsZeroMatrix() const;

   /// \return the transpose of the current matrix.
   Matrix<ScalarType> TransposedMatrix() const;

   /// Transposes the current matrix.
   void Transpose();

   /// \return the current matrix with the given row and column removed.
   Matrix<ScalarType> SubMatrix(unsigned int rowIndexToRemove, unsigned int colIndexToRemove) const;

   /// Multiplies the current matrix by the given matrix.
   void MultMatrix(const Matrix<ScalarType>& otherMatrix);

   /// Swaps the given rows of the matrix.
   void SwapRows(unsigned int rowIndex1, unsigned int rowIndex2);

   //{CodeReview:RowReduction}
   /*!
    * \brief changes the matrix into row echelon form.
    *
    * \param[in] reduce If true, then the matrix is changed
    * into reduced row echelon form.
    */
   void MakeRowEchelon(bool reduce);

   /*!
    * \return true if the matrix is diagonal.
    *
    * \details A rectangular matrix is considered
    * diagonal if all the elements not on its main
    * diagonal are zero.
    */
   bool IsDiagonal() const;

   /*!
    * \return the determinant of the matrix.
    *
    * \details asserts if the matrix is rectangular.
    */
   ScalarType Determinant() const;

   /*!
    * \return true if the matrix is invertible.
    *
    * \details rectangular matrices are considered to
    * never be invertible.
    */
   bool IsInvertible() const;

   /*!
    * \return the inverse of the matrix.
    *
    * \details asserts if the matrix is rectangular.
    * If the matrix is not invertible, then the zero
    * matrix is returned.
    *
    * \sa IsInvertible
    */
   Matrix<ScalarType> GetInverse() const;

   /*!
    * \brief Inverts the matrix.
    *
    * \return true if the matrix is invertible.
    *
    * \sa IsInvertible
    */
   bool Invert();

   /*!
    * \return the trace of the matrix.
    *
    * \details asserts if the matrix is rectangular.
    */
   ScalarType Trace() const;

   /*!
    * \return the characteristic polynomial of the matrix.
    *
    * \details asserts if the matrix is rectangular.
    */
   Polynomial<ScalarType> CharacteristicPolynomial() const;

   /*!
    * \brief solves for the eigenvalues of the matrix.
    *
    * \return true on success.
    *
    * \details asserts if the matrix is rectangular.
    */
   bool SolveEigenvalues(std::vector<ScalarType>& eigenValues) const;

   /*!
    * \brief solves for the eigenvectors of the matrix.
    *
    * \return true on success.
    *
    * \details asserts if the matrix is rectangular.
    */
   bool SolveEigenvectors(std::vector<std::vector<ScalarType>>& eigenVectors) const;

private:
   unsigned int rows;
   unsigned int columns;
   std::vector<ScalarType> values;

   void MakeRowEchelon(unsigned int col, bool reduce);
   void ReducePivot(unsigned int col);
   void OperateOnRow(unsigned int rowToChange, ScalarType multipleOnRowToChange, unsigned int rowToUse, ScalarType multipleOnRowToUse);
   void ScalarMultiplyRow(unsigned int rowToChange, ScalarType multipleOnRowToChange);

   void ReDimension(unsigned int newRows, unsigned int newColumns);

   static void ReDimensionFrom(std::vector<ScalarType>& matrixValues, const Matrix& matrix, unsigned int newRows, unsigned int newColumns);

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

} // namespace Locus