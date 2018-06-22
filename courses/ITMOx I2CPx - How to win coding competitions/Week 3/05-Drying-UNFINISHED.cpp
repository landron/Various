/*
   Week 3, 5

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 3 > 3rd Week Problems > Drying
   https://goo.gl/1WM0kk

   "When you use a radiator for a thing, the wetness of this thing reduces by , and the wetness of all other things reduces by 1."

   05.11.2016  /  "You have used 2 of 200 attempts"  / "Wrong answer, test #4"
                     solution 1: "6 7 / 5"   => 3 (2)
                  "You have used 3 of 200 attempts"  / "Wrong answer, test #4"
                     solution 2: "3 1 3 3 / 4" => 2 (3)
                  "You have used 5 of 200 attempts"  / "Wrong answer, test #4"
                     "3 8 / 2" => 4 (3)
                  "You have used 6 of 200 attempts"  / "Wrong answer, test #4"
                     solution 3: "When you use a radiator for a thing, the wetness of this thing reduces by k, 
                                 and the wetness of all other things reduces by 1."
                     "6 7 / 6"      => 2
                     "3 1 3 3 / 4"  => 3
                     "3 8 / 3"      => 3

                     !!! also, clothes can be rearranged (every minute the drying one can be changed)
                  "You have used 8 of 200 attempts"  / "Time limit exceeded, test #7"
                     solution 4
                  "You have used 8 of 200 attempts"  / "Wrong answer, test #4"
                     "1 2 1"  => 1 (2)
                     "1 4 2"  => 3 (2)
                  "You have used 10 of 200 attempts" / "Time limit exceeded, test #16"
                  "You have used 12 of 200 attempts" / "Time limit exceeded, test #18"
                  "You have used 13 of 200 attempts" / "Time limit exceeded, test #16"
                     array added, than removed
                  "You have used 13 of 200 attempts" / "Time limit exceeded, test #18"
                     elements < max eliminated

   Not finished :(                     
   21.11.2016     "Time limit exceeded, test #18"
*/

/*
   Next / todo
    
   1)
   "Your running time is something like Θ(N⋅W⋅log⁡W), where W is the total weight of all clothes. This W can be quite too large - 
   even Θ(W) solution would be too slow. You need to get rid of W appearing as a multiple in your running time - log⁡W is good, 
   however."

   Calculate complexity.

   2) eliminate "already dried" as long as the container is filled up
         Categories (/k): 1, 2, ... k Last one eliminates previous ones if it has more than k elements.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <set>
#include <vector>
#include <algorithm>

#include <fstream>
#include <string> //getline

enum
{
   TEST_ONE = 1,  // use this to submit
   //TEST_FULL,
   TEST_REFERENCE
};

// "6 7 / 5"   => 3 (2)
namespace solution1
{
   static
   int k_best()
   {
      FILE* in = fopen("drying.in", "r");
      if (!in)
         return -1;
      FILE* out = fopen("drying.out", "w");
      if (!out)
         return -2;

      unsigned n;
      auto res = fscanf (in, "%u", &n);
      assert(res == 1);

      typedef std::multiset<unsigned> Cont;
      Cont clothes;
      int total = 0;

      for (unsigned i = 0; i < n; ++i)
      {
         unsigned ai;
         res = fscanf (in, "%u", &ai);
         assert(res == 1);

         clothes.insert(ai);
         total += ai;
      }

      unsigned k;
      res = fscanf (in, "%u", &k);
      assert(res == 1);

      Cont::iterator it = clothes.end();
      --it;
      unsigned minutes = 0;
      unsigned current = *it;
      --it;
      for (unsigned kf = k+1, nf = n-1; total > 0; ++minutes, ++kf)
      {
         if (current > kf)
         {
            total -= (nf+kf);
            current -= kf;
         }
         else
         {
            total -= (nf+current);
            if (total > 0)
            {
               current = *it;
               --it;
               --nf;
            }
         }
      }

      fprintf(out, "%u ", minutes);

      res = fclose(out);
      assert(res == 0);

      return 0;
   }
};

// "3 8 / 2" => 4 (3)
namespace solution2
{
   static
   int k_best(FILE* in, FILE* out)
   {
      unsigned n;
      auto res = fscanf (in, "%u", &n);
      if (EOF == res)
         return EOF;
      assert(res == 1);

      typedef std::multiset<unsigned> Cont;
      Cont clothes;
      //int total = 0;
      unsigned max = 0;

      for (unsigned i = 0; i < n; ++i)
      {
         unsigned ai;
         res = fscanf (in, "%u", &ai);
         assert(res == 1);

         clothes.insert(ai);
         //total += ai;
         if (ai > max)
            max = ai;
      }

      unsigned k;
      res = fscanf (in, "%u", &k);
      assert(res == 1);
      const unsigned kf = k+1;

      unsigned minutes = 0;
      Cont::iterator it = clothes.end();
      for (unsigned nf = n, current = 0; (nf > 0) || (current > 0); ++minutes)
      {
         // eliminate already dried
         const auto dried = clothes.equal_range(minutes);
         //if (0)
         if (dried.second != dried.first)
         {
            const unsigned size = std::distance(dried.first, dried.second);
            if (nf < size)
               break;
            nf -= size;
            if (nf == 0 && 0 == current)
               break;
            
            clothes.erase(dried.first, dried.second);
         }

         if (0 == current)
         {
            assert(nf);
            --nf;

            if (max >= kf+minutes)
            {
               it = clothes.upper_bound(kf+minutes-1);
               if (clothes.end() != it)
               {
                  current = *it;
                  current -= (kf+minutes);

                  clothes.erase(it);
                  it = clothes.end();  // leave something valid here
                  continue;
               }
               else
               {
                  max = 0;
               }
            }
            
            --it;
            current = *it;
         }
         
         // not in the 'for' instruction
         current = (current > kf) ? (current - kf) : 0;
      }

      fprintf(out, "%u\n", minutes);

      return 0;
   }
};

/*
   forum: "When you use a radiator for a thing, the wetness of this thing reduces by , 
      and the wetness of all other things reduces by 1."
   unclear problem statement  :(
*/
namespace solution3
{
   template<typename T>
   struct Greater
   {
      bool operator() (const T& a, const T& b) const
      {
         return a > b;
      }
   };
   // hard to use lower_bound, upper_bound
   typedef std::multiset<unsigned, Greater<unsigned>> Cont;
   //typedef std::multiset<unsigned> Cont;

