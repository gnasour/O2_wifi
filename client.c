#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>

#include "dbcon.h"

int send_to_db(char* info){
        char prepared_stmt[512];
        sprintf(prepared_stmt, "INSERT INTO patient_data \
        VALUES(John,Doe,45,%d,%d,37");
        exec_stmt(prepared_stmt);
        return 0;
}

int recv_data(int socket_fd, int data_fd){
    
        char buff[512];
        int amt_read;
        while(amt_read = read(socket_fd, buff,(sizeof buff)-1)){
	    buff[amt_read] = '\0';
	    if(strncmp(buff, "HR", 2) || strncmp(buff, "SP", 2))
	      write(data_fd, buff, amt_read);
              printf("test");
	}
        return amt_read;
    
}

