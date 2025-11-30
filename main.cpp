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
#include "NcursesProgressBar.h"

int main()
{
   { // Make scope for NcursesProgressBar
      int progBar_maxProgress = 64;
      NcursesProgressBar bar(progBar_maxProgress);

      for (int i = 0; i <= progBar_maxProgress; ++i)
      {
         bar.update(i, "Status Text - " + std::to_string(i));
         std::this_thread::sleep_for(std::chrono::milliseconds(250));
      }
   }

   std::this_thread::sleep_for(std::chrono::milliseconds(5000));

   return 0;   // Destructor handles cleanup
}