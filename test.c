#include <sys/stat.h>
#include <fcntl.h>

int main(){
  printf("%zd %zd %zd\n", O_CREAT, O_TRUNC, S_IRUSR);
}
