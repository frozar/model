#include <stdio.h>

int H, W;

typedef struct pt_ {
  int alt;
  char c;
} pt;

void modif(pt matrix[H][W]){
  matrix[0][0].alt=3;
  matrix[0][0].c='a';
  matrix[0][1].alt=4;
  matrix[0][1].c='a';
  matrix[1][0].alt=5;
  matrix[1][0].c='b';
}   

int main(){
  
  int i, j;
  H=3, W=3;
  
  pt matrix[H][W];
  
  modif(matrix);
  
  for (i=0; i<2; i++){
    for(j=0; j<W; j++){
      printf("%d %c ", matrix[i][j].alt, matrix[i][j].c);
    }
    printf("\n");
  }
  
  printf("%d %c \n", matrix[0][3].alt, matrix[0][3].c);

  return 1;
}
