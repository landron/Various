/*
   Week 4, 4

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 4 > 4th Week Problems > Shortest Paths in a Sparse Graph
   https://goo.gl/ymyHTf

   03.11.2016  /  "You have used 2 of 200 attempts" / "Wrong answer, test #14"
                  - not because of overflow ("unsigned long long" still fails)
                  "You have used 6 of 200 attempts" / "Time limit exceeded, test #39
                  "You have used 16 of 200 attempts" / Accepted.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <algorithm>
#include <limits>

#define TO_SUBMIT 0

#if TO_SUBMIT
typedef unsigned long Distance;
struct Vertex
{
   Distance distance;
   std::list<std::pair<unsigned, Distance>> edges;
};
typedef std::vector<Vertex> ContDistances;
#else
#include "Tools.h"
using namespace adjacency_list;
#endif

void pre()
{
   static_assert(sizeof(unsigned long) == 4, "");
   static_assert(sizeof(unsigned) == 4, "");
   static_assert(4*(10^9) < (2^32), "");
}

// "Time limit exceeded, test #39
namespace solution_1
{
   static 
   void Dijsktra(unsigned M, ContDistances& v)
   {
      pre();

      // "Time limit exceeded, test #39
      typedef std::multimap<Distance, unsigned> ContDistancesToProcess; 

      v[0].distance = 0;

      ContDistancesToProcess work;
      // "Wrong answer, test #14": necessary to have all the nodes, even those with infinite distances
      for (size_t i = 1; i < v.size(); ++i)
         work.insert(ContDistancesToProcess::value_type(v[i].distance, i));
      work.insert(ContDistancesToProcess::value_type(0, 0));

      while (!work.empty())
      { 
         auto next = work.cbegin()->second;
         work.erase(work.cbegin());

         for (auto i : v[next].edges)
         {
            if (v[i.first].distance > v[next].distance + i.second)
            {
               v[i.first].distance = v[next].distance + i.second;
               work.insert(ContDistancesToProcess::value_type(v[i.first].distance, i.first));
            }
         }
      }
   }
};

// copy of adjacency_list::Dijsktra
namespace solution_2
{
   static 
   void Dijsktra(unsigned M, ContDistances& v)
   {
      struct CompareDistances
      {
         CompareDistances(const ContDistances& v_): m_v(v_) {}

         // unambiguous LESS => set is enough (as opposed to multiset)
         bool operator() (unsigned f, unsigned s) const
         {
            // "Wrong answer, test #13": be sure to allow equal values
            if (m_v[f].distance == m_v[s].distance)
               return f < s;
            return m_v[f].distance < m_v[s].distance;
         }

         const ContDistances& m_v;
      };
      typedef std::set<unsigned, CompareDistances> ContDistancesToProcess;

      v[0].distance = 0;

      CompareDistances comp(v);
      ContDistancesToProcess work(comp);
      for (size_t i = 1; i < v.size(); ++i)
         work.insert(i);
      work.insert(0);

      while (!work.empty())
      { 
         auto next = *work.cbegin();
         work.erase(work.cbegin());

         for (auto i : v[next].edges)
         {
            // "Runtime error, test #3"
            //assert (i.first != next);
            if (i.first == next)
               continue;

            if (v[i.first].distance > v[next].distance + i.second)
            {
               work.erase(i.first); // remove it before changing its distance
               v[i.first].distance = v[next].distance + i.second;               
               work.insert(ContDistancesToProcess::value_type(i.first));
            }
         }
      }
   }
};

static inline
void solve(unsigned M, ContDistances& v)
{
#if TO_SUBMIT
   solution_2::Dijsktra(M, v);
#else
   adjacency_list::Dijsktra(M, v);
#endif
}

static
void shortest_paths()
{
   FILE* in = fopen("sparse.in", "r");
   assert(in);
   FILE* out = fopen("sparse.out", "w");
   assert(out);

   unsigned N=0, M=0;
   auto res = fscanf (in, "%u", &N);
   assert(res == 1);
   res = fscanf (in, "%u", &M);
   assert(res == 1);

   ContDistances dist;
   Vertex v_init;
   v_init.distance = std::numeric_limits<Distance>::max();
   dist.resize(N, v_init);

   for (unsigned i = 0; i < M; ++i)
   {
      unsigned s, d, w;
      auto res = fscanf (in, "%u %u %u", &s, &d, &w);
      assert(res == 3);
      --s; --d;

      dist[s].edges.push_back(std::make_pair(d, w));
      dist[d].edges.push_back(std::make_pair(s, w));
   }   

   solve(M, dist);

   assert(0 == dist[0].distance);
   for (auto i : dist)
   {
      assert(v_init.distance != i.distance);
      fprintf(out, "%lu ", i.distance);
   }

   res = fclose(out);
   assert(res == 0);
}

//int main()
//{
//   shortest_paths();
//   return 0;
//}