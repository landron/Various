/*
   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 2 > 2nd Week Problems > Kenobi's Lightsabers
   https://goo.gl/0VwqGs

   Tries
      02.11.2016  /  "You have used 2 of 200 attempts" / "Time limit exceeded, test #46"
                     "You have used 4 of 200 attempts" / "Time limit exceeded, test #46"
                        same thing with memcpy
                     "You have used 5 of 200 attempts" / "Time limit exceeded, test #46"
                        initial reserve does not help ... but I have some ideas: 
                           - the list is circular
                           - try to group successive "mum" operations
     03.11.2016  /  "You have used 6 of 200 attempts" / "Wrong answer, test #7"
                        successive mum operations
                    "You have used 7 of 200 attempts" / "Runtime error, test #3"
                        rounding error corrected:  "distance*(cur->size()/2)"
                        ... but some wrong assertion
                    "You have used 8 of 200 attempts" / "Wrong answer, test #46"
                    "You have used 13 of 200 attempts" / "Wrong answer, test #46"
                        minor improvements
    06.11.2016 /    "You have used 14 of 200 attempts" / "Runtime error, test #3"
                        solution 2: using list & splice - not finished
                        solution 3: using custom list
                    "You have used 15 of 200 attempts" / "Time limit exceeded, test #45"
                        solution 3: kenobi_lightsabers_direct
                    "You have used 16 of 200 attempts" / "Wrong answer, test #46"
                        solution 3: kenobi_lightsabers_groupmum
                    "You have used 17 of 200 attempts" / Accepted
                        kenobi_lightsabers_groupmum + ApplyMum_Rotations
                        So probably solution1 could be accepted too using the same trick (ApplyMum_Rotations)
*/
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <assert.h>
#include <stdlib.h>  // strtol
#include <string.h>  // memcpy

//  http://stackoverflow.com/a/6154614/1032277
enum STR2INT_ERROR { CA_STR2INT_SUCCESS, CA_STR2INT_OVERFLOW, CA_STR2INT_UNDERFLOW, CA_STR2INT_INCONVERTIBLE };
static inline
STR2INT_ERROR str2long(long &i, char const *s, int base = 0)
{
    errno = 0;
    char *end = nullptr;
    const long l = strtol(s, &end, base);
    if ((errno == ERANGE && l == LONG_MAX) /*|| l > INT_MAX*/) {
        return CA_STR2INT_OVERFLOW;
    }
    if ((errno == ERANGE && l == LONG_MIN) /*|| l < INT_MIN*/) {
        return CA_STR2INT_UNDERFLOW;
    }
    if (*s == '\0' || *end != '\0') {
        return CA_STR2INT_INCONVERTIBLE;
    }
    i = l;
    return CA_STR2INT_SUCCESS;
}

static inline
STR2INT_ERROR str2int(int &i, char const *s, int base = 0)
{
    long l = 0;
    const auto res = str2long(l, s, base);
    if (CA_STR2INT_SUCCESS != res)
        return res;
    if (l > INT_MAX)
        return CA_STR2INT_OVERFLOW;
    if (l < INT_MIN)
        return CA_STR2INT_UNDERFLOW;
    i = l;
    return CA_STR2INT_SUCCESS;
}

namespace solution1
{
   typedef std::vector<int> Sabres;

   static inline
   void apply_mum_1(Sabres*& cur, Sabres*& other)
   {
      other->reserve(cur->size());
      Sabres::iterator it = cur->begin();
      std::advance(it, cur->size()/2);
      other->assign(it, cur->end());
      other->insert(other->end(), cur->begin(), it);

      auto tmp = cur; cur = other; other = tmp;
   }

   // directly playing with memory: no gain
   static inline
   void apply_mum_2(Sabres*& cur, Sabres*& other)
   {
      other->resize(cur->size());
      memcpy(&((*other)[0]),                          &((*cur)[cur->size()/2]), (cur->size()-cur->size()/2)*sizeof(unsigned));
      memcpy(&((*other)[cur->size()-cur->size()/2]),  &((*cur)[0]),             cur->size()/2*sizeof(unsigned));

      auto tmp = cur; cur = other; other = tmp;
   }

   static inline
   void apply_mum_base(Sabres*& cur, Sabres*& other, unsigned distance)
   {
      assert(distance);
      assert(cur->size());

      distance = (distance*(cur->size()/2))%cur->size();
      if (!distance)
         return;

      other->reserve(cur->size());
      Sabres::iterator it = cur->begin();
      std::advance(it, distance);
      other->assign(it, cur->end());
      other->insert(other->end(), cur->begin(), it);

      auto tmp = cur; cur = other; other = tmp;
   }

   static inline
   void Print(const Sabres& s, std::ofstream& out)
   {
      for (auto i : s)
         out << i << ' ';
      out << '\n';
   }

   static inline
   void apply_mum(Sabres*& cur, Sabres*& other, unsigned& distance, std::ofstream& /*out*/)
   {
      if (distance)
      {
         if (cur->size() > 1)
            apply_mum_base(cur, other, distance);
         distance = 0;
         //Print(*cur, out);
      }
   }

