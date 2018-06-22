/*
   Week 3, 3

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 3 > 3rd Week Problems > Anti-Quicksort
   https://goo.gl/lTJXZs

   Tries
      19.11.2016  /  "You have used 2 of 200 attempts" / "Wrong answer, test #3"
                        using "1 3 2" ("2 3 1" is not good)
                     "You have used 3 of 200 attempts" / "Wrong answer, test #4"
                     "You have used 4 of 200 attempts" / "Wrong answer, test #5"
                        corrected: 2,3,4
                        but it seems ... the permutations must be evaluated
                     "You have used 5 of 200 attempts" / "Time limit exceeded, test #10"
                        solution_2 : precised
                     "You have used 8 of 200 attempts" / "Accepted"
                        solution_3 : pattern recognized
*/

// error C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
#include <algorithm>
#include <assert.h>

typedef std::vector<int> Cont;

static 
void quick_sort(Cont& v, unsigned l, unsigned r, unsigned& comparisons)
{
   const auto key = v[l+(r-l)/2];
   int i=l, j=r;
   for (;i <= j;++comparisons)
   {
      for (;v[i]<key;++i, ++comparisons);
      ++comparisons; // previous for
      for (;v[j]>key;--j, ++comparisons);
      ++comparisons; // previous for

      ++comparisons;
      if (i <= j)
      {
         std::swap(v[i], v[j]);
         ++i; --j;
      }
   }
   ++comparisons; // previous for

   comparisons += 2;
   if ((int)l < j)
      quick_sort(v, l, j, comparisons);
   if (i < (int)r)
      quick_sort(v, i, r, comparisons);
}

/* 
   trying a "smart" guess:    "You have used 4 of 200 attempts" / "Wrong answer, test #5"
   works for 2,3,4:
      1 2
      1 3 2
      1 4 2 3
*/
namespace solution1
{
static
void generate_permutation(Cont& v, unsigned s, unsigned e, unsigned max)
{
   assert(s <= e);
   assert(max);

   switch (e-s)
   {
   case 0:
      v[s] = max;
      break;
   case 1:
      {
         v[s] = max-1;
         v[e] = max;
      }
      break;
   default:
      {
         const bool is_interval_odd = 0 != (e-s)%2;
         const auto m = s+(e-s)/2;// + (is_interval_odd ? 1 : 0);

         v[m] = max;

         --max;
         //generate_permutation(v, s, m-1, max);
         //generate_permutation(v, m+1, e, max-(e-s)/2);
         generate_permutation(v, m+1, e, max);
         max -= (e-s)/2;
         max -= (is_interval_odd ? 1 : 0);
         generate_permutation(v, s, m-1, max);         
      };
   }
}

static
void generate_permutation(Cont& v)
{
   const auto n = v.size();
   return generate_permutation(v, 0, n-1, n);
}

};


// precise solution
namespace solution2
{

struct Max
{
   Cont v;
   unsigned comparisons;

   Max(unsigned n): comparisons(0)
   {
      v.resize(n);
   }
};

static
void generate_permutation(Cont& v, Cont& free, unsigned pos, Max& max)
{
   const auto n = v.size();
   assert(n == free.size());

   for (unsigned i = 0; i < n; ++i)
   {
      if (free[i])
         continue;

      free[i] = 1;
      v[pos++] = i+1;

      if (pos == n)
      {
         Cont u(v);
         unsigned comparisons = 0;
         quick_sort(u, 0, n-1, comparisons);
         if (comparisons > max.comparisons)
         {
            max.comparisons = comparisons;
            max.v = v;
         }
      }
      else
      {
         generate_permutation(v, free, pos, max);
      }

      v[--pos] = 0;
      free[i] = 0;
   }
}

static
void generate_permutation(Cont& v)
{
   const auto n = v.size();

   Cont free;
   free.resize(v.size());
   Max max(n);
   generate_permutation(v, free, 0, max);
   v = max.v;
}
};

