#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>


int send_to_db(char* info){
        
}

int read_data(int sock_fd){
    
        char buff[512];
        int amt_read;
        while(amt_read = read(sock_fd, buff,(sizeof buff)-1)){
	    buff[amt_read] = '\0';
            #ifdef DEBUG
            printf("%s", buff);
            printf("test");
            #endif
        }
        return amt_read;
    
}

