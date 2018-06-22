
/*
   "You have used 1 of 200 attempts":  "Time limit exceeded, test #4"
   "You have used 5 of 200 attempts":  "Wrong answer, test #6"

   "Wrong answer, test #15":  but an upload was on the way
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>

// "You have used 1 of 200 attempts":  "Time limit exceeded, test #4"
static
void solution_1(FILE* in, FILE* out, unsigned n, unsigned m, unsigned k_no)
{
   const auto start = ftell(in);

   std::vector<unsigned> v;

   for (unsigned type = 0; type < k_no; ++type, fseek(in, start, SEEK_SET))
   {
      v.resize(n);

      std::map<unsigned, unsigned> comp;

      for (unsigned i = 0; i < m; ++i)
      {
         unsigned s,d,k;
         auto res = fscanf(in, "%u %u %u", &s, &d, &k);
         assert(res == 3);
         --s; --d; --k;

         if (type != k)
            continue;

         if (s > d)
            std::swap(s, d);
         if (v[s] == 0)
         {
            if (v[d] == 0)
            {
               comp[s] = s; v[s] = s; v[d] = s;
            }
            else
               v[s] = d;
         }
         else 
         {
            if (v[d] == 0)
               v[d] = s;
            else 
               comp[v[d]] = v[s];
         }
      }

      v.resize(0);

      std::set<unsigned> components;
      for (const auto& j : comp)
         components.insert(j.second);

      fprintf(out, "%u\n", components.size());
   }
}

// "You have used 2 of 200 attempts":  "Time limit exceeded, test #5"
static
void solution_2(FILE* in, FILE* out, unsigned n, unsigned m, unsigned k_no)
{
   std::vector< std::list< std::pair<unsigned, unsigned>>> edges;
   edges.resize(k_no);

   for (unsigned i = 0; i < m; ++i)
   {
      unsigned s,d,k;
      auto res = fscanf(in, "%u %u %u", &s, &d, &k);
      assert(res == 3);
      --s; --d; --k;

      if (s > d)
         std::swap(s, d);

      edges[k].push_back(std::make_pair(s, d));
   }
   auto res = fclose(in);
   assert(res == 0);

   std::vector<unsigned> v;
   v.resize(n);

   for (unsigned i = 0; i < k_no; ++i)
   {
      std::map<unsigned, unsigned> comp;

      for (const auto& edge : edges[i])
      {
         const auto s = edge.first;
         const auto d = edge.second;

         if (v[s] == 0)
         {
            if (v[d] == 0)
            {
               comp[s] = s; v[s] = s; v[d] = s;
            }
            else
               v[s] = d;
         }
         else 
         {
            if (v[d] == 0)
               v[d] = s;
            else 
               comp[v[d]] = v[s];
         }
      }

      std::set<unsigned> components;
      for (const auto& j : comp)
         components.insert(j.second);

      fprintf(out, "%u\n", components.size());

      std::fill_n(v.begin(), n, 0);
   }
}

/*
   Memory limit exceeded, test #4
   You have used 4 of 200 attempts: "Memory limit exceeded, test #4"
*/
static
void solution_3(FILE* in, FILE* out, unsigned n, unsigned m, unsigned k_no)
{
   struct Category
   {
      std::vector<unsigned> nodes;
      std::map<unsigned, unsigned> comp;
   };

   std::vector<Category> categories;
   categories.resize(k_no);

   for (unsigned i = 0; i < m; ++i)
   {
      unsigned s,d,k;
      auto res = fscanf(in, "%u %u %u", &s, &d, &k);
      assert(res == 3);
      --s; --d; --k;

      if (s > d)
         std::swap(s, d);

      auto& v = categories[k].nodes;
      if (v.empty())
         v.resize(n);
      auto& comp = categories[k].comp;

      if (v[s] == 0)
      {
         if (v[d] == 0)
         {
            comp[s] = s; v[s] = s; v[d] = s;
         }
         else
            v[s] = d;
      }
      else 
      {
         if (v[d] == 0)
            v[d] = s;
         else 
            comp[v[d]] = v[s];
      }
   }

   // try to get down memory
   auto res = fclose(in);
   assert(res == 0);

   // try to get down memory
   for (unsigned i = 0; i < k_no; ++i)
      categories[i].nodes.resize(0);

   for (unsigned i = 0; i < k_no; ++i)
   {
      const auto& comp = categories[i].comp;

      std::set<unsigned> components;
      for (const auto& j : comp)
         components.insert(j.second);

      fprintf(out, "%u\n", components.size());
   }
}

static 
unsigned SolveEntries(std::map<unsigned, unsigned>& assoc, unsigned key, unsigned* new_comp = nullptr)
{
   auto comp = assoc[key];
   if (comp != key)
   {
      comp = SolveEntries(assoc, comp);
      assoc[key] = comp;
   }
   
   if (new_comp && *new_comp != comp)
   {
      assoc[comp] = *new_comp;
      if (comp != key)
         assoc[key]  = *new_comp;
      comp = *new_comp;
   }

   return comp;
}

/*
   "Time limit exceeded, test #8"   => SolveEntries
   "You have used 7 of 200 attempts"   :  "Time limit exceeded, test #14"
   "Runtime error, test #14"
   "Wrong answer, test #15"
*/
static
void solution_4(FILE* in, FILE* out, unsigned n, unsigned m, unsigned k_no)
{
   struct Category
   {
      //std::vector<unsigned> nodes;
      std::map<unsigned, unsigned> comp;
   };

   std::vector<Category> categories;
   categories.resize(k_no);

   for (unsigned i = 0; i < m; ++i)
   {
      unsigned s,d,k;
      auto res = fscanf(in, "%u %u %u", &s, &d, &k);
      assert(res == 3);
      --s; --d; --k;

      if (s > d)
         std::swap(s, d);

      auto& comp = categories[k].comp;

      if (comp.find(s) == comp.end())
      {
         if (comp.find(d) == comp.end())
         {
            comp[s] = s; comp[d] = s;
         }
         else
            comp[s] = d;
      }
      else 
      {
         if (comp.find(d) == comp.end())
            comp[d] = s;
         else 
         {
            //auto prev = d;
            //for (auto it = comp.find(prev); it->second != prev; prev = it->second, it = comp.find(prev))
            //{
            //   assert(it != comp.end());  // too late if wrong
            //}
            //comp[prev] = s;

            //if (0)
            {
            auto component = comp[d];
            if (component != d)
               component = SolveEntries(comp, component, &s);
            }
            comp[d] = s;
         }
      }
   }

   // try to get down memory
   if (0)
   {
      auto res = fclose(in);
      assert(res == 0);
   }

   for (unsigned i = 0; i < k_no; ++i)
   {
      auto& assoc = categories[i].comp;

      std::set<unsigned> components;
      for (const auto& j : assoc)
      {
         // solve the entry
         auto comp = assoc[j.second];
         if (comp != j.second)
         {
            comp = SolveEntries(assoc, comp);
            assoc[j.second] = comp;
         }

         components.insert(comp);
      }

      fprintf(out, "%u\n", components.size());
   }
}

static 
void problem()
{
   FILE* in = fopen("underground.in", "r");
   assert(in);
   FILE* out = fopen("underground.out", "w");
   assert(out);

   unsigned n, m, k_no;
   auto res = fscanf(in, "%u %u %u", &n, &m, &k_no);
   assert(res == 3);   
   assert(1 <= n);
   
   solution_4(in, out, n, m, k_no);

   res = fclose(out);
   assert(res == 0);
}

//int main()
//{
//   problem();
//   return 0;
//}