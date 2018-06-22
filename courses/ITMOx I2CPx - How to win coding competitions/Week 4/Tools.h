
#include <list>
#include <vector>

namespace adjacency_list
{
   typedef unsigned long Distance;
   struct Vertex
   {
      Distance distance;
      std::list<std::pair<unsigned, Distance>> edges;
   };
   typedef std::vector<Vertex> ContDistances;

   void Dijsktra(unsigned edges_number, ContDistances& vertexes);
};

namespace adjacency_matrix
{
   typedef long long Distance;

   bool Dijskstra(int* const* mat, unsigned N, unsigned S, unsigned F, Distance& distSF);
};
