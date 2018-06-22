/*
   Lesson: first do the obvious solution, then try to improve it!

   Week 4, 2

   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 4 > 4th Week Problems > Bipartite Graph
   https://goo.gl/QZHPnA

   25.11.2016  /  "You have used 1 of 200 attempts" / "Wrong answer, test #13"
                  "You have used 5 of 200 attempts" / "Wrong answer, test #3"
                        Boundary test cases
                  Case added:
                  "
                     4 3
                     1 3
                     2 4
                     4 3
                  " YES
                  "You have used 10 of 200 attempts" / "Runtime error, test #13"
                     - not the assertions
                     - strangely, validating against color 0 does not work (test #3)
                  "You have used 18 of 200 attempts" / "Runtime error, test #13"
                  "
                     4 4
                     1 3
                     2 4
                     4 1
                     1 2
                  " NO

                  Finally, again!
                  "You have used 22 of 200 attempts" / "Wrong answer, test #13"

   26.11.2016  /  "You have used 23 of 200 attempts" / "Wrong answer, test #26"
                  "You have used 24 of 200 attempts" / "Time limit exceeded, test #33"

                  => solution_3
                  "You have used 26 of 200 attempts" / "Time limit exceeded, test #33"

                  => solution_4
                  "You have used 28 of 200 attempts" / "Wrong answer, test #19"
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <vector>
#include <list>

// it doesn't work:  too complex; "Wrong answer, test #13"
namespace solution_1
{
   struct Vertex
   {
      unsigned color;
      unsigned component;
      std::list<unsigned> edges;

      Vertex(): color(0), component(0)
      {}
   };
   typedef std::vector<Vertex> Vertices;

   static 
   void AssignComponent(Vertices& v, unsigned vertex, unsigned component)
   {
      assert(vertex < v.size());
      assert(v[vertex].color);
      assert(component);

      v[vertex].component = component;
      for (auto i : v[vertex].edges)
      {
         v[i].color = 3 - v[vertex].color;
         AssignComponent(v, i, component);
      }
   }

   static inline
   void ReassignVerticesOrdered(Vertices& v, unsigned first, unsigned second)
   { 
      if (!v[first].component)
      {
         v[first].component = first+1;
         assert(!v[first].color);
         v[first].color = 1;
      }
      const bool colour_changed = v[second].color == v[first].color;
      v[second].color = 3 - v[first].color;

      const auto second_vertex = v[second].component ? (v[second].component-1) : second;
      if (colour_changed && v[second].component)   // also revert its colour
         v[second_vertex].color = 3 - v[second_vertex].color;
      AssignComponent(v, second_vertex, v[first].component);
      v[first].edges.push_back(second_vertex);
   }

   static
   bool reassign_vertices(Vertices& v, unsigned a, unsigned b)
   { 
      assert(a !=  b);

      if (0 == v[a].component)
      {
         if (0 == v[b].component && a < b)
            ReassignVerticesOrdered(v, a, b);
         else
            ReassignVerticesOrdered(v, b, a);
      }
      else
      {
         if (v[b].color == v[a].color)
         {
            if (v[b].component == v[a].component)
               return false;
            // otherwise, the component must be updated
         }
         else if (v[b].component == v[a].component)
            return true;

         if (0 != v[b].component && v[b].component < v[a].component)
            ReassignVerticesOrdered(v, b, a);
         else
            ReassignVerticesOrdered(v, a, b);
      }

      return true;
   }

   static
   bool is_bipartite(FILE* in, unsigned N, unsigned M)
   {
      if (N == 1)
         return false;
      if (M == 0)
         return true;

      // at least an unconnected vertex
      if (N/2 > M)
         return false;
      else if (N/2 == M && N%2 == 1)
         return false;

      Vertices v;
      v.resize(N);

      for (unsigned i = 0; i < M; i++)
      {
         unsigned a=0, b=0;
         auto res = fscanf (in, "%u %u", &a, &b);
         assert(res == 2);
         --a;
         --b;

         // loop (not allowed: test 4)
         if (a == b)
            return false;

         if (!reassign_vertices(v, a, b))
            return false;
      }

      // are there any unconnected nodes ? they can be connected to any component
      {
         for (auto i : v)
         {
            assert(i.color || 0 == i.component); 
            assert(i.component || 0 == i.color);
         }
      }

      return true;
   }
}

//  determine component by cycle, on the fly:   "Time limit exceeded, test #33"
/*
   - cannot keep each entry for a couple: it will make a cycle => keep the biggest one only
*/
namespace solution_2
{
   struct Vertex
   {
      unsigned colour;
      std::list<unsigned> edges;
      Vertex(): colour(0)
      {}
   };
   typedef std::vector<Vertex> Vertices;