   // "Time limit exceeded, test #46"
   static 
   int ApplyCommands_Directly(std::ifstream& in, Sabres*& cur, Sabres*& other, unsigned N)
   {
      //cur->reserve(N);
      //other->reserve(N);

      for (size_t i = 0; i < N; ++i)
      {
         std::string line;
         if (!std::getline(in, line))
            return -8;
         assert(!line.empty());

         switch (line[0])
         {
            // add 
         case 'a':
            assert(line.size() > 4);
            int nb;
            (void)str2int(nb, line.c_str()+4);
            cur->push_back(nb);
            break;

            // take
         case 't':
            cur->pop_back();
            break;

            // mum!
         case 'm':
            apply_mum_1(cur, other);
            break;
         }

         //out << line << '\n';
      }

      return 0;
   }

   // "Wrong answer, test #46"
   static 
   int ApplyCommands_CombineMum(std::ifstream& in, std::ofstream& out, Sabres*& cur, Sabres*& other, unsigned N)
   {
      unsigned mum = 0;

      for (size_t i = 0; i < N; ++i)
      {
         std::string line;
         if (!std::getline(in, line))
            return -8;
         assert(!line.empty());

         switch (line[0])
         {
            // add 
         case 'a':
            apply_mum(cur, other, mum, out);
            assert(line.size() > 4);
            int nb;
            (void)str2int(nb, line.c_str()+4);
            cur->push_back(nb);
            break;

            // take
         case 't':
            apply_mum(cur, other, mum, out);
            cur->pop_back();
            break;

            // mum!
         case 'm':
            ++mum;
            break;
         }
      }

      apply_mum(cur, other, mum, out);

      return 0;
   }

   int kenobi_lightsabers()
   {
      std::ifstream in("kenobi.in");
      if (!in)
         return -1;
      std::ofstream out("kenobi.out");
      if (!in)
         return -2;
      unsigned N = 0;
      in >> N;
      if (in.bad())
         return -3;

      std::string empty;
      if (!std::getline(in, empty))
         return -5;

      Sabres aux1, aux2;
      Sabres *cur = &aux1, *other = &aux2;

      auto err = ApplyCommands_CombineMum(in, out, cur, other, N);
      //auto err = ApplyCommands_Directly(in, cur, other, N);
      if (err)
         return err;

      out << cur->size() << '\n';
      Print(*cur, out);
      out.close();

      return 0;
   }
}

// using rotate & splice: it does not work (yet)
namespace solution2
{
   typedef std::list<unsigned> Sabres;

   static 
   int kenobi_lightsabers()
   {
      std::ifstream in("kenobi.in");
      if (!in)
         return -1;
      std::ofstream out("kenobi.out");
      if (!in)
         return -2;
      unsigned N = 0;
      in >> N;
      if (in.bad())
         return -3;

      std::string empty;
      if (!std::getline(in, empty))
         return -5;

      Sabres sabres;
      Sabres::iterator middle(sabres.begin());
      unsigned n = 0;

      for (size_t i = 0; i < N; ++i)
      {
         std::string line;
         if (!std::getline(in, line))
            return -8;
         assert(!line.empty());

         switch (line[0])
         {
            // add 
         case 'a':
            assert(line.size() > 4);
            int nb;
            (void)str2int(nb, line.c_str()+4);
            sabres.push_back(nb);
            
            if (n%2)
            {
               if (middle == sabres.end())
                  middle = sabres.begin();
               ++middle;
            }
            ++n;

            break;

            // take
         case 't':
            //if (n == 5)
            //   printf("");
            sabres.pop_back();
            --n;
            if (n%2)
               --middle;
            break;

            // mum!
         case 'm':
            if (n > 1)
            {
               Sabres::const_iterator middle_old = middle;
               middle = sabres.end();
               --middle;
               sabres.splice(sabres.end(), sabres, sabres.begin(), middle_old);
            }
            break;
         }
      }

      out << sabres.size() << '\n';
      for (auto i : sabres)
         out << i << ' ';
      out.close();

      return 0;
   }
}

// using custom list
namespace solution3
{
   struct El
   {
      unsigned value;
      El* prev;
      El* next;

      El(unsigned value_): value(value_), prev(nullptr), next(nullptr) 
      {}
   };

   class Sabres
   {
   public:
      Sabres(): first(nullptr), last(nullptr), middle(nullptr), size(0)
      {}

      void AddBack(unsigned value)
      {
         if (!last)
         {
            assert(!first && !middle && !size);
            first = new El(value);
            last = middle = first;
            size = 1;
         }
         else
         {
            assert(first);
            last->next = new El(value);
            last->next->prev = last;
            last = last->next;

            ++size;
            if (size > 1 && size%2)
            {
               middle = middle->next;
               assert(middle);
            }
         }
      }

