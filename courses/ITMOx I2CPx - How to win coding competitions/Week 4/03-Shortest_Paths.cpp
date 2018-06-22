/*
   Week 4, 3

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 4 > 4th Week Problems > Shortest Paths and Their Friends
   https://goo.gl/oY583I

   03.11.2016  /  "You have used 2 of 200 attempts" / "Wrong answer, test #5"
                  adapted FloydWarshall
                  "You have used 6 of 200 attempts" / "Time limit exceeded, test #8"
                  solution 3
                  "You have used 12 of 200 attempts" / "Time limit exceeded, test #9"

                  solution_4
                     switched to Bellman-Ford
                  "You have used 16 of 200 attempts" / "Wrong answer, test #7"
                  "You have used 24 of 200 attempts" / "Wrong answer, test #15"
                  "You have used 29 of 200 attempts" / "Wrong answer, test #15"

                  solution_5: simplified Bellman-Ford
                  "You have used 45 of 200 attempts" / "Wrong answer, test #9"
                  "You have used 47 of 200 attempts" / "Wrong answer, test #15"

                  solution_BF3
                  "You have used 55 of 200 attempts" / Accepted
                  sum_no_inf added to solve big numbers math
*/

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <limits>
#include <vector>
#include <set>
#include <list>

#define TO_SUBMIT 1

//distances: 6 bytes needed
#if TO_SUBMIT
typedef long long Distance;
struct Path
{
   enum
   {
      NO_PATH = 0,
      SHORTEST_PATH,
      NO_SHORTEST_PATH,
   };
   unsigned type;
   Distance distance;
   int prev;

   Path(): type(NO_PATH), distance(0), prev(-1) {}
};
typedef std::vector<Path> Distances;
const auto MAX_DISTANCE = std::numeric_limits<Distance>::max();
const auto MIN_DISTANCE = std::numeric_limits<Distance>::min();
typedef std::vector<std::vector<Distance>> Matrix;

typedef std::list<std::pair<unsigned, Distance>> NodeEdges;
struct Vertex
{
   Path distance;
   NodeEdges edges;
};
typedef std::vector<Vertex> ContDistances;

struct Edge
{
   unsigned s;
   unsigned d;
   Distance w;
};
typedef std::vector<Edge> Edges;

#else
#include "Tools.h"
using namespace adjacency_matrix;
#endif

//"Time limit exceeded, test #8"
namespace solution_1
{
   static
   bool FloydWarshall(Distance** mat, unsigned N, unsigned S, bool retry, Distances& distS)
   {
      bool cycle = false;

      for (size_t k = 0; k < N; ++k)
      {
         // "Wrong answer, test #5": not only S
         for (size_t i = 0; i < N; ++i)
         {
            for (size_t j = 0; j < N; ++j) 
            {
               if (mat[i][k] == MAX_DISTANCE || mat[k][j] == MAX_DISTANCE)
                  continue;
               if (2 == distS[k].type)
               {
                  distS[j].type = 2;
                  continue;
               }

               if (mat[i][j] > mat[i][k] + mat[k][j])
                  mat[i][j] = mat[i][k] + mat[k][j];
            }
         }
      }

      for (size_t i = 0; i < N; ++i)
      {
         if (retry)
         {
            if (distS[i].type != 1)
               continue;

            if (distS[i].distance > mat[S][i])
            {
               distS[i].type = 2;
               cycle = true;
            }
         }
         else 
         {
            if (MAX_DISTANCE == mat[S][i])
               continue;

            distS[i].type = 1;
            distS[i].distance = mat[S][i];
         }
      }

      return cycle;
   }

   static
   void FloydWarshall_DetectCycles(Distance** mat, unsigned N, unsigned S, Distances& distS)
   {
      for (bool retry = false, cycle = solution_1::FloydWarshall(mat, N, S, retry, distS); !retry || cycle; )
      {
         retry = true;
         cycle = solution_1::FloydWarshall(mat, N, S, retry, distS);
      }
   }
}