   static
   bool join_components(Vertices& v, unsigned src, unsigned colour_parent, unsigned b)
   {
      v[b].colour = 3 - colour_parent;
      for (auto i : v[b].edges)
      {
         // already visited
         if (v[i].colour == 3 - v[b].colour)
            continue;
         // cannot change the colour in the source
         if (i == src)
            return false;
         if (!join_components(v, src, v[b].colour, i))
            return false;
      }
      return true;
   }

   static
   bool is_bipartite(FILE* in, unsigned N, unsigned M)
   {
      if (M == 0) // we can group them as wanted
         return true;
      if (N == 1)
         return false;

      // at least an unconnected vertex:  "Wrong answer, test #26"
      if (0)
      {
         if (N/2 > M)
            return false;
         else if (N/2 == M && N%2 == 1)
            return false;
      }

      Vertices v;
      v.resize(N);

      for (unsigned i = 0; i < M; i++)
      {
         unsigned a=0, b=0;
         auto res = fscanf (in, "%u %u", &a, &b);
         assert(res == 2);
         --a;
         --b;

         // loop (not allowed: test 4)
         if (a == b)
            return false;

         // inverse them to have the order 
         if (a > b)
            std::swap(b, a);

         bool skip = false;
         for (auto i : v[a].edges)
            if (i == b)
            {
               skip = true;
               break;
            }
         if (skip)
            continue;

         if (v[a].colour)
         {
            if (v[b].colour)
            {
               // can we join them ?
               if (!join_components(v, b, v[b].colour, a))
                  return false;
            }
            else
               v[b].colour = 3 - v[a].colour;
         }
         else if (v[b].colour)
            v[a].colour = 3 - v[b].colour;
         else 
         {
            v[a].colour = 1;
            v[b].colour = 2;
         }

         v[a].edges.push_back(b);
         v[b].edges.push_back(a);
      }

      // are there any unconnected nodes ? they can be connected to any component, so just ignore them

      return true;
   }

};

// solution_2 + component
namespace solution_3
{
   struct Vertex
   {
      unsigned colour;
      unsigned component;
      std::list<unsigned> edges;
      Vertex(): colour(0), component(0)
      {}
   };
   typedef std::vector<Vertex> Vertices;

   static
   bool join_components(Vertices& v, unsigned parent, unsigned b)
   {
      v[b].colour    = 3 - v[parent].colour;
      v[b].component = v[parent].component;

      for (auto i : v[b].edges)
         if (v[i].component != v[parent].component)
            join_components(v, b, i);
         else if (v[b].colour == v[i].colour)
            return false;
      return true;
   }

