
#include <set>
#include <assert.h>

#include "Tools.h"

namespace adjacency_list
{
   // good for sparse graphs: V*E*logV < V*V
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

// N^2 complexity
namespace adjacency_matrix
{
   /*
      \todo    C++:  send a proper const matrix
   */
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