   static
   int k_best(FILE* in, FILE* out)
   {
      unsigned n;
      auto res = fscanf (in, "%u", &n);
      if (EOF == res)
         return EOF;
      assert(res == 1);

      Cont clothes;

      for (unsigned i = 0; i < n; ++i)
      {
         unsigned ai;
         res = fscanf (in, "%u", &ai);
         assert(res == 1);

         clothes.insert(ai);
      }

      unsigned k;
      res = fscanf (in, "%u", &k);
      assert(res == 1);

      unsigned minutes_total = 0;
      // larger numbers
      Cont::iterator it = clothes.begin();
      {
         //unsigned kf = k+minutes_total+1;
         unsigned kf = k+minutes_total;

         const unsigned target = *it/kf*kf;

         Cont::iterator end = std::upper_bound(it, clothes.end(), target, Greater<unsigned>());
         assert(end != it);
         for (Cont::iterator j = end; j != it;)
         {
            --j;
            if (minutes_total >= *j)
               break;
            unsigned minutes = (*j/kf + (0 == *j%kf ? 0 : 1));
            kf += minutes;
            minutes_total += minutes;
         }

         it = end;
      }
      // see the remaining ones
      for (; it != clothes.end() && minutes_total < *it; ++minutes_total, ++it);

      fprintf(out, "%u\n", minutes_total);

      return 0;
   }
};

/*
   "Every minute Jane can select one thing to dry on the radiator."
   => also, clothes can be rearranged
*/
namespace solution4
{
   template<typename T>
   struct Greater
   {
      bool operator() (const T& a, const T& b) const
      {
         return a > b;
      }
   };
   // hard to use lower_bound, upper_bound
   typedef std::multiset<unsigned, Greater<unsigned>> Cont;
   //typedef std::multiset<unsigned> Cont;

