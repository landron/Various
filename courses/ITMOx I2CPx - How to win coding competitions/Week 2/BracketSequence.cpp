#include <fstream>
#include <string>
#include <list>
#include <assert.h>

bool Validate(const char* next)
{
   std::list<char> brackets;

   for (;*next;++next)
   {
      switch (*next)
      {
      case '(':
      case '[':
         brackets.push_back(*next);
         break;
      case ']':
         if (brackets.empty() || (brackets.back() != '['))
            return false;
         brackets.pop_back();
         break;
      case ')':
         if (brackets.empty() || (brackets.back() != '('))
            return false;
         brackets.pop_back();
         break;
      }
   }

   return brackets.empty();
}

int bracket_sequence()
{
    std::ifstream in("brackets.in");
    if (!in)
        return -1;
    std::ofstream out("brackets.out");
    if (!in)
        return -2;

    std::string line;
    while (std::getline(in, line))
    {
       //out << line.c_str();
       out << (Validate(line.c_str()) ? "YES" : "NO");
       out << '\n';
    }

    return 0;
}

//int main()
//{
//   return bracket_sequence();
//}