   static
   bool is_bipartite(FILE* in, unsigned N, unsigned M)
   {
      if (M == 0) // we can group them as wanted
         return true;
      if (N == 1)
         return false;

      // at least an unconnected vertex:  "Wrong answer, test #26"
      if (0)
      {
         if (N/2 > M)
            return false;
         else if (N/2 == M && N%2 == 1)
            return false;
      }

      Vertices v;
      v.resize(N);

      for (unsigned i = 0; i < M; i++)
      {
         unsigned a=0, b=0;
         auto res = fscanf (in, "%u %u", &a, &b);
         assert(res == 2);
         --a;
         --b;

         // loop (not allowed: test 4)
         if (a == b)
            return false;

         // inverse them to have the order 
         if (a > b)
            std::swap(b, a);

         bool skip = false;
         for (auto i : v[a].edges)
            if (i == b)
            {
               skip = true;
               break;
            }
         if (skip)
            continue;

         if (v[a].colour)
         {
            if (v[b].colour)
            {
               if (v[b].colour == v[a].colour)
               {
                  // can we join them ?
                  if (v[a].component == v[b].component)
                     return false;
               }
               else if (v[a].component == v[b].component)
                  continue;

               if (!join_components(v, b, a))
                  return false;
            }
            else
            {
               v[b].colour = 3 - v[a].colour;
               v[b].component = v[a].component;
            }
         }
         else if (v[b].colour)
         {
            v[a].colour = 3 - v[b].colour;
            v[a].component = v[b].component;
         }
         else 
         {
            v[a].colour = 1;
            v[b].colour = 2;
            v[a].component = v[b].component = a;
         }

         v[a].edges.push_back(b);
         v[b].edges.push_back(a);
      }

      // are there any unconnected nodes ? they can be connected to any component, so just ignore them

      return true;
   }

};

// solution_3 + validate only in the end
namespace solution_4
{
   typedef solution_3::Vertices Vertices;

   static
   bool join_components(Vertices& v, unsigned src)
   {
      for (auto i : v[src].edges)
         if (v[i].component != v[src].component)
         {
            if (!solution_3::join_components(v, src, i))
               return false;
         }
         else if (v[src].colour == v[i].colour)
            return false;
      return true;
   }

   static
   bool is_bipartite(FILE* in, unsigned N, unsigned M)
   {
      if (M == 0) // we can group them as wanted
         return true;
      if (N == 1)
         return false;

      // at least an unconnected vertex:  "Wrong answer, test #26"
      if (0)
      {
         if (N/2 > M)
            return false;
         else if (N/2 == M && N%2 == 1)
            return false;
      }

      Vertices v;
      v.resize(N);

      for (unsigned i = 0; i < M; i++)
      {
         unsigned a=0, b=0;
         auto res = fscanf (in, "%u %u", &a, &b);
         assert(res == 2);
         --a;
         --b;

         // loop (not allowed: test 4)
         if (a == b)
            return false;

         // inverse them to have the order 
         if (a > b)
            std::swap(b, a);

         bool skip = false;
         for (auto i : v[a].edges)
            if (i == b)
            {
               skip = true;
               break;
            }
         if (skip)
            continue;

         v[a].edges.push_back(b);
         v[b].edges.push_back(a);   // comment => "Wrong answer, test #14"
      }

      for (unsigned i = 0; i < N; i++)
      {
         if (v[i].edges.empty())
            continue;
         else if (0 == v[i].component)
         {
            v[i].component = i+1;
            v[i].colour = 1;
         } 
         else
            assert(v[i].colour);

         if (!join_components(v, i))
            return false;
      }

      // are there any unconnected nodes ? they can be connected to any component, so just ignore them

      return true;
   }

};

static
int has_cycle()
{
   FILE* in = fopen("bipartite.in", "r");
   if (!in)
      return -1;
   FILE* out = fopen("bipartite.out", "w");
   if (!out)
      return -2;

   unsigned N=0, M=0;
   auto res = fscanf (in, "%u", &N);
   assert(res == 1);
   res = fscanf (in, "%u", &M);
   assert(res == 1);

   const auto test = solution_4::is_bipartite(in, N, M);
   fprintf(out, test ? "YES" : "NO");

   res = fclose(out);
   assert(res == 0);

   return 0;
}

//int main()
//{
//   return has_cycle();
//}