#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>


int main(){
    while(1){
        char buff[512];
        int amt_read;
        while(amt_read = read(newfd, buff,(sizeof buff)-1)){
	    buff[amt_read] = '\0';
	    printf("%s\n", buff);
    }
}

