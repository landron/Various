/*
   This code exists mostly for the fstream  stuff: how to read a text file (a pain, really)

   https://courses.edx.org/courses/course-v1:ITMOx+I2CPx+3T2016/courseware/5f7ded3dde75420f8da894830d69e7e4/05c1037ba1e2450e931dfbd1c2d1856d/

   Additional files:
      example-week1-02-template.in.txt
      example-week1-01-template.in.txt
*/
#include <assert.h>
#include <string>
#include <fstream>
//#include <ostream>

static inline
unsigned modulo(int a)
{
   return a > 0 ? a : -a;
}

static inline
unsigned distance(unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
   return std::max(modulo(x1-x2), modulo(y1-y2));
}

int WriteCodeTemplate()
{
   std::ifstream in("template.in");
   if (!in)
      return -1;
   int H, W;
   in >> W >> H;
   if (in.bad())
      return -2;
   std::string empty;
   if (!std::getline(in, empty))
      return -3;
   struct 
   {
      int x;
      int y;
   } 
   chars[128] = {};
   for (int i = 0; i < H; ++i)
   {
      std::string line;
      if (!std::getline(in, line))
         return -4;
      assert(W == line.length());

      for (int j = 0; j < W; ++j)
      {
         //char ch;
         //in >> ch;
         const char ch = line[j];
         chars[ch].x = i;
         chars[ch].y = j;
      }
   }

   if (!std::getline(in, empty))
         return -7;

   std::string programs[3];
   unsigned scores[3] = {};
   for (int i = 0; i < 3; ++i)
   {
      if (!std::getline(in, programs[i]))
         return -8;

      std::string code;
      std::string line;
      do 
      {
         if (!std::getline(in, line))
            return -5;
         code += line;
      }
      while (!line.empty());

      unsigned score = 0;
      for (size_t j = 0; j < code.length()-1; ++j)
         score += distance(chars[code[j]].x, chars[code[j]].y, chars[code[j+1]].x, chars[code[j+1]].y);
      scores[i] = score;
      
   }

   unsigned best_score = scores[0];
   unsigned best_language = 0;
   for (int i = 1; i < 3; ++i)
   {
      if (scores[i] < best_score)
      {
         best_score = scores[i];
         best_language = i;
      }
   }

   std::ofstream out("template.out");
   out << programs[best_language] << std::endl << best_score << std::endl;

   return 0;
}