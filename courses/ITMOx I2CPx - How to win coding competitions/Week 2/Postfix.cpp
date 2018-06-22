/*
   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 2 > 2nd Week Problems > Postfix Notation
   https://goo.gl/0VwqGs

   Tries
      01.11.2016  /  "You have used 1 of 200 attempts" / "Time limit exceeded, test #32"
                  /  "You have used 3 of 200 attempts" / "Time limit exceeded, test #31"
                        resized list bug
      "You have used 4 of 200 attempts":  (reserved) vector instead of list
*/
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <cmath>
#include <assert.h>

//typedef std::list<int> Terms;
typedef std::vector<int> Terms;

static inline
void Evaluate(Terms& terms, char next)
{
   switch (next)
   {
   case '+':
   case '-':
   case '*':
      {
         assert(2 <= terms.size());
         const auto second = terms.back();
         terms.pop_back();
         const auto first = terms.back();
         terms.pop_back();

         const auto result = (next == '*') ? first*second : ((next == '+') ? first+second : first-second);
         terms.push_back(result);
      }
      break;
   default:
      assert('0' <= next);
      assert('9' >= next);
      terms.push_back(next - '0');
   }
}

int Evaluate(const char* next, unsigned size)
{
   Terms terms;
   terms.reserve(static_cast<unsigned>(size/(2*2)));

   for (;*next;++next)
   {
      if (' ' != *next)
         Evaluate(terms, *next);
   }

   assert(!terms.empty());
   return terms.back();
}

int postfix_sequence()
{
    std::ifstream in("postfix.in");
    if (!in)
        return -1;
    std::ofstream out("postfix.out");
    if (!in)
        return -2;

    std::string line;
    while (std::getline(in, line))
    {
       //out << line.c_str();
       out << Evaluate(line.c_str(), line.size());
       out << '\n';
    }

    return 0;
}

//int main()
//{
//   return postfix_sequence();
//}