      void PopBack()
      {
         assert(size && first && last);

         auto to_delete = last;
         if (last->prev)
            last->prev->next = nullptr;
         last = last->prev;
         delete to_delete;
         if (!last)
         {
            first = nullptr;
            middle = nullptr;
         }

         if (size > 1 && size%2)
            middle = middle->prev;
         --size;
      }

      void Rotate()
      {
         switch (size)
         {
         case 0:
         case 1:
            return;
         case 2:
            {
               middle = last;
               last = first;
               first = middle;

               first->next = last;
               last->prev = first;
            }
            break;
         default:
            {
               assert (first != last);
               last->next = first;
               first->prev = last;
               first = (size%2) ? middle : middle->next;
               auto new_last = (size%2) ? middle->prev : middle;
               middle = last;
               assert (middle->next);
               last = new_last;
               assert (last);
            }
         };

         last->next = nullptr;
         first->prev = nullptr;
      }

      //"Wrong answer, test #46"
      void ApplyMum_Directly(unsigned& rotations)
      {
         const auto size = Size();
         if (size < 2)
            return;

         const unsigned distance = (rotations*(size/2))%size;
         if (!distance)
            return;

         last->next = first;
         first->prev = last;
         for (unsigned i = 0; i < distance; ++i)
         {
            last = last->next;
            first = first->next;
            middle = middle->next;
         }
         last->next = nullptr;
         first->prev = nullptr;
      }

      // Accepted
      void ApplyMum_Rotations(unsigned& rotations)
      {
         const auto size = Size();
         if (size < 2)
            return;

         if (size%2 == 0)
            rotations = rotations%2 ? 1 : 0;
         else
            rotations = rotations%size;

         for (unsigned i = 0; i < rotations; ++i)
            Rotate();
      }

      void ApplyMum(unsigned& rotations)
      {
         if (rotations)
         {
            //ApplyMum_Directly(rotations);
            ApplyMum_Rotations(rotations);
            rotations = 0;
         }
      }

      size_t Size() const
      {
         return size; 
      }

      void Print(std::ostream& out) const
      {
         out << Size() << '\n';
         for (auto i = first; i; i = i->next)
            out << i->value << ' ';
      }

      // not used, nor tested
      bool Print(const char* filename) const
      {
         FILE* out = fopen("drying.out", "w"); 
         if (!out)
            return false;

         fprintf(out, "%u\n", Size());
         for (auto i = first; i; i = i->next)
            fprintf(out, "%u ", i->value);

         auto res = fclose(out);
         assert(0 == res);

         return true;
      }

      ~Sabres()
      {
         // not really necessary
         for (El *next = first, *prev = nullptr; next; prev = next, next = next->next)
            delete prev;
         delete last;
      }

   private:
      El* first;
      El* last;
      El* middle;
      size_t size;
   };

   static 
   int kenobi_lightsabers_direct()
   {
      std::ifstream in("kenobi.in");
      if (!in)
         return -1;
      std::ofstream out("kenobi.out");
      if (!in)
         return -2;
      unsigned N = 0;
      in >> N;
      if (in.bad())
         return -3;

      std::string empty;
      if (!std::getline(in, empty))
         return -5;

      Sabres sabres;

      for (size_t i = 0; i < N; ++i)
      {
         std::string line;
         if (!std::getline(in, line))
            return -8;
         assert(!line.empty());

         switch (line[0])
         {
            // add 
         case 'a':
            assert(line.size() > 4);
            int nb;
            (void)str2int(nb, line.c_str()+4);
            sabres.AddBack(nb);

            break;

            // take
         case 't':
            sabres.PopBack();
            break;

            // mum!
         case 'm':
            sabres.Rotate();
            break;
         }
      }

      sabres.Print(out);
      out.close();

      return 0;
   }

   static 
   int kenobi_lightsabers_groupmum()
   {
      std::ifstream in("kenobi.in");
      if (!in)
         return -1;
      std::ofstream out("kenobi.out");
      if (!in)
         return -2;
      unsigned N = 0;
      in >> N;
      if (in.bad())
         return -3;

      std::string empty;
      if (!std::getline(in, empty))
         return -5;

      Sabres sabres;
      unsigned mum = 0;

      for (size_t i = 0; i < N; ++i)
      {
         std::string line;
         if (!std::getline(in, line))
            return -8;
         assert(!line.empty());

         switch (line[0])
         {
            // add 
         case 'a':
            sabres.ApplyMum(mum);
            assert(line.size() > 4);
            int nb;
            (void)str2int(nb, line.c_str()+4);
            sabres.AddBack(nb);

            break;

            // take
         case 't':
            sabres.ApplyMum(mum);
            sabres.PopBack();
            break;

            // mum!
         case 'm':
            //sabres.Rotate();
            mum++;
            break;
         }
      }

      sabres.ApplyMum(mum);
      sabres.Print(out);
      out.close();

      return 0;
   }

   static 
   int kenobi_lightsabers()
   {
      //return kenobi_lightsabers_direct();
      return kenobi_lightsabers_groupmum();
   }
}

//int main()
//{
//   return solution3::kenobi_lightsabers();
//   //return solution1::kenobi_lightsabers();
//}
