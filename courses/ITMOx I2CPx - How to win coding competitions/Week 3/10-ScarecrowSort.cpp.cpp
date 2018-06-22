/*
   Week 3, 8

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 3 > 3rd Week Problems > Scarecrow Sort
   https://goo.gl/wfSGHJ
*/

// error C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
#include <assert.h>

// normally, k = 1
static 
void quick_sort_step(std::vector<int>& src, unsigned s, unsigned e, unsigned k, unsigned& sn, unsigned& en)
{
   assert(s < e+k+1);
   assert((e-s)%k == 0);
   assert(0 == sn && 0 == en);
   
   const auto m = s+((e-s)/(2*k))*k;
   const auto pivot = src[m];

   unsigned i = s, j = e;
   for (;; i += k, j -= k)
   {
      for (; i < e+1 && src[i] < pivot; i += k);
      assert(i < e+1);

      for (; src[j] > pivot; j -= k);
      assert(j >= s);

      if (i < j)
         std::swap(src[i], src[j]);
      else
      {
         if (i == j)
         {i += k; j -= k;}
         break;
      }
   }

   sn = j;
   en = i;
}

static 
void quick_sort(std::vector<int>& src, unsigned s, unsigned e, unsigned k)
{
   assert(s <= e);
   assert((e-s)%k == 0);

   switch ((e-s)/k)
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
         quick_sort_step(src, s, e, k, sn, en);
         assert(s <= sn && sn < en && en <= e);

         if (s != sn)
            quick_sort(src, s, sn, k);
         if (e != en)
            quick_sort(src, en, e, k);
      }
      break;
   }
}

static
bool scarecrow_sortable(std::vector<int>&v, unsigned k)
{
   const auto n = v.size();

   // sort each k string of numbers
   for (unsigned i = 0; i < k; i++)
   {
      unsigned e = i+((n-i)/k)*k;
      if (e == n)
         e -= k;
      if (e > n)
         break;
      else if (i < e)
         quick_sort(v, i, e, k);
   }

   // is the vector sorted ?
   for (unsigned  i = 0; i < n-1; ++i)
   {
      if (v[i] > v[i+1])
         return false;
   }

   return true;
}

static
int scarecrow_sortable()
{
   FILE* in = fopen("scarecrow.in", "r");
   if (!in)
      return -1;
   FILE* out = fopen("scarecrow.out", "w");
   if (!out)
      return -2;

   unsigned n=0, k=0;
   auto res = fscanf (in, "%u", &n);
   assert(res == 1);
   res = fscanf (in, "%u", &k);
   assert(res == 1);

   std::vector<int> v;
   v.reserve(n);

   for (unsigned i = 0; i < n; ++i)
   {
      int ai;
      res = fscanf (in, "%d", &ai);
      assert(res == 1);

      v.push_back(ai);
   }

   const auto sortable = scarecrow_sortable(v, k);
   fprintf(out, "%s", sortable ? "YES" : "NO");

   res = fclose(out);
   assert(res == 0);

   return 0;
}

//int main()
//{
//   return scarecrow_sortable();
//}