// it doesn't work
namespace solution_2
{
   static
   void FloydWarshall(Distance** mat, unsigned N, unsigned S, Distances& distS)
   {
      for (size_t k = 0; k < N; ++k)
      {
         for (size_t i = 0; i < N; ++i)
         {
            // why do we continue ?
            if (mat[i][i] < 0)
            {
               distS[i].type = 2;
               mat[i][i] = MIN_DISTANCE;
            }

            for (size_t j = 0; j < N; ++j) 
            {
               if (i == j || mat[i][k] == MAX_DISTANCE || mat[k][j] == MAX_DISTANCE)
                  continue;

               if (mat[i][j] > mat[i][k] + mat[k][j])
                  mat[i][j] = mat[i][k] + mat[k][j];
            }
         }
      }

      for (size_t i = 0; i < N; ++i)
      {
         if (MAX_DISTANCE == mat[S][i])
            continue;

         distS[i].type = 1;
         distS[i].distance = mat[S][i];
      }
   }

   static
   void FloydWarshall_DetectCycles(Distance** mat, unsigned N, unsigned S, Distances& distS)
   {
      FloydWarshall(mat, N, S, distS);
   }
}

static
void solve_1(FILE* in, unsigned N, unsigned M, unsigned S, Distances& distS)
{
   Distance** mat = new Distance*[N];
   for (unsigned i = 0; i < N; ++i)
   {
      mat[i] = new Distance[N];
      //static_assert(8 == sizeof(Distance), "");
      //std::memset(mat+i, 0, N*sizeof(Distance));
      std::fill_n(mat[i], N, MAX_DISTANCE);
      mat[i][i] = 0;
   } 
   for (unsigned i = 0; i < M; ++i)
   {
      unsigned s,d;
      Distance w;
      auto res = fscanf(in, "%u %u %lld", &s, &d, &w);
      assert(res == 3);

      // keep only the smallest value (multiple endges are possible)
      if (w < mat[s-1][d-1])
         mat[s-1][d-1] = w;
   }

   solution_1::FloydWarshall_DetectCycles(mat, N, S, distS);

   for (unsigned i = 0; i < N; ++i)
      delete [] mat[i];
   delete [] mat;
}

// "Time limit exceeded, test #9"
namespace solution_FW3
{
   static
   void FloydWarshall(Matrix& mat, unsigned N, unsigned S, Distances& distS)
   {
      for (size_t k = 0; k < N; ++k)
      {
         for (size_t i = 0; i < N; ++i)
         {
            // Wrong answer, test #7
            //if (mat[i][i] < 0)
            //   continue;
            if (mat[i][k] == MAX_DISTANCE)
               continue;

            for (size_t j = 0; j < N; ++j) 
            {
               if (mat[k][j] == MAX_DISTANCE)
                  continue;

               // Wrong answer, test #7
               //if (mat[i][i] < 0)
               //{
               //   mat[j][j] = MIN_DISTANCE;
               //   continue;
               //}

               if (mat[i][j] > mat[i][k] + mat[k][j])
                  mat[i][j] = mat[i][k] + mat[k][j];
            }
         }
      }

      for (size_t i = 0; i < N; ++i)
      {
         if (MAX_DISTANCE == mat[S][i])
            continue;
         if (mat[i][i] < 0)
         {
            distS[i].type = 2;
            continue;
         }

         distS[i].type = 1;
         distS[i].distance = mat[S][i];
      }
   }

   static
   void FloydWarshall_DetectCycles(Matrix& mat, unsigned N, unsigned S, Distances& distS)
   {
      FloydWarshall(mat, N, S, distS);
   }

