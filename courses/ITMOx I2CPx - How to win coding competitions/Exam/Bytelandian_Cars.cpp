

/*
      "You have used 1 of 200 attempts"   :  "Wrong answer, test #3"
      "You have used 3 of 200 attempts"   :  "Wrong answer, test #15"
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <set>

struct Car
{
   // immutable anyway
   const unsigned long long price;
   unsigned count;

   Car(unsigned long long price_, unsigned count_ = 1): price(price_), count(count_)
   {
      assert(count);
   }

   bool operator< (const Car& other) const
   {
      return price < other.price;
   }
};
typedef std::set<Car> CarsOrganized;

static 
void problem()
{
   std::ifstream in("cars.in");
   assert(in);
   std::ofstream out("cars.out");
   assert(out);

   unsigned N;
   in >> N;
   assert(!in.bad());
   assert(1 <= N);

   std::string empty;
   auto res = !!std::getline(in, empty);
   assert(res);

   CarsOrganized cars_order;
   std::list<unsigned long long> cars;
   unsigned long long sum = 0;
   unsigned cars_no = 0;

   for (unsigned i = 0; i < N; ++i)
   {
      std::string line;
      auto res = !!std::getline(in, line);
      assert(res);
      assert(!line.empty());

      switch (line[0])
      {
      case '+':
         {
            assert(line.size() > 2);
            std::string temp(line.c_str()+2);
            std::istringstream conv(temp);
            unsigned long long k;
            conv >> k;
            assert(!!conv);

            sum += k;
            cars.push_back(k);
            ++cars_no;

            Car add(k);
            auto it = cars_order.find(k);
            if (cars_order.end() != it)
            {
               add.count += it->count;
               cars_order.erase(it);
            }
            cars_order.insert(add);
         }
         break;

      case '-':
         {
            --cars_no;
            const auto k = cars.front();
            sum -= k;
            cars.pop_front();

            auto it = cars_order.find(k);
            assert (cars_order.end() != it);
            const auto count = it->count - 1;
            cars_order.erase(it);
            if (count)
            {
               Car add(k, count);
               cars_order.insert(add);
            }
         }
         break;

      case '?':
         {
            const unsigned long long m = sum / cars_no;
            if (m * cars_no != sum)
            {
               out << 0 << '\n';
               continue;
            }
            const auto it = cars_order.find(m);
            if (cars_order.end() == it)
            {
               out << 0 << '\n';
               continue;
            }
            out << it->count << '\n';
         }
         break;
      };
   }

   out.close();
}

//int main()
//{
//   problem();
//   return 0;
//}