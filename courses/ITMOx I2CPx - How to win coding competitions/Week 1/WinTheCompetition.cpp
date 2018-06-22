/*
   "the maximum possible number of problems they can solve in 300 minutes"
   "Input
      The ﬁrst line of the input ﬁle contains an integer number n (1 ≤ n ≤ 15) – the number of proposed problems. The second line 
      contains n integer numbers t1, t2, …, tn, where ti is the time, in seconds, which is required to solve the i-th problem. All 
      ti are positive and don’t exceed 105. Please recall that one minute consists of 60 seconds.
   Output
      Output the single number – the maximum number of problems, which Dream Team can solve."

   https://courses.edx.org/courses/course-v1:ITMOx+I2CPx+3T2016/courseware/5f7ded3dde75420f8da894830d69e7e4/05c1037ba1e2450e931dfbd1c2d1856d/

   Additional files:
      example-week1-01-win.in.txt
      example-week1-02-win.in.txt
*/
#include <fstream>

enum
{
   TIME_TOTAL = 300*60,
};

void get_max(int t[15], size_t n, size_t pos, size_t total, size_t current, size_t& maximum)
{
   if (pos == n)
      return;

   if (t[pos]+total > TIME_TOTAL)
      return get_max(t, n, pos+1, total, current, maximum);

   // with pos
   total += t[pos];
   current ++;
   if (current > maximum)
      maximum = current;
   get_max(t, n, pos+1, total, current, maximum);

   // without pos
   total -= t[pos];
   -- current;
   get_max(t, n, pos+1, total, current, maximum);
}

int WinTheCompetition()
{
   std::ifstream in("win.in");
   if (!in)
      return -1;
   unsigned n = 0, total = 0;
   in >> n;
   if (in.bad())
      return -2;
   int t[15];
   for (size_t i = 0; i < n; ++i)
      in >> t[i];
   if (in.bad())
      return -3;

   get_max(t, n, 0, 0, 0, total);

   std::ofstream out("win.out");
   out << total << std::endl;

   return 0;
}