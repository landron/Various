/*
   Week 3, 4

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 3 > 3rd Week Problems > K-th Ordered Statistic
   https://goo.gl/0VwqGs

   Tries
      03.11.2016  /  "You have used 2 of 200 attempts" / "Time limit exceeded, test #28"
      04.11.2016  /  "You have used 3 of 200 attempts" / "Time limit exceeded, test #3"
                     solution_bad_1
      06.11.2016  /  "You have used 5 of 200 attempts" / "Time limit exceeded, test #28"
                     solution_2, test: using a plain vector
      20.11.2016  /  "You have used 6 of 200 attempts" / "Runtime error, test #9"
                     solution_3: using quick sort to strictly search the wanted interval (k1 - k2)
                        assert(s <= k1 || k2 <= e);
                     "You have used 7 of 200 attempts" / "Accepted"
*/
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <assert.h>

// "Time limit exceeded, test #28"
namespace solution_1
{
   static
   void kth_ordered(std::ofstream& out, unsigned N, unsigned k1, unsigned k2, int A, int B, int C, int a1, int a2)
   {
      typedef std::multiset<int> Container;

      Container order;
      order.insert(a1);
      order.insert(a2);

      int first=a1, second=a2;
      for (size_t i = 0; i < N-2; i++)
      {
         int next = first*A+second*B+C;
         first = second; second = next;
         order.insert(next);
      }

      Container::const_iterator it = order.begin();
      std::advance(it, k1-1);

      for (size_t i = k1; i <= k2; ++i, ++it)
         out << *it << ' ';
   }
}

// "Time limit exceeded, test #3"
namespace solution_bad_1
{
   typedef std::vector<int> Container2;
   struct Limit
   {
      int val;
      size_t size;

      Limit(int a): val(a), size(0)
      {}
   };
   struct Interval
   {
      int min;
      size_t left;
      int max;
      size_t right;

      Interval(int a, int b): left(0), right(0), min(a), max(b)
      {
         if (a > b)
         {
            min = b;
            max = a;
         }
      }
   };

   static
   void ReduceInterval(Interval& M, unsigned limit_left, unsigned limit_right, const Container2& numbers)
   {
      const int middle = M.min + (M.max - M.min)/2;
      Limit left(M.min);
      Limit right(M.max);

      for (auto i : numbers)
      {
         if (M.min <= i)
         {
            if (i < middle)
            {
               left.size++;
               if (i > left.val)
                  left.val = i;
            }
            else if (i <= M.max)
            {
               right.size++;
               if (i < right.val)
                  right.val = i;
            }
         }
      }

      if (limit_right <= M.left+left.size)
      {
         M.max = left.val;
         M.right += right.size;
      }
      else if (limit_left > M.left+left.size)   // strict => it must be in the right
      {
         M.min = right.val;
         M.left += left.size;
      }
      else return;

      ReduceInterval(M, limit_left, limit_right, numbers);
   }

   static
   void kth_ordered(std::ofstream& out, unsigned N, unsigned k1, unsigned k2, int A, int B, int C, int a1, int a2)
   {
      static_assert(4 == sizeof(unsigned), "");

      Container2 numbers;
      numbers.reserve(N);
      numbers.push_back(a1);
      numbers.push_back(a2);

      Interval M(a1, a2);

      int first=a1, second=a2;
      for (size_t i = 0; i < N-2; i++)
      {
         int next = first*A+second*B+C;
         first = second; second = next;
         numbers.push_back(next);

         if (next < M.min)
            M.min = next;
         else if (next > M.max)
            M.max = next;
      }

      ReduceInterval(M, k1, k2, numbers);

      typedef std::multiset<int> Container;
      Container order;
      for (auto i : numbers)
      {
         if (i >= M.min && i <= M.max)
            order.insert(i);
      }
      Container::iterator it = order.begin();
      std::advance(it, k1-M.left-1);
      for (size_t i = k1; i <= k2; ++i, ++it)
         out << *it << ' ';
   }
}

