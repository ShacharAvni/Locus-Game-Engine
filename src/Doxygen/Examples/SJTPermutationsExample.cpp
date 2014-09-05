//This example shows how to use the SJTPermutations class.

#include "Locus/Math/SJTPermutations.h"

#include <iostream>

int main(int argc, char** argv)
{
   const int NUM_ELEMENTS = 5;

   Locus::SJTPermutations sjtPermutations(NUM_ELEMENTS);

   do
   {
      std::cout << sjtPermutations.GetElement(0);

      for (int index = 1; index < NUM_ELEMENTS; ++index)
      {
         std::cout << ", " << sjtPermutations.GetElement(index);
      }

      std::cout << std::endl;
   } while (sjtPermutations.GenerateNext());

   return 0;
}