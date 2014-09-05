//This example shows how to use the MByNIterations class.

#include "Locus/Math/MByNIterations.h"

#include <iostream>

int main(int argc, char** argv)
{
   const size_t NUM_LOOPS = 5;
   const size_t NUM_ITERATIONS_PER_LOOP = 3;

   Locus::MByNIterations mByNIterations(NUM_LOOPS, NUM_ITERATIONS_PER_LOOP);

   // The following prints:
   // 0, 0, 0, 0, 0
   // 0, 0, 0, 0, 1
   // 0, 0, 0, 0, 2
   // 0, 0, 0, 1, 0
   // 0, 0, 0, 1, 1
   // 0, 0, 0, 1, 2
   // 0, 0, 0, 2, 0
   // ...
   do
   {
      std::cout << mByNIterations.GetElement(0);

      for (size_t index = 1; index < NUM_LOOPS; ++index)
      {
         std::cout << ", " << mByNIterations.GetElement(index);
      }

      std::cout << std::endl;
   } while (mByNIterations.GenerateNext());

   return 0;
}