// Time limit exceeded, test #28
namespace solution_2
{
   static
   void kth_ordered(std::ofstream& out, unsigned N, unsigned k1, unsigned k2, int A, int B, int C, int a1, int a2)
   {
      typedef std::vector<int> Container;

      Container buff;
      buff.reserve(N);
      buff.push_back(a1);
      buff.push_back(a2);

      int first=a1, second=a2;
      for (size_t i = 0; i < N-2; i++)
      {
         int next = first*A+second*B+C;
         first = second; second = next;
         buff.push_back(next);
      }

      std::sort(buff.begin(), buff.end());

      for (size_t i = k1-1; i < k2; ++i)
         out << buff[i] << ' ';
   }
}

/* 
   using quick sort to only search the wanted interval (k1 - k2)
      adapted from 01-MergeSort.cpp 
*/
namespace solution_3
{
   static 
      void quick_sort_step(std::vector<int>& src, unsigned s, unsigned e, unsigned& sn, unsigned& en)
   {
      assert(s < e+1);
      assert(0 == sn && 0 == en);

      const auto m = s+(e-s)/2;
      const auto pivot = src[m];

      unsigned i = s, j = e;
      for (;;++i, --j)
      {
         for (; i < e+1 && src[i] < pivot; ++i);
         assert(i < e+1);
         
         for (; src[j] > pivot; --j);
         assert(j >= s);

         if (i < j)
            std::swap(src[i], src[j]);
         else
         {
            if (i == j)
               {i++;--j;}
            break;
         }
      }

      sn = j;
      en = i;
   }

   static 
      void quick_sort(std::vector<int>& src, unsigned k1, unsigned k2, unsigned s, unsigned e)
   {
      assert(s < e);
      assert(k1 <= k2);
      //assert(s <= k1 || k2 <= e);

      switch (e-s)
      {
      case 0:
         break;

      case 1:
         {
            if (src[s] > src[e])
            {
               auto temp = src[s];
               src[s] = src[e];
               src[e] = temp;
            }
         }
         break;

      default:
         {
            unsigned sn = 0, en = 0;
            quick_sort_step(src, s, e, sn, en);
            assert(s <= sn && sn < en && en <= e);

            bool left = true, right = true;
            if (k1 >= en)
               left = false;
            else if (k2 <= sn)
               right = false;

            if (s != sn && left)
               quick_sort(src, k1, k2, s, sn);
            if (e != en && right)
               quick_sort(src, k1, k2, en, e);
         }
         break;
      }
   }

   static
      void kth_ordered(std::ofstream& out, unsigned N, unsigned k1, unsigned k2, int A, int B, int C, int a1, int a2)
   {
      assert(0 < k1 && k1 <= N);
      assert(0 < k2 && k2 <= N);

      typedef std::vector<int> Container;

      Container buff;
      buff.reserve(N);
      buff.push_back(a1);
      buff.push_back(a2);

      int first=a1, second=a2;
      for (size_t i = 0; i < N-2; i++)
      {
         int next = first*A+second*B+C;
         first = second; second = next;
         buff.push_back(next);
      }

      //std::sort(buff.begin(), buff.end());
      quick_sort(buff, k1-1, k2-1, 0, N-1);

      for (size_t i = k1-1; i < k2; ++i)
         out << buff[i] << ' ';
   }
}

// "Time limit exceeded, test #28"
int kth_ordered()
{
   std::ifstream in("kth.in");
   if (!in)
      return -1;
   std::ofstream out("kth.out");
   if (!in)
      return -2;
   unsigned N, k1, k2;
   in >> N >> k1 >> k2;
   if (in.bad())
      return -3;
   int A, B, C, a1, a2;
   in >> A >> B >> C >> a1 >> a2;
   if (in.bad())
      return -4;

   solution_3::kth_ordered(out, N, k1, k2, A, B, C, a1, a2);

   return 0;
}

//int main()
//{
//   return kth_ordered();
//}