namespace solution3
{
   static
      void generate_permutation_base(Cont& v, unsigned size)
   {
      assert(size);

      switch (size)
      {
      case 1:
         v[0] = 1;
         break;
      case 2:
         v[0] = 1;
         v[1] = 2;
         break;
      // verified to 9
      default:
         {
            generate_permutation_base(v, size-1);
            const auto is_odd = 0 != size%2;
            const auto m = size/2 - (is_odd ? 0 : 1);
            v[size-1] = v[m];
            v[m] = size;
         }
         break;
      };
   }

   static
      void generate_permutation(Cont& v)
   {
      return generate_permutation_base(v, v.size());
   }

}

static inline
void generate_permutation(Cont& v)
{
   const auto n = v.size();
   //return solution2::generate_permutation(v);
   return solution3::generate_permutation(v);
}

int generate_permutation()
{
   FILE* in = fopen("antiqs.in", "r");
   if (!in)
      return -1;
   FILE* out = fopen("antiqs.out", "w");
   if (!out)
      return -2;

   unsigned n;
   auto res = fscanf (in, "%u", &n);
   assert(res == 1);

   Cont v;
   v.resize(n);

   generate_permutation(v);

   for (auto i : v)
      fprintf(out, "%u ", i);

   res = fclose(out);
   assert(res == 0);

   return 0;
}

static 
unsigned no_repetitions(const Cont& v)
{
   Cont u(v);
   std::sort(u.begin(), u.end());
   for (unsigned i = 0; i < u.size()-1; ++i)
      if (u[i] == u[i+1])
         return i+1;
   return v.size();
}

static inline
unsigned Comparisons(const int* in, size_t n)
{
   Cont v(in, in+n);
   unsigned comparisons = 0;
   quick_sort(v, 0, n-1, comparisons);
   return comparisons;
}

void DebugValidation_no_repetitions(unsigned limit)
{
   for (unsigned i = 1; i < limit; ++i)
   {
      Cont v;
      v.resize(i);
      generate_permutation(v);
      auto res = no_repetitions(v);
      assert(v.size() == res);
   }
}

/*
   1 2
   1 3 2
   1 4 2 3
   1 4 5 3 2
   1 4 6 3 2 5
   1 4 6 7 2 5 3
   1 4 6 8 2 5 3 7
   1 4 6 8 9 5 3 7 2 
*/
void DebugValidation_known_cases()
{
   const int v1[] = {1, 2};
   auto res1 = Comparisons(v1, 2);
   const int v2[] = {2, 1};
   auto res2 = Comparisons(v2, 2);
   assert(res2 < res1);

   const int v3[] = {1, 3, 2};
   res1 = Comparisons(v3, 3);
   const int v4[] = {2, 3, 1};
   res2 = Comparisons(v4, 3);
   assert(res2 < res1);

   const int v4_1[] = {1, 2, 4, 3};
   res1 = Comparisons(v4_1, 4);
   const int v4_2[] = {1, 4, 2, 3};
   res2 = Comparisons(v4_2, 4);
   assert(res1 < res2);
   const int v4_3[] = {1, 4, 3, 2};
   res1 = Comparisons(v4_3, 4);
   assert(res1 < res2);
   const int v4_4[] = {2, 4, 1, 3};
   res1 = Comparisons(v4_4, 4);
   assert(res1 < res2);

   const int v5_1[] = {1, 2, 5, 3, 4};
   res1 = Comparisons(v5_1, 5);
   const int v5_2[] = {1, 5, 2, 3, 4};
   res2 = Comparisons(v5_2, 5);
   assert(res1 < res2);
   const int v5_3[] = {1, 3, 5, 2, 4};
   res1 = Comparisons(v5_3, 5);
   assert(res2 < res1);
   const int v5_4[] = {1, 4, 5, 2, 3};
   res2 = Comparisons(v5_4, 5);
   assert(res2 < res1);
   const int v5_5[] = {1, 4, 5, 3, 2};
   res2 = Comparisons(v5_5, 5);
   assert(res1 < res2);
}

void DebugValidation()
{
   DebugValidation_no_repetitions(25); // too much for solution_2
   DebugValidation_known_cases();
}

//int main()
//{
//   //DebugValidation();
//   return generate_permutation();
//}