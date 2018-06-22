/*
   Week 3, 2

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 3 > 3rd Week Problems > Inversions
   https://goo.gl/pnAouJ

   Tries
      19.11.2016  /  "You have used 1 of 200 attempts" / "Wrong answer, test #26"
                     "You have used 4 of 200 attempts" : the number of inversions was too big
*/

// error C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead.
#define _CRT_SECURE_NO_WARNINGS
// error C4996: 'std::_Copy_n': Function call with parameters that may be unsafe - this call relies 
//    on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
#define _SCL_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
#include <array>
#include <algorithm>
#include <assert.h>

typedef std::vector<int>   Cont;       // -/+ 10^9 maxim number
typedef unsigned long long SumType;    // but this sum can exceed max int

static
void Print(FILE* out, const Cont& src, unsigned s, unsigned e)
{
   fprintf(out, "%u %u %d %d\n", s+1, e+1, src[s], src[e]);
}

static inline 
void Inversions(unsigned src, unsigned dst, SumType& inversions)
{
   inversions += (src > dst) ? (src-dst) : (dst-src);
}

static inline
void Merge(Cont& src, Cont& aux, unsigned s, unsigned m, unsigned e, SumType& inversions_out)
{
   SumType inversions = 0;

   for (unsigned i = s, j = m, k = s; k != e+1; ++k)
   {
      if (src[i] <= src[j])
      {
         aux[k] = src[i];
         Inversions(i, k, inversions);
         ++i;
         if (i == m)
         {
            ++k;
            for (; k != e+1; ++k, ++j)
            {
               aux[k] = src[j];
               Inversions(j, k, inversions);
            }
            break;
         }
      }
      else
      {
         aux[k] = src[j];
         Inversions(j, k, inversions);
         ++j;
         if (j == e+1)
         {
            ++k;
            for (; k != e+1; ++k, ++i)
            {
               aux[k] = src[i];
               Inversions(i, k, inversions);
            }
            break;
         }
      }
   }

   std::copy_n(&aux[s], e-s+1, &src[s]);

   assert(inversions%2 == 0);
   inversions_out += inversions/2;
}

static 
void merge_sort(Cont& src, Cont& aux, unsigned s, unsigned e, SumType& inversions)
{
   assert(s <= e);

   switch (e - s)
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

            ++inversions;
         }
      }
      break;

      default:
      {
         const unsigned m = s+(e-s)/2+1;
         //const unsigned m = e/2+s/2+1;

         merge_sort(src, aux, s, m-1, inversions);
         merge_sort(src, aux, m, e, inversions);

         Merge(src, aux, s, m, e, inversions);
      }
      break;
   }
}

static 
int  merge_sort()
{
   FILE* in = fopen("inversions.in", "r");
   if (!in)
      return -1;
   FILE* out = fopen("inversions.out", "w");
   if (!out)
      return -2;

   unsigned n;
   auto res = fscanf (in, "%u", &n);
   assert(res == 1);

   Cont v, u;
   v.reserve(n);
   u.resize(n);

   for (unsigned i = 0; i < n; ++i)
   {
      int ai;
      res = fscanf (in, "%d", &ai);
      assert(res == 1);

      v.push_back(ai);
   }

   SumType inversions = 0;
   merge_sort(v, u, 0, n-1, inversions);

   fprintf(out, "%llu", inversions);

   res = fclose(out);
   assert(res == 0);

   return 0;
}

static inline
unsigned Inversions(const int* in, size_t n)
{
   Cont v(in, in+n), u;
   u.resize(v.size());

   SumType inversions = 0;
   merge_sort(/*static_cast<FILE*>(nullptr),*/ v, u, 0, v.size()-1, inversions);
   return static_cast<unsigned>(inversions);
}

void Debug_validations()
{
  const int v1[] = {7, 5, 16, 8};
  assert(2 == Inversions(v1,4));
  const int v2[] = {3, 2, 1, 1};
  assert(5 == Inversions(v2,4));
  const int v3[] = {3, 2, 1, 1, 2, 3};
  assert(6 == Inversions(v3,6));
  const int v4[] = {2, 1, 2, 1, 4};
  assert(3 == Inversions(v4,5));
  const int v5[] = {2, 1, 2, 1, 4, 2};
  assert(4 == Inversions(v5,6)); 
}

//int main()
//{
//   Debug_validations();
//   return merge_sort();
//}