/*
   Week 3, 6

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 3 > 3rd Week Problems > Fence Painting
   https://goo.gl/5mslvV

   20.11.2016  /  "You have used 1 of 200 attempts"  / "Runtime error, test #25"
                  "You have used 3 of 200 attempts"  / "Time limit exceeded, test #25"
                        no quicksort assertions
                  "You have used 8 of 200 attempts"  / "Accepted"
                        various improvements & corrections 
                        solved in the last minute: ugly infinite recursion?
*/

// error C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
#include <assert.h>

typedef std::vector<unsigned> Cont;

static 
void quick_sort_step(Cont& src, const unsigned s, const unsigned e, unsigned& sn, unsigned& en)
{
   assert(s < e+1);
   assert(0 == sn && 0 == en);
   
   const auto m = s+(e-s)/2;
   const auto pivot = src[m];

   unsigned i = s, j = e;
   for (;;++i, --j)
   {
      for (; src[i] < pivot; ++i);
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
void quick_sort(Cont& src, const unsigned s, const unsigned e)
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

         if (s < sn) // "s != sn"   should work
            quick_sort(src, s, sn);
         if (en < e) // "en != e"   should work
            quick_sort(src, en, e);
      }
      break;
   }
}

namespace solution_1
{
   static
   unsigned MaxSatisfaction(const Cont& v, unsigned pos, unsigned painted, const unsigned planks, const unsigned max, 
                              bool& diminishMaxAllowed)
   {
      assert(painted < planks);

      const auto n = v.size();
      assert(pos < n);
      assert(0 != max);

      auto to_paint = max;
      if (v[pos] >= max+painted) 
         to_paint = v[pos]-painted;
      else
         diminishMaxAllowed = true;
      assert(to_paint >= max);

      if (painted+to_paint >= planks)
      {
         if (pos != n-1)
            return 0;

         // this was the (main) problem for "Time limit exceeded, test #25"
         if (painted+to_paint == planks)  // perfect solution
            return max;

         diminishMaxAllowed = true;
         //return (planks-painted);   // this works, but it seems like an error
         return 0;
      }

      painted += to_paint;
      ++pos;

      if (pos == n)
         return max;
      return MaxSatisfaction(v, pos, painted, planks, max, diminishMaxAllowed);
   }

   static
   unsigned MaxSatisfaction(const Cont& v, const unsigned planks, unsigned max)
   {
      unsigned calculated_max = 0;
      for (;max; max = (calculated_max > 0) ? calculated_max : --max)
      {
         bool diminishMaxAllowed = false;
         calculated_max = MaxSatisfaction(v, 0, 0, planks, max, diminishMaxAllowed);
         assert(calculated_max <= max);
         if (calculated_max == max)
            break;
         if (!diminishMaxAllowed)
            return 0;
      }
      return max;
   }

   static
   unsigned MaxSatisfaction(const Cont& v, unsigned planks)
   {
      const auto n = v.size();

      unsigned max = planks/n;
      if (max > v.front())
         max = v.front();

      if (max == 0)
         return max;

      // optimization: not needed for a correct solution, but it works with
      if (0)
      if (n > 1)
      {
         // last two > planks => 0
         if (v[n-1] >= planks && v[n-2] >= planks)
            return 0;

         for (size_t i = 2; i < n; ++i)
         {
            const auto max_new = (planks - v[n-i])/(i-1);
            if (max_new < max)
            {
               max = max_new;
               if (max == 0)
                  return max;
            }
         }
      }
      
      return MaxSatisfaction(v, planks, max);
   }
};

namespace solution_wip
{
static
unsigned MaxSatisfaction(const Cont& src, unsigned planks)
{
   unsigned max = planks/src.size();
   if (max >= src.back())
      return src.front();

   unsigned long long sum  = 0;
   for (auto i : src)
   {
      sum += i;
      if (sum > planks)
         break;
   }
   if (sum <= planks)
      return src.front();
}
};

static
int fence_painting()
{
   FILE* in = fopen("fence.in", "r");
   if (!in)
      return -1;
   FILE* out = fopen("fence.out", "w");
   if (!out)
      return -2;

   unsigned n=0, k=0;
   auto res = fscanf (in, "%u", &n);
   assert(res == 1);
   res = fscanf (in, "%u", &k);
   assert(res == 1);

   Cont v;
   v.reserve(n);

   for (unsigned i = 0; i < n; ++i)
   {
      int ai;
      res = fscanf (in, "%d", &ai);
      assert(res == 1);

      v.push_back(ai);
   }

   quick_sort(v, 0, n-1);
   const auto max = solution_1::MaxSatisfaction(v, k);

   fprintf(out, "%u", max);

   res = fclose(out);
   assert(res == 0);

   return 0;
}

//int main()
//{
//   return fence_painting();
//}