   static
   void solve(FILE* in, unsigned N, unsigned M, unsigned S, Distances& distS)
   {
      Matrix mat;
      mat.resize(N);
      for (size_t i = 0; i < N; ++i)
      {
         mat[i].resize(N, MAX_DISTANCE);
         mat[i][i] = 0;
      }
      for (unsigned i = 0; i < M; ++i)
      {
         unsigned s,d;
         Distance w;
         auto res = fscanf(in, "%u %u %lld", &s, &d, &w);
         assert(res == 3);

         // keep only the smallest value (multiple endges are possible)
         if (w < mat[s-1][d-1])
            mat[s-1][d-1] = w;
      }

      FloydWarshall_DetectCycles(mat, N, S, distS);
   }
}


// switched to Bellman-Ford:  "Wrong answer, test #15"
namespace solution_BF1
{
   static
   bool SetNegativeCycle(ContDistances& mat, unsigned S)
   {
      mat[S].distance.type = Path::NO_SHORTEST_PATH;
      for (auto& i : mat[S].edges)
         if (Path::NO_SHORTEST_PATH != mat[i.first].distance.type)
            SetNegativeCycle(mat, i.first);

      return true;   // changed
   }

   static inline
   bool Update(Path& next, const Distance& d1, const Distance& d2)
   {
      assert(d1 != MAX_DISTANCE && d2 != MAX_DISTANCE);
      assert(next.type == Path::NO_PATH || next.distance > d1 + d2);

      next.type = Path::SHORTEST_PATH;
      next.distance = d1 + d2;

      return true;   // changed
   }

   static
   void BellmanFord(ContDistances& mat, unsigned N, unsigned S)
   {
      unsigned loop = 0;
      // use change :  #15
      const unsigned LOOP_LIMIT = N;   // #9

      for (bool change = true; change && (loop <= LOOP_LIMIT); ++loop)
      {
         change = false;

         for (size_t i = 0; i < N; ++i)
         {
            const auto& current = mat[i].distance;

            if (Path::SHORTEST_PATH != current.type)
            {
               // negative cycles (type 2) were already propagated
               continue;
            }
            //assert(current.distance != MAX_DISTANCE);

            for (const auto& j : mat[i].edges)
            {
               auto& next = mat[j.first].distance;    

               switch (next.type)
               {
               case Path::NO_SHORTEST_PATH:
                  continue;

               case Path::SHORTEST_PATH:
                  {
                     if (next.distance <= current.distance + j.second)
                        continue;

                     if (loop >= 1)
                        change = SetNegativeCycle(mat, j.first);
                     else 
                        change = Update(next, current.distance, j.second);
                  }
                  break;

               case Path::NO_PATH:
                     change = Update(next, current.distance, j.second);
                  break;
               }
            }  // all edges of current node

         }
      };
   }

   static
   void ShortestPaths_DetectCycles(ContDistances& mat, unsigned N, unsigned S, Distances& distS)
   {
      // prepare
      {
         // set the obvious distances, that's the source's edges
         for (const auto& j : mat[S].edges)
         {
            auto& next = mat[j.first].distance; 
            next.distance = j.second;

            next.type = (S == j.first && j.second < 0)  ? Path::NO_SHORTEST_PATH : Path::SHORTEST_PATH;
         }

         auto& next = mat[S].distance;
         if (0 == next.type)
         {
            next.distance = 0;
            next.type = 1;
         }

         // propage negative cycles
         for (size_t i = 0; i < N; ++i)
            if (mat[i].distance.type == Path::NO_SHORTEST_PATH)
               (void)SetNegativeCycle(mat, i);
      }

      BellmanFord(mat, N, S);

      for (size_t i = 0; i < N; ++i)
         distS[i] = mat[i].distance;
   }

