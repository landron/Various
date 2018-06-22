/*
   Week 3, 1

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 3 > 3rd Week Problems > Sorting
   https://goo.gl/pnAouJ
*/

// error C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead.
#define _CRT_SECURE_NO_WARNINGS
// error C4996: 'std::_Copy_n': Function call with parameters that may be unsafe - this call relies 
//    on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
#define _SCL_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
#include <algorithm>
#include <assert.h>

static
void Print(FILE* out, const std::vector<int>& src, unsigned s, unsigned e)
{
   fprintf(out, "%u %u %d %d\n", s+1, e+1, src[s], src[e]);
}

static inline
void Merge(std::vector<int>& src, std::vector<int>& aux, unsigned s, unsigned m, unsigned e)
{
   assert(s < e+1);

   for (unsigned i = s, j = m, k = s; k != e+1; ++k)
   {
      if (src[i] <= src[j])
      {
         aux[k] = src[i];
         ++i;
         if (i == m)
         {
            ++k;
            for (; k != e+1; ++k, ++j)
               aux[k] = src[j];
            break;
         }
      }
      else
      {
         aux[k] = src[j];
         ++j;
         if (j == e+1)
         {
            ++k;
            for (; k != e+1; ++k, ++i)
               aux[k] = src[i];
            break;
         }
      }
   }

   std::copy_n(&aux[s], e-s+1, &src[s]);
}

static 
void merge_sort(FILE* out, std::vector<int>& src, std::vector<int>& aux, unsigned s, unsigned e)
{
   assert(s <= e);

   switch (e - s)
   {
      case 0:
      break;

      case 1:
      {
         Print(out, src, s, s);
         Print(out, src, e, e);

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
         merge_sort(out, src, aux, s, s+(e-s)/2);
         merge_sort(out, src, aux, s+(e-s)/2+1, e);

         Merge(src, aux, s, s+(e-s)/2+1, e);
      }
      break;
   }

   Print(out, src, s, e);
}

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
      //if (i == e+1)
      //{
      //   sn = m; en = m+1;
      //   return;
      //}

      // avoid 0 for unsigned: there cannot be all larger than the pivot
      //for (; j > s && src[j] > pivot; --j);
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
void quick_sort(std::vector<int>& src, unsigned s, unsigned e)
{
   assert(s <= e);

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

         if (s != sn)
            quick_sort(src, s, sn);
         if (e != en)
            quick_sort(src, en, e);
      }
      break;
   }
}

static 
int  merge_sort()
{
   FILE* in = fopen("sort.in", "r");
   if (!in)
      return -1;
   FILE* out = fopen("sort.out", "w");
   if (!out)
      return -2;

   unsigned n;
   auto res = fscanf (in, "%u", &n);
   assert(res == 1);

   std::vector<int> v, u;
   v.reserve(n);
   u.resize(n);

   for (unsigned i = 0; i < n; ++i)
   {
      int ai;
      res = fscanf (in, "%d", &ai);
      assert(res == 1);

      v.push_back(ai);
   }

   merge_sort(out, v, u, 0, n-1);
   //quick_sort(v, 0, n-1);

   for (unsigned i = 0; i < n; ++i)
   {
      fprintf(out, "%d ", v[i]);
   }

   res = fclose(out);
   assert(res == 0);

   return 0;
}

//int main()
//{
//   return merge_sort();
//}