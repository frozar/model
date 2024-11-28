#include<stdio.h>
#include<memory.h>
#include<string.h>
int N, C, I, T;
int L[2048];
int i, j;

int Sean_add(int i1, int i2) {
  int res = 0;
  int i;
  
  int i1_bin, i2_bin;
  for (i = 0; i < 32; i++) {
    i1_bin = i1 & (1<<i);
    i2_bin = i2 & (1<<i);

    res += (i1_bin + i2_bin) & (1<<i);
  }
  return res;
}

int main()
{
  int i1, i2;
  i1 = 5;
  i2 = 4;

  printf("%d\n",  Sean_add(i1, i2));
  return 1;
}