   static
   void solve(FILE* in, unsigned N, unsigned M, unsigned S, Distances& distS)
   {
      ContDistances mat;
      //Vertex v_init;
      //v_init.distance.distance = std::numeric_limits<Distance>::max();
      //mat.resize(N, v_init);
      mat.resize(N);
      
      for (unsigned i = 0; i < M; ++i)
      {
         unsigned s,d;
         Distance w;
         auto res = fscanf(in, "%u %u %lld", &s, &d, &w);
         assert(res == 3);
         --s;
         --d;

         // Runtime error, test #7 : "negative loops"
         //assert(s != d || w > 0);
         assert(s != d || w < 0);

         NodeEdges::iterator it = mat[s].edges.begin();
         for (; it != mat[s].edges.end() && it->first != d; ++it);
         if (it != mat[s].edges.end())
         {
            // keep only the smallest value (multiple endges are possible)
            if (w < it->second)
               it->second = w;
         }
         else
            mat[s].edges.push_back(std::make_pair(d, w));
      }

      ShortestPaths_DetectCycles(mat, N, S, distS);
   }
}

// BF rewritten: "Wrong answer, test #15"
namespace solution_BF2
{
   static
   void SetNegativeCycle(const Edges& edges, unsigned S, Distances& dist)
   {
      dist[S].type = Path::NO_SHORTEST_PATH;
      for (auto& edge : edges)
         if (edge.s == S && Path::NO_SHORTEST_PATH != dist[edge.d].type)
            SetNegativeCycle(edges, edge.d, dist);
   }

   static
   void BellmanFord(const Edges& edges, unsigned N, unsigned S, Distances& dist)
   {
      assert(dist.size() == N);

      //Path v_init;
      //v_init.distance = MAX_DISTANCE;
      //dist.resize(N, v_init);
      for (auto& d : dist)
         d.distance = MAX_DISTANCE;
      dist[S].distance = 0;

      bool change = true;
      for (unsigned i = 0; i < N && change; ++i)
      {
         change = false;
         for (auto& edge : edges)
         {
            if (dist[edge.s].distance == MAX_DISTANCE || dist[edge.s].distance == MIN_DISTANCE)
               continue;
            if (dist[edge.s].type == Path::NO_SHORTEST_PATH)
               continue;

            change = true;
            if (dist[edge.d].distance > dist[edge.s].distance + edge.w)
            {
               // this can rapidly overflow
               dist[edge.d].distance = dist[edge.s].distance + edge.w;

               dist[edge.d].type = (dist[edge.d].prev != edge.s) ? Path::SHORTEST_PATH : Path::NO_SHORTEST_PATH;
               //dist[edge.d].type = Path::SHORTEST_PATH;
               dist[edge.d].prev = edge.s;
            }
         }
      }

      // detect negative cycles
      for (auto& edge : edges)
      {
         if (dist[edge.s].distance == MAX_DISTANCE || dist[edge.s].distance == MIN_DISTANCE)
            continue;
         if (dist[edge.d].distance > dist[edge.s].distance + edge.w)
            dist[edge.s].type = Path::NO_SHORTEST_PATH;
      }

      // propagate negative cycles
      for (unsigned i = 0; i < dist.size(); ++i)
      {
         if (dist[i].type == Path::NO_SHORTEST_PATH)
            SetNegativeCycle(edges, i, dist);
      }

      // special case for source
      if (Path::NO_SHORTEST_PATH != dist[S].type)
      {
         dist[S].type = Path::SHORTEST_PATH;
         dist[S].distance = 0;
      }
   }

   static
   void solve(FILE* in, unsigned N, unsigned M, unsigned S, Distances& distS)
   {
      Edges edges;
      edges.reserve(M);

      for (unsigned i = 0; i < M; ++i)
      {
         Edge edge;
         auto res = fscanf(in, "%u %u %lld", &edge.s, &edge.d, &edge.w);
         assert(res == 3);
         --edge.s;
         --edge.d;

         // not necessarily
         assert(edge.s != edge.d || edge.w < 0);
         assert(edge.s != edge.d || edge.s != S);   

         edges.push_back(edge);
      }

      BellmanFord(edges, N, S, distS);
   }
}

