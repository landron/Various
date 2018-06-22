/*
   ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 2 > 2nd Week Problems > Snowmen
   https://goo.gl/0VwqGs

   Tries
      01.11.2016  /  "You have used 1 of 200 attempts" / "Wrong answer, test #6"
                     "You have used 4 of 200 attempts" / "Wrong answer, test #17"
                           all entries must retain the history
                     "You have used 4 of 200 attempts" / "Wrong answer, test #17"
                           it is not because of size vs unsigned
                           simplified: Container1 -> Container2
                     "You have used 7 of 200 attempts" / "Presentation error, test #8"
                     
                     Accepted:   "You have used 10 of 200 attempts"
                        "unsigned long long":   weight
*/
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <assert.h>

typedef std::vector<unsigned> Stack;
typedef std::vector<std::pair<Stack, size_t>> Container1;
static inline
void Add(Container1& snowmen, unsigned idx, unsigned mass)
{
   assert(idx < snowmen.size());

   if (mass)
   {
      snowmen.push_back(Container1::value_type(snowmen[idx].first, snowmen[idx].second+mass));
      snowmen.back().first.push_back(mass);
   }
   else
   {
      snowmen.push_back(Container1::value_type(snowmen[idx].first, snowmen[idx].second-snowmen[idx].first.back()));
      snowmen.back().first.pop_back();
   }
}

typedef std::vector<std::pair<unsigned, size_t>> Container2;
static inline
void Add(Container2& snowmen, unsigned idx, unsigned mass)
{
   assert(idx < snowmen.size());

   if (mass)
      snowmen.push_back(Container2::value_type(idx, snowmen[idx].second+mass));
   else
      snowmen.push_back(snowmen[snowmen[idx].first]);
}

int snowmen_weight()
{
   std::ifstream in("snowmen.in");
   if (!in)
      return -1;
   std::ofstream out("snowmen.out");
   if (!in)
      return -2;
   size_t N = 0;
   in >> N;
   if (in.bad())
      return -3;

   //Container1 snowmen;
   //snowmen.reserve(N+1);
   //snowmen.push_back(Container1::value_type(Stack(),0));
   Container2 snowmen;
   snowmen.reserve(N+1);
   snowmen.push_back(Container2::value_type(0,0));

   std::string empty;
   if (!std::getline(in, empty))
      return -5;

   for (size_t i = 0; i < N; ++i)
   {
      unsigned t = 0, m = 0;
      in >> t >> m;
      if (in.bad())
         return -6;

      Add(snowmen, t, m);
   }

   //auto weight = 0.0;
   unsigned long long weight = 0;
   for (auto i : snowmen)
      weight += i.second;
   //out << std::setprecision(0) << static_cast<unsigned long>(std::floor(weight)) << '\n';
   //out << std::setprecision(0) << std::floor(weight);
   out << weight;

   return 0;
}

//int main()
//{
//   return snowmen_weight();
//}
