/*
Week 3, 7

ITMOx: I2CPx How to win coding competitions: secrets of champions"
Week 3 > 3rd Week Problems > K Best
https://goo.gl/0VwqGs

04.11.2016  /  "You have used 1 of 200 attempts"  / "Wrong answer, test #4"
                  comparison problem (equal fractions included)
               "You have used 2 of 200 attempts"  / "Wrong answer, test #4"
               "You have used 5 of 200 attempts"  / "Presentation error, test #9"
               "You have used 7 of 200 attempts"  / "Time limit exceeded, test #11"
               "You have used 8 of 200 attempts"  / "Time limit exceeded, test #11"
                  no improvement if the scores are firstly sorted: no need to verify a sequence if the score is already worst
05.11.2016  /  "You have used 14 of 200 attempts"  / "Wrong answer, test #4"
                  solutions namespaces: solution_1, solution_2
               "You have used 18 of 200 attempts"  /  "Time limit exceeded, test #11"
                  some troubles to recover this solution:   no best solution initialization ?
               "You have used 21 of 200 attempts"  /  "Time limit exceeded, test #11"
                  solution_2
                  (very) minor improvements:
                  - best solution starts with a value
                  - avoid double division
                  - zero fractions comparisons corrected
                  no improvements:
                  - avoid the initial sorting (and the compare improving)
                  - avoid the use of multiset

               "You have used 24 of 200 attempts"  / "Wrong answer, test #4"
                  no real progress :(
                     solution_2 cannot be improved, solution_1 does not pass test #4

20.11.2016  /  "You have used 27 of 200 attempts"  / "Time limit exceeded, test #11"
*/

/*
   Next / todo
      solution_2 calculates all the partitions, so it is certainly not good
      (calculate complexity ? not needed - this is a brute force solution)

      "Yeah, this does seem to be the hardest problem till week 3. I've been able to solve all others."

      "Can you invent a way to test if you can construct a solution which costs at least alfa?"
      "Some ordering really works. But it is a different ordering. By the way, it depends on the answer."
         posté il y a 13 jours par mbuzdalov Équipe pédagogique
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <set>
#include <vector>
#include <algorithm>

#define NO_DOUBLE_DIVISION 0

struct Jewel
{
   unsigned index;
   unsigned value;
   unsigned weight;

   Jewel(unsigned index_, unsigned value_, unsigned weight_): index(index_), value(value_), weight(weight_)
   {}
};
struct LessJewel
{
   static inline
   bool direct_compare (unsigned v1, unsigned w1, unsigned v2, unsigned w2)
   {
      assert(w1);  // w2 is real => no zero weight

      const auto left = 1.0 * v1 / w1;
      const auto right = 1.0 * v2 / w2;
      if (left != right)
         return left < right;
      else if (left)
         return v1 > v2;
      else
         return w1 > w2;
   }

   static inline
   bool no_double_compare (unsigned v1, unsigned w1, unsigned v2, unsigned w2)
   {
      static_assert(sizeof(unsigned long long) == 8, "");

      unsigned long long left = v1 * w2;
      unsigned long long right = v2 * w1;
      if (left != right)
         return left < right;
      else if (left)
         return v1 > v2;
      else
         return w1 > w2;
   }

   static inline
   bool direct_compare (const Jewel& lhs, const Jewel& rhs)
   {
      return direct_compare(lhs.value, lhs.weight, rhs.value, rhs.weight);
   }

   static inline
   bool no_double_compare (const Jewel& lhs, const Jewel& rhs)
   {
      return no_double_compare(lhs.value, lhs.weight, rhs.value, rhs.weight);
   }

   bool operator() (const Jewel& lhs, const Jewel& rhs) const
   {
#if !NO_DOUBLE_DIVISION
      return direct_compare(lhs, rhs);
#else
      return no_double_compare(lhs, rhs);
#endif
   }
};
typedef std::multiset<Jewel, LessJewel> Cont1;

struct Sequence
   {
      std::vector<unsigned> vals;
      unsigned v, w;

      //Sequence(unsigned k, const Jewel& t): v(0), w(0)
      //{
      //   vals.reserve(k);
      //   add(t);
      //}
      Sequence(unsigned k): v(0), w(0)
      {
         vals.reserve(k);
      }
      void add(const Jewel& t)
      {
         vals.push_back(t.index);
         v += t.value;
         w += t.weight;
      }
      void remove(const Jewel& t)
      {
         vals.pop_back();
         v -= t.value;
         w -= t.weight;
      }

      //double Score() const
      //{
      //   assert(w);
      //   return 1.0 * v / w;
      //}
      // careful !!! equal might be improved
      bool ScoreImproved_double(unsigned v2, unsigned w2)
      {
         //return w ? !LessJewel::direct_compare(v2,w2,v,w) : true;
         return !LessJewel::direct_compare(v2,w2,v,w);
      }
      bool ScoreImproved_no_division(unsigned v2, unsigned w2)
      {
         return !LessJewel::no_double_compare(v2,w2,v,w);
      }
      bool ScoreImproved(const Sequence& left)
      {
#if !NO_DOUBLE_DIVISION
         return ScoreImproved_double(left.v, left.w);
#else
         return ScoreImproved_no_division(left.v, left.w);
#endif
      }
   };

/*
"Wrong answer, test #4"
trying to guess the best solution
*/

