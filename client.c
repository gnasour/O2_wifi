#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>


int read_data(int sock_fd){
    
        char buff[512];
        int amt_read;
        while(amt_read = read(sock_fd, buff,(sizeof buff)-1)){
	    buff[amt_read] = '\0';
	    printf("%s", buff);
            
        }
        return amt_read;
    
}

