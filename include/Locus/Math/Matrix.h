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
#include <type_traits>

namespace Locus
{

#include "Locus/Preprocessor/BeginSilenceDLLInterfaceWarnings"

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