   static
   int fastest_drying(FILE* in, FILE* out)
   {
      unsigned N;
      auto res = fscanf (in, "%u", &N);
      if (EOF == res)
         return EOF;
      assert(res == 1);

      Cont clothes;      
      unsigned max = 0;
      unsigned k = 0;
      //unsigned n = 0;

      // do not insert all the elements:  "Time limit exceeded, test #16" (instead of #18 :()
      if (0)
      {
         std::vector<unsigned> buff;
         buff.reserve(N);

         for (unsigned i = 0; i < N; ++i)
         {
            unsigned ai;
            res = fscanf (in, "%u", &ai);
            assert(res == 1);

            buff.push_back(ai);
         }

         res = fscanf (in, "%u", &k);
         assert(res == 1);

         for (unsigned i = 0; i < N; ++i)
         {
            const unsigned next = buff[i]/k + (buff[i]%k ? 1 : 0);
            if (next > max)
               max = next;
         }

         for (unsigned i = 0; i < N; ++i)
         {
            if (buff[i] <= max)
               continue;

            clothes.insert(buff[i]);
            //++n;
         }
      }
      else
      {
         //n = N;
         assert(!k);
         for (unsigned i = 0; i < N; ++i)
         {
            unsigned ai;
            res = fscanf (in, "%u", &ai);
            assert(res == 1);

            clothes.insert(ai);
         }

         res = fscanf (in, "%u", &k);
         assert(res == 1);

         max = *clothes.begin();
         max = max/k + (max%k ? 1 : 0);

         Cont::iterator it = std::lower_bound(clothes.begin(), clothes.end(), max, Greater<unsigned>());
         //if (clothes.end() != it)
         //   printf("");
         clothes.erase(it, clothes.end());
      }

      assert(k);

      unsigned minutes_total = 0;
      if (clothes.begin() == clothes.end())
      {
         minutes_total = max;
      }
      else if (k == 1)
      {
         minutes_total = *clothes.begin();
      }
      else
      {
         // equality doesn't work !? 
         while (!clothes.empty() && *clothes.begin() > k+minutes_total)
         {
            const unsigned max = *clothes.begin();

            Cont::iterator next = ++clothes.begin();
            if (clothes.end() == next)
               break;
            // -1:   the other also decreases
            const unsigned minutes = (max-*next)/(k-1) + 1;
            assert(minutes);
            minutes_total += minutes;

            clothes.erase(clothes.begin());
            if (max > minutes_total + minutes*(k-1))
               clothes.insert(max-minutes*(k-1));
         }
      
         Cont::iterator it = clothes.begin();
         if ((it != clothes.end()) && (++it == clothes.end())) // it does not pass by the previous loup
         {
            const unsigned max = *clothes.begin();
            if (max > minutes_total)
               minutes_total += (max/k + (max%k ? 1 : 0));
         }
         else
            // see the remaining ones
            for (Cont::iterator it = clothes.begin(); it != clothes.end() && minutes_total < *it; ++minutes_total, ++it);
      }

      fprintf(out, "%u\n", minutes_total);

      return 0;
   }
};

static
int fastest_drying_wrap(const char* filename, unsigned use, int (*fastest_drying)(FILE*, FILE*), bool& validate)
{
   std::string filename_full(filename);
   filename_full += ".in";
   if (TEST_REFERENCE == use)
      filename_full += ".reference";
   FILE* in = fopen(filename_full.c_str(), "r");
   if (!in)
   {
      // allow TEST_REFERENCE to work locally and on server
      if (TEST_REFERENCE != use)
         return -1;
      return fastest_drying_wrap(filename, TEST_ONE, fastest_drying, validate);
   }
   filename_full = filename;
   filename_full += ".out";
   FILE* out = fopen(filename_full.c_str(), "w");
   if (!out)
      return -2;

   auto final_res = fastest_drying(in, out);
   if (final_res)
      return final_res;
   if (TEST_ONE != use)
   {
      size_t count = 0;
      for (auto res = fastest_drying(in, out); 0 == res; res = fastest_drying(in, out), ++count);
      validate = (0 != count);
   }

   final_res = fclose(out);
   assert(final_res == 0);

   return final_res;
}

static 
bool validate_out(const char* filename)
{
   std::string filename_full(filename);
   filename_full += ".out";
   std::ifstream  in(filename_full);
   if (!in)
      return false;
   filename_full += ".validate";
   std::ifstream  validate(filename_full);
   if (!validate)
      return false;

   bool res = true;
   for (unsigned count = 0; res; ++count)
   {
      std::string line1, line2;
      //   Warning	1	warning C4800: 'void *' : forcing value to bool 'true' or 'false' (performance warning)
      bool err = !!std::getline(in, line1);
      if (!err)
      {
         err = !!std::getline(validate, line2);
         return !err;
      }
      err = !!std::getline(validate, line2);
      if (!err)
         return false;
      res = line1 == line2;
      assert(res);
   }

   return res;
}

static
int fastest_drying()
{
   static const auto FILENAME = "drying";

   bool validate = false;
   int res = fastest_drying_wrap(FILENAME, TEST_REFERENCE, solution4::fastest_drying, validate);
   if (res)
      return res;
   if (validate)
   {
      assert(validate_out(FILENAME));
   }
   return 0;
}

//int main()
//{
//   return fastest_drying();
//}