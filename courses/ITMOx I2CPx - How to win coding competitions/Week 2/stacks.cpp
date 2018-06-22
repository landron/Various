/*
   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 2 > 2nd Week Problems > Stacks
   https://goo.gl/0VwqGs

   Tries
      01.11.2016  /  "You have used 1 of 200 attempts" / "Wrong answer, test #8"
                     an non empty cup always starts a new stack even if the existent stack has only empty cups
      Accepted
*/
#include <fstream>
#include <string>
#include <set>
#include <assert.h>

int biggest_stack()
{
   std::ifstream in("stacks.in");
   if (!in)
      return -1;
   std::ofstream out("stacks.out");
   if (!in)
      return -2;
   size_t N = 0;
   in >> N;
   if (in.bad())
      return -3;

   std::string empty;
   if (!std::getline(in, empty))
      return -5;

   unsigned t = 0;
   in >> t;
   if (in.bad())
      return -6;

   size_t i = 1;

   // be sure we have at least a stack with some coffee left
   for (; !t && i < N; ++i)
   {      
      in >> t;
      if (in.bad())
         return -8;
   }

   typedef std::multiset<unsigned> Container;
   Container stacks;

   if (i != N)
   {
      //an non empty cup always starts a new stack even if the existent stack has only empty cups
      stacks.insert(1);
      if (i > 1)
         stacks.insert(i-1);

      for (; i < N; ++i)
      {
         in >> t;
         if (in.bad())
            return -9;

         if (t)
            stacks.insert(1);
         else
         {
            stacks.insert(1 + *stacks.begin());
            stacks.erase(stacks.begin());
         }
      }
   }
   else
      stacks.insert(i);

   if (0)
   {
      size_t total = 0;
      for (auto i : stacks)
         total += i;
      assert(total == N);
   }

   Container::const_iterator last = --stacks.end();
   out<<*last;

   return 0;
}

//int main()
//{
//   return biggest_stack();
//}
