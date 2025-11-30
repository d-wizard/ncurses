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
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <type_traits>
#include <limits>
#include <math.h>
#include <stdio.h>
#include <list>
#include <memory>

class DataTypeScoreInterface
{
public:
   DataTypeScoreInterface(const std::string& typeNameStr) : m_typeNameStr(typeNameStr) {}
   virtual void addChunk(const void* data, size_t numBytes) = 0;
   virtual float getScore() = 0;
protected:
   std::string m_typeNameStr;
};

template <typename T>
class DataTypeScore: public DataTypeScoreInterface
{
public:
   DataTypeScore(const std::string& typeNameStr)
      : DataTypeScoreInterface(typeNameStr)
      , m_halfMax(std::numeric_limits<T>::max()/2)
   {
   }
   virtual ~DataTypeScore(){}

   void addChunk(const void* data, size_t numBytes) override
   {
      const T* ptr = static_cast<const T*>(data);
      size_t numVals = numBytes / sizeof(T);

      m_numValues += numVals;

      if constexpr(std::is_floating_point_v<T>)
      {
         // float/double. Check for nan and keep pos/neg sums separate (so they don't cancel out).
         for(size_t i = 0; i < numVals; ++i)
         {
            if(!std::isfinite(ptr[i]))
               ++m_nanCount;
            else if(ptr[i] == 0.0)
               ++m_zeroCount;
            else if(ptr[i] > 0.0)
               m_positiveSum += (double)ptr[i];
            else
               m_negativeSum += (double)ptr[i];
         }
      }
      else if constexpr(std::is_signed_v<T>)
      {
         // Keep pos/neg sums separate (so they don't cancel out).
         for(size_t i = 0; i < numVals; ++i)
         {
            if(ptr[i] == 0)
               ++m_zeroCount;
            else if(ptr[i] > 0)
               m_positiveSum += (double)ptr[i];
            else
               m_negativeSum += (double)ptr[i];
         }
      }
      else
      {
         // Just sum.
         for(size_t i = 0; i < numVals; ++i)
         {
            if(ptr[i] == 0)
               ++m_zeroCount;
            else
               m_positiveSum += (double)ptr[i];
         }
      }
   }

   float getScore() override
   {
      if(m_numValues == 0)
         return 0;

      double nonZeroAbsSum = m_positiveSum - m_negativeSum;
      double finiteCount = double(m_numValues - m_nanCount);
      double avg = nonZeroAbsSum / finiteCount;
      double howFarFromAvg = abs(avg - m_halfMax) / m_halfMax;

      printf("--- %s ------------------------\n", m_typeNameStr.c_str());
      printf("Compare to rand avg = %.6lf\n", howFarFromAvg);
      printf("Zero Percent = %.3lf\n", double(m_zeroCount) / double(m_numValues));
      printf("Nan Percent = %.3lf\n\n", double(m_nanCount) / double(m_numValues));
      return 0;
   }

private:
   const double m_halfMax;

   uint64_t m_numValues = 0;
   double m_positiveSum = 0;
   double m_negativeSum = 0;

   uint64_t m_nanCount = 0;
   uint64_t m_zeroCount = 0;
};

////////////////////////////////////////////////////////////////////////////////

class DetermineDataType
{
public:
   DetermineDataType()
   {
      m_types.emplace_back(std::make_unique<DataTypeScore<uint8_t >>("uint8_t"));
      m_types.emplace_back(std::make_unique<DataTypeScore<int8_t  >>("int8_t"));
      m_types.emplace_back(std::make_unique<DataTypeScore<uint16_t>>("uint16_t"));
      m_types.emplace_back(std::make_unique<DataTypeScore<int16_t >>("int16_t"));
      m_types.emplace_back(std::make_unique<DataTypeScore<uint32_t>>("uint32_t"));
      m_types.emplace_back(std::make_unique<DataTypeScore<int32_t >>("int32_t"));
      m_types.emplace_back(std::make_unique<DataTypeScore<uint64_t>>("uint64_t"));
      m_types.emplace_back(std::make_unique<DataTypeScore<int64_t >>("int64_t"));
      m_types.emplace_back(std::make_unique<DataTypeScore<float   >>("float"));
      m_types.emplace_back(std::make_unique<DataTypeScore<double  >>("double"));
   }

   virtual ~DetermineDataType()
   {
   }

   void addChunk(const void* data, size_t numBytes)
   {
      for(auto& t : m_types)
      {
         t->addChunk(data, numBytes);
      }
   }

   std::string getBestType()
   {
      for(auto& t : m_types)
      {
         t->getScore();
      }
      return "";
   }

private:
   std::list<std::unique_ptr<DataTypeScoreInterface>> m_types;
};
