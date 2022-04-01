#include <stdio.h>

int main(){
  int* x;
  if(x == NULL){
    printf("x is null\n");
  }
  printf("%p\n", x);
  return 0;
}