// Accepted.
namespace solution_BF3
{
   static
   void SetNegativeCycle(const Edges& edges, unsigned S, Distances& dist)
   {
      if (dist[S].type == Path::NO_SHORTEST_PATH)
         return;
      dist[S].type = Path::NO_SHORTEST_PATH;
      for (auto& edge : edges)
         if (edge.s == S && Path::NO_SHORTEST_PATH != dist[edge.d].type)
            SetNegativeCycle(edges, edge.d, dist);
   }

   static inline 
   Distance sum_no_inf(const Distance& d, const Distance& w)
   {
      const auto sum = d + w;
      if (d < 0)
      {
         if (sum > 0 && w < 0)
            return MIN_DISTANCE;
      }
      else 
      {
         if (sum < 0 && w > 0)
            return MAX_DISTANCE;
      }
      return sum;
   }

   static inline 
   Distance calculate(const Distance& d, const Distance& weight)
   {
      // "Wrong answer, test #9": the key of the solution
      //return d == MAX_DISTANCE ? MAX_DISTANCE : d == MIN_DISTANCE ? MIN_DISTANCE : d+weight;
      return d == MAX_DISTANCE ? MAX_DISTANCE : d == MIN_DISTANCE ? MIN_DISTANCE : sum_no_inf(d, weight);
   }

   static
   void BellmanFord(const Edges& edges, unsigned N, unsigned S, Distances& dist)
   {
      assert(dist.size() == N);
      for (auto& d : dist)
         d.distance = MAX_DISTANCE;
      dist[S].distance = 0;

      bool change = true;
      for (unsigned i = 0; i < N && change; ++i)
      {
         change = false;
         for (auto& edge : edges)
         {
            const auto d = calculate(dist[edge.s].distance, edge.w);

            if (dist[edge.d].distance > d)
            {
               change = true;

               dist[edge.d].distance = d;
               dist[edge.d].type = Path::SHORTEST_PATH; 
            }
         }
      }

      // detect negative cycles
      for (auto& edge : edges)
      {
         const auto d = calculate(dist[edge.s].distance, edge.w);
         if (dist[edge.d].distance > d || d == MIN_DISTANCE)
            SetNegativeCycle(edges, edge.s, dist);
      }

      // special case for source
      if (Path::NO_SHORTEST_PATH != dist[S].type)
      {
         dist[S].type = Path::SHORTEST_PATH;
         dist[S].distance = 0;
      }
   }

   static
   void solve(FILE* in, unsigned N, unsigned M, unsigned S, Distances& distS)
   {
      Edges edges;
      edges.reserve(M);

      for (unsigned i = 0; i < M; ++i)
      {
         Edge edge;
         auto res = fscanf(in, "%u %u %lld", &edge.s, &edge.d, &edge.w);
         assert(res == 3);
         --edge.s;
         --edge.d;

         // not necessarily
         assert(edge.s != edge.d || edge.w < 0);
         assert(edge.s != edge.d || edge.s != S);   

         edges.push_back(edge);
      }

      BellmanFord(edges, N, S, distS);
   }
}

static
void shortest_paths()
{
   FILE* in = fopen("path.in", "r");
   assert(in);
   FILE* out = fopen("path.out", "w");
   assert(out);

   unsigned N=0, M=0, S=0;
   auto res = fscanf(in, "%u %u %u", &N, &M, &S);
   assert(res == 3);
   --S;
   assert(S < N);

   Distances distS;
   distS.resize(N);

   solution_BF3::solve(in, N, M, S, distS);

   for (auto i : distS)
   {
      switch (i.type)
      {
      case 0:
         fprintf(out, "*");
         break;
      case 1:
         fprintf(out, "%lld", i.distance);
         break;
      default:
         fprintf(out, "-");
         break;
      }
      fprintf(out, "\n");
   }

   res = fclose(out);
   assert(res == 0);
}

//int main()
//{
//   shortest_paths();
//   return 0;
//}