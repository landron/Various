
/*
   "You have used 2 of 200 attempts"   "Wrong answer, test #1"
      "the friends choose two beetles, which differ in mass by at most one"
   "You have used 3 of 200 attempts"   "Memory limit exceeded, test #5"
   "You have used 4 of 200 attempts"   "Time limit exceeded, test #5"
   "You have used 7 of 200 attempts"   "Time limit exceeded, test #5"
      solution 3
   new MoveBack: starting from the beginning
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <vector>
#include <list>

#include <cstdlib>   //_itoa
#include <string.h>

typedef std::vector<unsigned> Cont;
typedef std::list<std::pair<unsigned, unsigned>> ContOperations;
struct Process
{
   unsigned count;
   ContOperations operations;

   Process(): count(0) 
   {}

   void Add(unsigned i, unsigned j)
   {
      operations.push_back(std::make_pair(i, j));
      ++count;
   }
};

static 
void quick_sort_step(Process& out, Cont& src, const unsigned s, const unsigned e, unsigned& sn, unsigned& en)
{
   assert(s < e+1);
   assert(0 == sn && 0 == en);
   
   const auto m = s+(e-s)/2;
   const auto pivot = src[m];

   unsigned i = s, j = e;
   for (;;++i, --j)
   {
      for (; src[i] < pivot; ++i);
      assert(i < e+1);
      
      for (; src[j] > pivot; --j);
      assert(j >= s);

      if (i < j)
      {
         std::swap(src[i], src[j]);
         out.Add(i,j);
      }
      else
      {
         if (i == j)
         {i++;--j;}
         break;
      }
   }

   sn = j;
   en = i;
}

static 
void quick_sort(Process& out, Cont& src, const unsigned s, const unsigned e)
{
   assert(s <= e);

   switch (e-s)
   {
   case 0:
      break;

   case 1:
      {
         if (src[s] > src[e])
         {
            std::swap(src[s], src[e]);
            out.Add(s, e);
         }
      }
      break;

   default:
      {
         unsigned sn = 0, en = 0;
         quick_sort_step(out, src, s, e, sn, en);
         assert(s <= sn && sn < en && en <= e);

         if (s < sn) // "s != sn"   should work
            quick_sort(out, src, s, sn);
         if (en < e) // "en != e"   should work
            quick_sort(out, src, en, e);
      }
      break;
   }
}

static inline
bool Adjacent(unsigned a, unsigned b)
{
   return (a > b) ? (1 == a - b) : (1 == b - a);
}

template<class T>
static inline
void MoveBack_base(T& p, Cont& v, unsigned pos)
{
   assert(pos);

   int i = pos-1;
   for (; i >= 0 && !(v[i] > v[pos] && 1 == v[i] - v[pos]); --i);
   if (i < 0)
      for (i = pos-1; i >= 0 && !Adjacent(v[i], v[pos]); --i);
   assert(i >= 0);

   std::swap(v[i], v[pos]);
   p.Add(i, pos);

   if (i && v[i] < v[i-1])
      MoveBack(p, v, i);
}

template<class T>
static inline
void MoveBack_inverted(T& p, Cont& v, unsigned pos)
{
   assert(pos);

   int i = 0;
   for (; i < pos && !(v[i] > v[pos] && 1 == v[i] - v[pos]); ++i);
   if (i == pos)
      for (i = 0; i < pos && !Adjacent(v[i], v[pos]); ++i);
   assert(i < pos);

   std::swap(v[i], v[pos]);
   p.Add(i, pos);

   if (i && v[i] < v[i-1])
      MoveBack(p, v, i);
}

template<class T>
static inline
void MoveBack(T& p, Cont& v, unsigned pos)
{
   assert(pos);

   int i = pos-1;
   for (; i >= 0 && !Adjacent(v[i], v[pos]); --i);
   assert(i >= 0);

   std::swap(v[i], v[pos]);
   p.Add(i, pos);

   if (i && v[i] < v[i-1])
      MoveBack(p, v, i);
}

// "You have used 3 of 200 attempts"   "Memory limit exceeded, test #5"
static 
void solution_1(FILE* out, unsigned N, Cont& v)
{
   Process p;
   for (unsigned i = 1; i != N;)
   {
      if (v[i-1] <= v[i])
      {
         ++i;
         continue;
      }
      MoveBack(p, v, i);
   }

   fprintf(out, "%u\n", p.count);
   for (const auto& pair : p.operations)
      fprintf(out, "%u %u\n", 1+pair.first, 1+pair.second);
}

// "You have used 4 of 200 attempts"   "Time limit exceeded, test #5"
static 
void solution_2(FILE* out, unsigned N, Cont& v_in)
{
   // count
   {
      struct Counter
      {
         unsigned count;
         void Add(unsigned i, unsigned j)
         {
            ++count;
         }
         Counter(): count(0) {}
      };

      Counter p;
      Cont v(v_in);
      for (unsigned i = 1; i != N;)
      {
         if (v[i-1] <= v[i])
         {
            ++i;
            continue;
         }
         MoveBack(p, v, i);
      }
      fprintf(out, "%u\n", p.count);
      v.resize(0);
   }

   struct Printer
   {
      FILE* out;
      void Add(unsigned i, unsigned j)
      {
         fprintf(out, "%u %u\n", 1+i, 1+j);
      }
      Printer(FILE* out_): out(out_) {}
   };
   Printer print(out);
   Cont& v = v_in;
   for (unsigned i = 1; i != N;)
   {
      if (v[i-1] <= v[i])
      {
         ++i;
         continue;
      }
      MoveBack(print, v, i);
   }
}

// "You have used 7 of 200 attempts"   "Time limit exceeded, test #5"
void solution_3(FILE* out, unsigned N, Cont& v)
{
   // make room for the total
   fprintf(out, "50000\n");

   struct Printer
   {
      FILE* out;
      unsigned count;

      void Add(unsigned i, unsigned j)
      {
         fprintf(out, "%u %u\n", 1+i, 1+j);
         ++count;
      }
      Printer(FILE* out_): out(out_), count(0) {}
   };
   Printer p(out);
   for (unsigned i = 1; i != N;)
   {
      if (v[i-1] <= v[i])
      {
         ++i;
         continue;
      }
      MoveBack(p, v, i);
   }

   auto res = fseek(out, 0, SEEK_SET);
   assert(0 == res);

   char buff[7];
   _itoa(p.count, buff, 10);
   for (unsigned i = strlen(buff); i < 7; ++i) buff[i] = ' ';
   buff[5] = '\n';
   buff[6] = '\0';
   fprintf(out, "%s", buff);
}

static 
void problem()
{
   FILE* in = fopen("sort.in", "r");
   assert(in);
   FILE* out = fopen("sort.out", "w");
   assert(out);

   unsigned N;
   auto res = fscanf(in, "%u", &N);
   assert(res == 1);

   Cont v;
   v.reserve(N);
   
   for (unsigned i = 0; i < N; ++i)
   {
      unsigned next;
      auto res = fscanf(in, "%u", &next);
      assert(res == 1);
      v.push_back(next);
   }

   solution_3(out, N, v);

   res = fclose(out);
   assert(res == 0);
}

//int main()
//{
//   problem();
//   return 0;
//}