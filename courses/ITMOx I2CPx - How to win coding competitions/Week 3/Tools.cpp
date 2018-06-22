
/*
   latest quick sort variant:
   - not really exported as function (it should have a template)
   - tested with "06-FencePainting.cpp"
   - a more generic version in "10-ScarecrowSort.cpp.cpp"
*/

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
