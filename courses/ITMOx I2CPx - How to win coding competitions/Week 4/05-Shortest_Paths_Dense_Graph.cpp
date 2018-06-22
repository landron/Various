/*
   Week 4, 5

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 4 > 4th Week Problems > Shortest Path in a Dense Graph
   https://goo.gl/9v5ccc

   By rapport to 4:
      "it works in Θ(N2log⁡N) for dense graphs (here N=|V|). A naive implementation would do the same in Θ(N2)."

   \todo 
   - "In common presentations of Dijkstra's algorithm, initially all nodes are entered into the priority queue. 
      This is, however, not necessary: the algorithm can start with a priority queue that contains only one item, 
      and insert new items as they are discovered"

   03.11.2016  /  "You have used 1 of 200 attempts" / "Time limit exceeded, test #25"
                     => solution 2
                  "You have used 3 of 200 attempts" / "Wrong answer, test #12"
                  "You have used 4 of 200 attempts" / "Wrong answer, test #17"
                  "You have used 5 of 200 attempts" / "Time limit exceeded, test #23"
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <limits>
#include <vector>
#include <set>
#include <list>

#define TO_SUBMIT 0

//distances: 6 bytes needed
#if TO_SUBMIT
typedef long long Distance;
#else
#include "Tools.h"
using namespace adjacency_matrix;
#endif

/* Time limit exceeded, test #25 
      "it works in Θ(N2log⁡N) for dense graphs (here N=|V|). A naive implementation would do the same in Θ(N2)."
*/
namespace solution_1
{
   static
   bool Dijskstra(const int** mat, unsigned N, unsigned S, unsigned F, Distance& distSF)
   {
      typedef std::vector<Distance> ContDistances;

      ContDistances dist;
      const auto MAX_DISTANCE = std::numeric_limits<Distance>::max();
      dist.resize(N, MAX_DISTANCE);
      dist[S] = 0;

      struct CompareDistances
      {
         CompareDistances(const ContDistances& v_): m_v(v_) {}

         bool operator() (unsigned f, unsigned s) const
         {
            if (m_v[f] == m_v[s])
               return f < s;
            return m_v[f] < m_v[s];
         }

         const ContDistances& m_v;
      };
      typedef std::set<unsigned, CompareDistances> ContDistancesToProcess;

      CompareDistances comp(dist);
      ContDistancesToProcess work(comp);
      for (size_t i = 0; i < N; ++i)
         work.insert(i);

      while (!work.empty())
      { 
         auto next = *work.cbegin();
         work.erase(work.cbegin());

         for (unsigned i = 0; i < N; ++i)
         {
            if (i == next)
            {
               assert(0 == mat[i][next]);
               continue;
            }
            // no edge
            else if (mat[next][i] == -1)
               continue;
            assert(0 <= mat[next][i]);

            if (dist[i] > dist[next] + mat[next][i])
            {
               work.erase(i);
               dist[i] = dist[next] + mat[next][i];
               work.insert(i);
            }
         }

         if (F == next)
            break;
      }

      if (dist[F] == MAX_DISTANCE)
         return false;

      distSF = dist[F];

      return true;
   }
}

// copy of adjacency_matrix::Dijsktra
namespace solution_2
{
   static
   bool Dijskstra(int* const* mat, unsigned N, unsigned S, unsigned F, Distance& distSF)
   {
      typedef std::vector<Distance> ContDistances;

      const auto MAX_DISTANCE = std::numeric_limits<Distance>::max();
      ContDistances dist;
      dist.resize(N, MAX_DISTANCE);
      dist[S] = 0;

      ContDistances queue;
      queue.resize(N, MAX_DISTANCE);
      queue[S] = 0;

      while (true)
      {
         unsigned next = 0;
         for (size_t i = 1; i < N; ++i)
         {
            if (queue[i] < queue[next])
               next = i;
         }
         if (queue[next] == MAX_DISTANCE)
            break;
         queue[next] = MAX_DISTANCE;
         if (next == F)
            break;

         for (size_t j = 0; j < N; ++j)
         {
            if (next == j)
            {
               assert(0 == mat[next][j]);
               continue;
            }
            // no edge
            else if (mat[next][j] == -1)
               continue;
            assert(0 <= mat[next][j]);

            if (dist[j] > dist[next] + mat[next][j])
            {
               dist[j] = dist[next] + mat[next][j];
               queue[j] = dist[j];
            }
         }
      }

      if (dist[F] == MAX_DISTANCE)
         return false;

      distSF = dist[F];

      return true;
   }
}

static inline
bool solve(int* const* mat, unsigned N, unsigned S, unsigned F, Distance& distSF)
{
#if TO_SUBMIT
   return solution_2::Dijskstra(mat, N, S, F, distSF);
#else
   return adjacency_matrix::Dijskstra(mat, N, S, F, distSF);
#endif
}

static
void shortest_paths()
{
   FILE* in = fopen("dense.in", "r");
   assert(in);
   FILE* out = fopen("dense.out", "w");
   assert(out);

   unsigned N=0, S=0, F=0;
   auto res = fscanf(in, "%u %u %u", &N, &S, &F);
   assert(res == 3);
   --S, --F;
   assert(S < N && F < N);

   int** mat = new int*[N];
   for (unsigned i = 0; i < N; ++i)
   {
      mat[i] = new int[N];
      for (unsigned j = 0; j < N; ++j)
      {
         res = fscanf(in, "%d ", &(mat[i][j]));
         assert(res == 1);
      }
   }  

   Distance distSF = 0;
   if (solve(mat, N, S, F, distSF))
      fprintf(out, "%lld", distSF);
   else
      fprintf(out, "-1");

   res = fclose(out);
   assert(res == 0);
}

//int main()
//{
//   shortest_paths();
//   return 0;
//}