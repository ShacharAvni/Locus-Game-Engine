/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#include "Locus/Math/Polynomial.h"
#include "Locus/Math/ComplexUtil.h"

#include "Locus/Common/Float.h"

#include <algorithm>

#include <assert.h>

namespace Locus
{

template <typename ScalarType>
Polynomial<ScalarType>::Polynomial(unsigned int degree)
   : degree(degree), values(degree + 1)
{
}

template <typename ScalarType>
const ScalarType& Polynomial<ScalarType>::operator[](unsigned int index) const
{
   assert(index < degree + 1);
   return values[index];
}

template <typename ScalarType>
ScalarType& Polynomial<ScalarType>::operator[](unsigned int index)
{
   assert(index < degree + 1);
   return values[index];
}

template <typename ScalarType>
unsigned int Polynomial<ScalarType>::GetDegree() const
{
   return degree;
}

template <typename ScalarType>
void Polynomial<ScalarType>::ForceDegree(unsigned int newDegree)
{
   values.resize(newDegree + 1);

   if (Float::FIsZero<ScalarType>(values[newDegree]))
   {
      values[newDegree] = 1;
   }

   degree = newDegree;
}

template <typename ScalarType>
bool Polynomial<ScalarType>::IsNull() const
{
   for (unsigned int termIndex = 0; termIndex < degree + 1; ++termIndex)
   {
      if (Float::FNotZero<ScalarType>(values[termIndex]))
      {
         return false;
      }
   }

   return true;
}

template <typename ScalarType>
std::complex<ScalarType> Polynomial<ScalarType>::ValueAt(const std::complex<ScalarType>& x) const
{
   std::complex<ScalarType> value;

   std::complex<ScalarType> xToTheN = static_cast<ScalarType>(1);

   for (unsigned int coefficientIndex = 0; coefficientIndex <= degree; ++coefficientIndex)
   {
      value += (values[coefficientIndex] * xToTheN);
      xToTheN *= x;
   }

   return value;
}

template <typename ScalarType>
ScalarType Polynomial<ScalarType>::ValueAt(ScalarType x) const
{
   std::complex<ScalarType> complexValueAt = ValueAt(std::complex<ScalarType>(x, 0));

   return complexValueAt.real();
}

template <typename ScalarType>
Polynomial<ScalarType> Polynomial<ScalarType>::Derivative() const
{
   if (degree == 0)
   {
      return Polynomial<ScalarType>(0);
   }
   else
   {
      const unsigned int degreeOfDerivative = (degree - 1);

      Polynomial<ScalarType> derivative(degreeOfDerivative);
      for (unsigned int coefficientIndex = 0; coefficientIndex <= degreeOfDerivative; ++coefficientIndex)
      {
         derivative[coefficientIndex] = ( (coefficientIndex + 1) * values[coefficientIndex + 1] );
      }

      return derivative;
   }
}

template <typename ScalarType>
bool Polynomial<ScalarType>::Solve(std::vector<std::complex<ScalarType>>& roots) const
{
   switch(degree)
   {
   case 1:
      return SolveLinear(roots);

   case 2:
      return SolveQuadratic(roots);

   case 3:
      return SolveCubic(roots);

   case 4:
      return SolveQuartic(roots);

   default:
      //TODO: Get Newton's method working with complex numbers
      return false;
   }
}

template <typename ScalarType>
bool Polynomial<ScalarType>::Solve(std::vector<ScalarType>& roots) const
{
   if (degree > 4)
   {
      return SolveWithNewtonsMethod(roots);
   }

   std::vector<std::complex<ScalarType>> potentiallyComplexRoots;

   if (Solve(potentiallyComplexRoots))
   {
      roots.reserve(potentiallyComplexRoots.size());

      for (const std::complex<ScalarType>& potentiallyComplexRoot : potentiallyComplexRoots)
      {
         if (Float::FIsZero<ScalarType>(potentiallyComplexRoot.imag()))
         {
            roots.push_back(potentiallyComplexRoot.real());
         }
      }

      return true;
   }

   return false;
}

template <typename ScalarType>
const std::array<std::complex<ScalarType>, 3>& Polynomial<ScalarType>::CubicRootsOfUnity()
{
   static const ScalarType rootThreeOver2 = sqrt( static_cast<ScalarType>(3) ) / 2;
   static const ScalarType minusAHalf = static_cast<ScalarType>(-1) / 2;

   static const std::array<std::complex<ScalarType>, 3> cubicRootsOfUnity =
   {
      1,
      std::complex<ScalarType>(minusAHalf, rootThreeOver2),
      std::complex<ScalarType>(minusAHalf, -rootThreeOver2)
   };

   return cubicRootsOfUnity;
}

template <typename ScalarType>
bool Polynomial<ScalarType>::SolveLinear(std::vector<std::complex<ScalarType>>& roots) const
{
   if (Float::FIsZero(values[1]))
   {
      return false;
   }

   roots.resize(1);

   roots[0] = (-values[0] / values[1]);

   return true;
}

template <typename ScalarType>
bool Polynomial<ScalarType>::SolveQuadratic(std::vector<std::complex<ScalarType>>& roots) const
{
   if (Float::FIsZero(values[2]))
   {
      return SolveLinear(roots);
   }

   const ScalarType discriminant = ( (values[1] * values[1]) - (4 * values[2] * values[0]) );

   const ScalarType twoA = (2 * values[2]);

   const ScalarType sqrtOfAbsDiscriminant = sqrt(std::abs(discriminant));

   roots.resize(2);

   if (discriminant < 0)
   {
      RealPart(roots[0]) = (-values[1] / twoA);
      RealPart(roots[1]) = roots[0].real();

      ImaginaryPart(roots[0]) = (sqrtOfAbsDiscriminant / twoA);
      ImaginaryPart(roots[1]) = -roots[0].imag();
   }
   else
   {
      roots[0] = ( (-values[1] + sqrtOfAbsDiscriminant) / twoA );
      roots[1] = ( (-values[1] - sqrtOfAbsDiscriminant) / twoA ); 
   }

   return true;
}

template <typename ScalarType>
bool Polynomial<ScalarType>::SolveCubic(std::vector<std::complex<ScalarType>>& roots) const
{
   if (Float::FIsZero(values[3]))
   {
      return SolveQuadratic(roots);
   }

   //http://en.wikipedia.org/wiki/Cubic_function

   const ScalarType D0 = ( (values[2] * values[2]) - (3 * values[3] * values[1]) );
   const ScalarType D1 = ( (2 * values[2] * values[2] * values[2]) - (9 * values[3] * values[2] * values[1]) + (27 * values[3] * values[3] * values[0]) );

   const ScalarType D2 = ( (D1 * D1) - (4 * D0 * D0 * D0) );

   const ScalarType sqrtOfAbsD2 = sqrt(std::abs(D2));

   std::complex<ScalarType> C;

   if (D2 < 0)
   {
      RealPart(C) = (D1 / 2);
      ImaginaryPart(C) = (sqrtOfAbsD2 / 2);
   }
   else
   {
      C = ( (D1 + sqrtOfAbsD2) / 2 );
   }

   C = std::pow(C, 1.0/3.0);

   const std::array<std::complex<ScalarType>, 3>& cubicRootsOfUnity = Polynomial<ScalarType>::CubicRootsOfUnity();

   const ScalarType minusOneOverThreeA = ( -1 / (3 * values[3]) );

   roots.resize(3);

   for (unsigned int rootIndex = 0; rootIndex < 3; ++rootIndex)
   {
      std::complex<ScalarType> rootOfUnityTimesC = (cubicRootsOfUnity[rootIndex] * C);
      roots[rootIndex] = minusOneOverThreeA * (values[2] + rootOfUnityTimesC + (D0 / rootOfUnityTimesC));
   }

   return true;
}

template <typename ScalarType>
bool Polynomial<ScalarType>::SolveQuartic(std::vector<std::complex<ScalarType>>& roots) const
{
   if (Float::FIsZero(values[4]))
   {
      return SolveCubic(roots);
   }

   //http://en.wikipedia.org/wiki/Quartic_function

   const ScalarType D0 = ( (values[2] * values[2]) - (3 * values[3] * values[1]) + (12 * values[4] * values[0]) );
   const ScalarType D1 = ( (2 * values[2] * values[2] * values[2]) - (9 * values[3] * values[2] * values[1]) + (27 * values[3] * values[3] * values[0]) +
                           (27 * values[4] * values[1] * values[1]) - (72 * values[4] * values[2] * values[0]) );

   const ScalarType D2 = ( (D1 * D1) - (4 * D0 * D0 * D0) );

   const ScalarType discriminant = (D2 / -27);

   std::complex<ScalarType> Q;

   if (Float::FNotZero<ScalarType>(discriminant) && Float::FIsZero<ScalarType>(D0))
   {
      Q = D1;
   }
   else if (Float::FIsZero<ScalarType>(discriminant) && Float::FIsZero<ScalarType>(D0))
   {
      Q = 0;
   }
   else
   {
      const ScalarType sqrtOfAbsD2 = sqrt(std::abs(D2));

      if (D2 < 0)
      {
         RealPart(Q) = (D1 / 2);
         ImaginaryPart(Q) = (sqrtOfAbsD2 / 2);
      }
      else
      {
         Q = ( (D1 + sqrtOfAbsD2) / 2 );
      }
   }

   Q = std::pow(Q, 1.0/3.0);

   const ScalarType p = ( ((8 * values[4] * values[2]) - (3 * values[3] * values[3])) / (8 * values[4] * values[4]) );

   std::complex<ScalarType> S;

   if (!Polynomial<ScalarType>::SolveForSInQuarticFormula(values[4], p, D0, Q, S))
   {
      return false;
   }

   const ScalarType q = ( ((values[3] * values[3] * values[3]) - (4 * values[4] * values[3] * values[2]) + (8 * values[4] * values[4] * values[1])) / (8 * values[4] * values[4] * values[4]) );

   const std::complex<ScalarType> qOverS = (q / S);

   std::complex<ScalarType> minus4SSquaredMinus2P = ( (static_cast<ScalarType>(-4) * S * S) - (2 * p) );

   std::complex<ScalarType> halfRoot1 = std::sqrt(minus4SSquaredMinus2P + qOverS) * static_cast<ScalarType>(0.5);
   std::complex<ScalarType> halfRoot2 = std::sqrt(minus4SSquaredMinus2P - qOverS) * static_cast<ScalarType>(0.5);

   const ScalarType minusBOver4A = (-values[3] / (4 * values[4]));

   roots.resize(4);

   roots[0] = minusBOver4A - S + halfRoot1;
   roots[1] = minusBOver4A - S - halfRoot1;
   roots[2] = minusBOver4A + S + halfRoot2;
   roots[3] = minusBOver4A + S - halfRoot2;

   return true;
}

template <typename ScalarType>
bool Polynomial<ScalarType>::SolveForSInQuarticFormula(ScalarType a, ScalarType p, ScalarType D0, const std::complex<ScalarType>& Q, std::complex<ScalarType>& S)
{
   const ScalarType minusTwoThirdsP = ((-2 * p) / 3);

   if (Float::IsZero(Q))
   {
      const ScalarType halfSqrtOfAbsMinusTwoThirdsP = (static_cast<ScalarType>(0.5) * std::sqrt(std::abs(minusTwoThirdsP)));

      if (minusTwoThirdsP < 0)
      {
         ImaginaryPart(S) = halfSqrtOfAbsMinusTwoThirdsP;
      }
      else
      {
         S = halfSqrtOfAbsMinusTwoThirdsP;
      }
   }
   else
   {
      const std::array<std::complex<ScalarType>, 3>& cubicRootsOfUnity = Polynomial<ScalarType>::CubicRootsOfUnity();

      const ScalarType oneOverThreeA = (1 / (3 * a));

      for (unsigned int rootIndex = 0; rootIndex < 3; ++rootIndex)
      {
         std::complex<ScalarType> QTimesRootOfUnity = (Q * cubicRootsOfUnity[rootIndex]);

         S = std::sqrt( minusTwoThirdsP + oneOverThreeA * (QTimesRootOfUnity + (D0 / QTimesRootOfUnity)) ) * static_cast<ScalarType>(0.5);

         if (Float::NotZero(S))
         {
            return true;
         }
      }
   }

   return Float::NotZero(S);
}

template <typename ScalarType>
bool Polynomial<ScalarType>::SolveWithNewtonsMethod(std::vector<ScalarType>& roots) const
{
   Polynomial<ScalarType> derivative = Derivative();

   std::vector<ScalarType> criticalPoints;
   if (!derivative.Solve(criticalPoints))
   {
      return false;
   }

   std::vector<ScalarType> startingPoints;

   const std::size_t numCriticalPoints = criticalPoints.size();

   if (numCriticalPoints > 0)
   {
      std::sort(criticalPoints.begin(), criticalPoints.end());

      startingPoints.reserve(numCriticalPoints);

      startingPoints.push_back(criticalPoints[0] - (Float::FLOAT_BASE_TOLERANCE * 3));

      for (std::size_t criticalPointIndex = 1; criticalPointIndex < numCriticalPoints; ++criticalPointIndex)
      {
         ScalarType lastCriticalPointValue = ValueAt(criticalPoints[criticalPointIndex - 1]);
         ScalarType thisCriticalPointValue = ValueAt(criticalPoints[criticalPointIndex]);

         if ( ((lastCriticalPointValue > 0) && (thisCriticalPointValue < 0)) ||
               ((lastCriticalPointValue < 0) && (thisCriticalPointValue > 0)) )
         {
            startingPoints.push_back( (criticalPoints[criticalPointIndex - 1] + criticalPoints[criticalPointIndex]) / 2 );
         }
      }

      startingPoints.push_back(criticalPoints[numCriticalPoints - 1] + (Float::FLOAT_BASE_TOLERANCE * 3));
   }
   else
   {
      startingPoints.resize(1, 0);
   }

   static const std::size_t Max_Iterations = 100000;

   roots.reserve(degree);

   for (ScalarType startingPoint : startingPoints)
   {
      ScalarType x = startingPoint;

      ScalarType value = ValueAt(x);

      std::size_t iterations = 0;

      do
      {
         ++iterations;

         ScalarType derivativeValue = derivative.ValueAt(x);

         if (Float::IsZero(derivativeValue))
         {
            break;
         }

         x = (x - (value / derivativeValue));

         value = ValueAt(x);
      } while (Float::NotZero(value) && (iterations < Max_Iterations));

      if (Float::Equal<ScalarType>(value, 0, 100 * Float::STANDARD_TOLERANCE))
      {
         roots.push_back(x);
      }
   }

   return true;
}

template class LOCUS_MATH_API Polynomial<float>;
template class LOCUS_MATH_API Polynomial<double>;
template class LOCUS_MATH_API Polynomial<long double>;

}