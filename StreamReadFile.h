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
#include <stddef.h>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

class StreamReadFile
{
public:
   explicit StreamReadFile(const std::string& filename)
   {
      file.open(filename, std::ios::binary);
      if(!file.is_open())
         throw std::runtime_error("Failed to open file: " + filename);
   }

   virtual ~StreamReadFile()
   {
      if(file.is_open())
         file.close();
   }

   // T is the element type you want to read
   // chunkCount = how many objects of type T to read
   template <typename T>
   size_t readNextChunk(std::vector<T>& outBuffer, size_t chunkCount)
   {
      if(!file.good())
         return 0;

      outBuffer.resize(chunkCount);

      size_t bytesToRead = chunkCount * sizeof(T);
      file.read(reinterpret_cast<char*>(outBuffer.data()), bytesToRead);

      size_t bytesRead = file.gcount();
      size_t itemsRead = bytesRead / sizeof(T);

      // resize to actual items read
      outBuffer.resize(itemsRead);

      return itemsRead;
   }

private:
   // Delete copy, move, default
   StreamReadFile() = delete;
   StreamReadFile(const StreamReadFile&) = delete;
   StreamReadFile& operator=(const StreamReadFile&) = delete;
   StreamReadFile(StreamReadFile&&) = delete;
   StreamReadFile& operator=(StreamReadFile&&) = delete;

   std::ifstream file;
};