namespace solution1
{
   static
      int k_best()
   {
      FILE* in = fopen("k.in", "r");
      if (!in)
         return -1;
      FILE* out = fopen("k.out", "w");
      if (!out)
         return -2;

      unsigned n, k;
      auto res = fscanf (in, "%u %u", &n, &k);
      assert(res == 2);

      // \todo:  only the best k are needed

      Cont1 score;
      for (unsigned i = 0; i < n; ++i)
      {
         unsigned v, w;
         res = fscanf (in, "%u %u", &v, &w);
         assert(res == 2);

         score.insert(Cont1::value_type(i+1, v, w));
      }

      Cont1::const_iterator it = score.cend();
      --it;
      if (0)   // order does not matter
      {
         std::multiset<unsigned> chosen;
         for (unsigned i = 0; i < k; i++, --it)
            chosen.insert(it->index);
         for (auto i : chosen)
            fprintf(out, "%u ", i);
      }
      for (unsigned i = 0; i < k; i++, --it)
         fprintf(out, "%u ", *it);

      res = fclose(out);
      assert(res == 0);

      return 0;
   }
}

/*
Complete solution
"Time limit exceeded, test #11"
*/
namespace solution2
{

   typedef std::vector<Jewel> Cont2;

   static 
      void Print(const Sequence& seq, FILE* out, unsigned k)
   {
      assert(seq.vals.size() == k);   // Runtime error, test #9
      for (auto i : seq.vals)
         fprintf(out, "%u ", i);
      fprintf(out, "\n");
   }

   static
      void k_best_rec(const Cont2& jewels, Sequence& best, Sequence& current, unsigned next, FILE* out, unsigned k)
   {
      if (current.vals.size() == k)
      {
         if (best.ScoreImproved(current))
            best = current;
         return;
      }

      //Print(current, out, current.vals.size());

      for (unsigned i = next; i < jewels.size(); ++i)
      {
         current.add(jewels[i]);
         //if (0)   // improvement if jewels is sorted
         if (!best.ScoreImproved(current))
         {
            current.remove(jewels[i]);
            return;
         }

         k_best_rec(jewels, best, current, i+1, out, k);
         current.remove(jewels[i]);
      }
   }

   /*
      "Time limit exceeded, test #11"
         - do not order the results
   */
   static
      int k_best()
   {
      FILE* in = fopen("k.in", "r");
      if (!in)
         return -1;
      FILE* out = fopen("k.out", "w");
      if (!out)
         return -2;

      unsigned n, k;
      auto res = fscanf (in, "%u %u", &n, &k);
      assert(res == 2);

      typedef std::multiset<Jewel, LessJewel> Cont;
      Cont jewels_ordered;
      Cont2 jewels;
      jewels.reserve(n);

      for (unsigned i = 0; i < n; ++i)
      {
         unsigned v, w;
         res = fscanf (in, "%u %u", &v, &w);
         assert(res == 2);

         //jewels.push_back(Cont2::value_type(i+1,v,w));
         jewels_ordered.insert(Cont::value_type(i+1,v,w));
      }
      jewels.assign(jewels_ordered.crbegin(), jewels_ordered.crend());
      //std::sort(jewels.rbegin(), jewels.rend(), LessJewel());

      Sequence best(k), current(k);
      // initialize best to avoid zero compares
      for (unsigned i = 0; i < k; ++i)
            best.add(jewels[i]);
      k_best_rec(jewels, best, current, 0, out, k);
      if (best.v == 0)
      {
         best.vals.resize(k);
         for (unsigned i = 0; i < k; ++i)
            best.vals[i] = jewels[i].index;
      }

      // the order is not a request, apparently
      if (0)
      {
         std::multiset<unsigned> order_vector;
         for (auto i : best.vals)
            order_vector.insert(i);

         assert(order_vector.size() == k);   // Runtime error, test #9
         for (auto i : order_vector)
            fprintf(out, "%u ", i);
      }
      Print(best, out, k);

      res = fclose(out);
      assert(res == 0);

      return 0;
   }
}

namespace solution3_wip
{
   static
      int k_best()
   {
      FILE* in = fopen("k.in", "r");
      if (!in)
         return -1;
      FILE* out = fopen("k.out", "w");
      if (!out)
         return -2;

      unsigned n, k;
      auto res = fscanf (in, "%u %u", &n, &k);
      assert(res == 2);

      // \todo:  only the best k are needed

      std::vector<Jewel> score;
      score.reserve(n);

      for (unsigned i = 0; i < n; ++i)
      {
         unsigned v, w;
         res = fscanf (in, "%u %u", &v, &w);
         assert(res == 2);

         score.push_back(std::vector<Jewel>::value_type(i+1, v, w));
      }

      Sequence max(k);

      for (unsigned i = 0; i < k; ++i)
      {
         auto it = score.cend(); --it;

         Sequence current(max);
         max.add(*it); 
         
         if (score.cbegin() != it)
         {
            auto j = it; --j;
            for (;; --j)
            {
               current.add(*j);
               if (max.ScoreImproved(current))
               {
                  max.remove(*it);
                  max.add(*j);
                  it = j;
               }
               if (j == score.cbegin())
                  break;
            }
            score.erase(it);
         }
      }

      for (auto i : max.vals)
         fprintf(out, "%u ", i);

      res = fclose(out);
      assert(res == 0);

      return 0;
   }
}

static
int k_best()
{
   return solution2::k_best();
   //return solution1::k_best();
}

//int main()
//{
//   return k_best();
//}