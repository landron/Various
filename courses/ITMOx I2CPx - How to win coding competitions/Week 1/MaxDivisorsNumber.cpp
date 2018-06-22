/*
   Toughest challenge week 1

   https://courses.edx.org/courses/course-v1:ITMOx+I2CPx+3T2016/courseware/5f7ded3dde75420f8da894830d69e7e4/05c1037ba1e2450e931dfbd1c2d1856d/

   Additional files:
      example-week1-02-testgen.in.txt
      example-week1-01-testgen.in.txt

   Results:
      23.10.2016  /  "You have used 2 of 200 attempts" /  "Time limit exceeded, test #15
      23.10.2016  /  "You have used 2 of 200 attempts" /  "Time limit exceeded, test #21"
      01.11.2016  /  "You have used 4 of 200 attempts" /  "Runtime error, test #31"
         int overflow for limit = 10^7

   !!!   accepted
      further improvements:
      -  any combination must contain 2
      -  calculate the largest string of prime factors that fits
            the best factors strings must be a combination of these ones
*/
#include <fstream>
#include <ostream>
#include <sstream> 
#include <iostream> 

#include <list>
#include <vector>
#include <cmath>
#include <assert.h>

static 
void SieveOfEratosthenes(unsigned limit, std::list<unsigned>& prime_factors)
{
   if (limit == 1)
        return;

   const double limit_d = std::sqrt(limit);
   const auto limit_real = static_cast<unsigned>(std::ceil(limit_d));
   //const auto limit_real = limit/2 + 1;

   std::vector<bool> sieve;
   sieve.resize(limit_real+1);

   for (unsigned i = 3; i <= limit_real; i += 2)
   {
      if (sieve[i])
         continue;
      for (unsigned j = 3*i; j <= limit_real; j += 2*i)
         sieve[j] = true;
   }

   prime_factors.push_back(2);
   for (size_t i = 3; i < sieve.size(); i += 2)
      if (!sieve[i])
         prime_factors.push_back(i);
}

typedef std::list<std::pair<unsigned, unsigned>> FactorsList;
struct Calculate
{
   const std::list<unsigned>& prime_factors; 
   const unsigned limit;

   FactorsList factors;
   unsigned current;

   std::pair<unsigned, unsigned> max;

   Calculate(const std::list<unsigned>& prime_factors_, unsigned limit_): 
      prime_factors(prime_factors_), limit(limit_), current(1)
   {
      max.first = 1;
      max.second = 1;
   }
};

// http://mathschallenge.net/library/number/number_of_divisors
static 
unsigned divisors_number(const FactorsList& factors)
{
   unsigned number = 1;
   for (auto i : factors)
   {
      number *= (1 + i.second);
   }
   return number;
}

static 
unsigned CalculateNumber(const FactorsList& factors)
{
   unsigned number = 1;
   for (auto i : factors)
   {
      for (unsigned j = 0; j < i.second; ++j)
         number *= i.first;
   }
   return number;
}

static
void Print(const FactorsList& factors)
{
   std::ostringstream print;

   for (auto i : factors)
   {
      print << i.first;
      print << '^';
      print << i.second;
      print << ' ';
   }
   print << '\n';

   std::cout<<print.str();
}

static 
void min_with_most_divisors(Calculate& calc)
{
   // further debugging
   if (0)
   {
      if (!calc.factors.empty())
         Print(calc.factors);
      assert(CalculateNumber(calc.factors) == calc.current);
   }

   auto i = calc.prime_factors.begin();
   if (!calc.factors.empty())
      for (; *i != calc.factors.back().first; ++i);
   for (; i != calc.prime_factors.end(); ++i)
   {
      // avoid int overflow for limit = 10^7
      const auto result = 1.0 * calc.current * *i;
      if (result > calc.limit)
         return;

      calc.current *= *i;
      assert(calc.current);

      if (calc.factors.empty() || calc.factors.back().first != *i)
         calc.factors.push_back(FactorsList::value_type(*i, 1));
      else
         ++calc.factors.back().second;

      min_with_most_divisors(calc);

      auto divisors = divisors_number(calc.factors);
      if ((divisors > calc.max.second) ||
         ((divisors == calc.max.second) && (calc.current < calc.max.first)))
      {
         calc.max.second = divisors;
         calc.max.first = calc.current;
      }

      calc.current /= *i;

      assert(calc.factors.back().first == *i);
      if (calc.factors.back().second > 1)
         --calc.factors.back().second;
      else
         calc.factors.pop_back();
   }
}

// 23.10.2016  /  "You have used 2 of 200 attempts" /  "Time limit exceeded, test #15"
//    this one does the first cycle for nothing
static 
unsigned min_with_most_divisors_1(const std::list<unsigned>& prime_factors, unsigned limit)
{
   Calculate calc(prime_factors, limit);
   for (auto i : prime_factors)
   {
      assert(calc.current == 1);
      assert(calc.factors.empty());

      min_with_most_divisors(calc);
   }

   return calc.max.first;
}

// supposition:   this number should be divisible by 2
// 23.10.2016  /  "You have used 3 of 200 attempts" /  "Time limit exceeded, test #21"
static 
unsigned min_with_most_divisors_2(const std::list<unsigned>& prime_factors, unsigned limit)
{
   // supposition:   this number should be divisible by 2
   Calculate calc(prime_factors, limit);
   min_with_most_divisors(calc);
   return calc.max.first;
}

static 
unsigned min_with_most_divisors(const std::list<unsigned>& prime_factors, unsigned limit)
{
   const auto min_with_most_divisors =  min_with_most_divisors_2(prime_factors, limit);
   return min_with_most_divisors;
}

static inline
unsigned min_with_most_divisors(unsigned limit)
{
   std::list<unsigned> primes;
   SieveOfEratosthenes(limit, primes);
   return min_with_most_divisors(primes, limit);
}

static
unsigned tests_for_min_with_most_divisors(unsigned limit)
{
   const auto min_number_with_most_divisors = min_with_most_divisors(limit);

   unsigned total = 0;
   assert(min_number_with_most_divisors <= limit);
   for (auto i = min_number_with_most_divisors; i <= limit; ++i, ++total);

   return total;
}

void DebugValidations()
{
   assert(6 == tests_for_min_with_most_divisors(11));
   assert(1 == tests_for_min_with_most_divisors(12));

   assert(48 == min_with_most_divisors(50));  // 16*3
   assert(3 == tests_for_min_with_most_divisors(50));

   assert(60 == min_with_most_divisors(100));   // 2*3*5

   assert(180 == min_with_most_divisors(200));   // ?? 180
}

int MaxDivisorsNumber(bool validations)
{
   if (validations)
      DebugValidations();

   std::ifstream in("testgen.in");
   if (!in)
      return -1;
   unsigned k = 0;
   in >> k;
   if (in.bad())
      return -2;
   
   const auto total = tests_for_min_with_most_divisors(k);

   std::ofstream out("testgen.out");
   out << total << std::endl;

   return 0;
}

//int main()
//{
//   return MaxDivisorsNumber(false);
//}
