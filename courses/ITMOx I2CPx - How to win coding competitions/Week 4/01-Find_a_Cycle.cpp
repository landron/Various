/*
   Week 4, 1

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 4 > 4th Week Problems > Find a Cycle
   https://goo.gl/q6rqxr

   26.11.2016  /  "You have used 1 of 200 attempts" / "Wrong answer, test #4"
                  Problem: a loop is a cycle.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <vector>
#include <list>

namespace solution_1
{
   typedef std::vector<std::list<int>> Edges;
   typedef std::list<int> Cycle;

   static 
   bool is_cycle(const Edges& edges, unsigned src, unsigned current, Cycle& cycle)
   {
      for (const auto& i: edges[current])
      {
         if (i == src || is_cycle(edges, src, i, cycle))
         {
            cycle.push_front(i);
            return true;
         }
      }
      return false;
   }

   static
   bool has_cycle(FILE* in, FILE* out, unsigned N, unsigned M)
   {
      Edges edges;
      edges.resize(N);

      Cycle cycle;
      for (unsigned i = 0; i < M; i++)
      {
         unsigned s=0, d=0;
         auto res = fscanf (in, "%u %u", &s, &d);
         assert(res == 2);

         // loop
         if (s == d)
         {
            fprintf(out, "YES\n%u", s);
            return true;
         }

         bool skip = false;
         for (const auto& j: edges[s-1])
         {
            if (d-1 == j)
            {
               skip = true;
               break;
            }
         }
         if (skip)
            continue;

         if (is_cycle(edges, s-1, d-1, cycle))
         {
            fprintf(out, "YES\n");
            cycle.push_front(d-1);
            for (auto j : cycle)
               fprintf(out, "%u ", j+1);
            return true;
         }
         edges[s-1].push_back(d-1);
      }

      fprintf(out, "NO");
      return false;
   }
}

static
int has_cycle()
{
   FILE* in = fopen("cycle.in", "r");
   if (!in)
      return -1;
   FILE* out = fopen("cycle.out", "w");
   if (!out)
      return -2;

   unsigned N=0, M=0;
   auto res = fscanf (in, "%u", &N);
   assert(res == 1);
   res = fscanf (in, "%u", &M);
   assert(res == 1);

   (void)solution_1::has_cycle(in, out, N, M);

   res = fclose(out);
   assert(res == 0);

   return 0;
}

//int main()
//{
//   return has_cycle();
//}