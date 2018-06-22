/*
   Calculate max squares sum with no intersection => combinations generation (a generalization)

   https://courses.edx.org/courses/course-v1:ITMOx+I2CPx+3T2016/courseware/5f7ded3dde75420f8da894830d69e7e4/05c1037ba1e2450e931dfbd1c2d1856d/

   Additional files:
      example-week1-01-team.in.txt
      example-week1-02-team.in.txt
*/
#include <fstream>

#include <cmath>

static
int get_eff_square(const int coef[3][3], const int a[3])
{
   int eff = 0;
   for (int i = 0; i < 3; ++i)
      eff += coef[i][a[i]-1]*coef[i][a[i]-1];
   return eff;
}

int CreateATeam()
{
   std::ifstream in("team.in");
   if (!in)
      return -1;
   int coef[3][3];
   for (size_t i = 0; i < 3; ++i)
      for (size_t j = 0; j < 3; ++j)
      {
         in >> coef[i][j];
         if (in.bad())
            return -3;
      }

   static const int combinations[][3] =
   {
      {1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1},
   };

   //int solution = 0;
   int max_efficiency = get_eff_square(coef, combinations[0]);
   for (int i = 1; i < sizeof(combinations)/sizeof(combinations[0]); ++i)
   {
      int efficiency = get_eff_square(coef, combinations[i]);
      if (max_efficiency < efficiency)
      {
         max_efficiency = efficiency;
         //solution = i;
      }
   }

   const double max_efficiency_d = sqrt(max_efficiency);

   std::ofstream out("team.out");
   // Any answer, which is diﬀerent from the correct one by no more than 10−6, is accepted.
   /*
   For C++, the typical problem is the output precision when using streams. If you use streams, consider setting precision 
   for at least 10 digits (this is 4 digits before the decimal point and 6 digits after it).
   */
   out.precision(10);
   out << max_efficiency_d << std::endl;

   return 0;
}