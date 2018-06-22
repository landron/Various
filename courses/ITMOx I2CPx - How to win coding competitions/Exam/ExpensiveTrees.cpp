
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <assert.h>

static 
void problem()
{
   FILE* in = fopen("trees.in", "r");
   assert(in);
   FILE* out = fopen("trees.out", "w");
   assert(out);

   unsigned long long N;
   auto res = fscanf(in, "%llu", &N);
   assert(res == 1);

   const unsigned long long price = N/2 * (N-N/2);
   fprintf(out, "%llu", price);

   res = fclose(out);
   assert(res == 0);
}

//int main()
//{
//   problem();
//   return 0;
//}