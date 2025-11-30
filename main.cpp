/* Copyright 2025 Dan Williams. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include <thread>
#include <chrono>
#include <random>
#include <type_traits>
#include "NcursesProgressBar.h"
#include "DetermineDataType.h"

template <typename T>
T randomInRange(T minVal, T maxVal)
{
   static std::random_device rd;
   static std::mt19937_64 gen(rd());

   if constexpr (std::is_integral_v<T>)
   {
      std::uniform_int_distribution<T> dist(minVal, maxVal);
      return dist(gen);
   }
   else if constexpr (std::is_floating_point_v<T>)
   {
      std::uniform_real_distribution<T> dist(minVal, maxVal);
      return dist(gen);
   }
   else {
      static_assert(std::is_arithmetic_v<T>,
         "randomInRange<T>: T must be arithmetic (int, float, double, etc).");
   }
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
   typedef uint64_t testType;
   static constexpr testType maxVal = 200;
   static constexpr testType minVal = 0;//-maxVal;

   static constexpr int NUM_VALS = 1e6;
   std::vector<testType> testData(NUM_VALS);
   for(size_t i = 0; i < NUM_VALS; ++i)
   {
      testData[i] = randomInRange(minVal, maxVal);
   }

   DetermineDataType ddt;
   ddt.addChunk(testData.data(), NUM_VALS*sizeof(testType));
   ddt.getBestType();


   return 0;   // Destructor handles cleanup
}