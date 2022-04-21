#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>

#include "dbcon.h"

int send_to_db(char* info){
        char prepared_stmt[70];
        char hr[3];
        char spo2[3];
        int i;
        for(i = 0; i < 3; i++){
                hr[i] = info[i+4];
                spo2[i] = info[i+14];
        }
        sprintf(prepared_stmt, "INSERT INTO patient_data\
        VALUES(\"John\",\"Doe\",\"000000000001\",45,%d,%d,37)", atoi(hr),atoi(spo2));
        exec_stmt(prepared_stmt);
        return 0;
}

int recv_data(int socket_fd, int data_fd){
    int c = 0;
        char buff[512];
        int amt_read;
        while(amt_read = read(socket_fd, buff,(sizeof buff)-1)){
	    buff[amt_read] = '\0';
	    if(strncmp(buff, "HR", 2) || strncmp(buff, "SP", 2))
	      //write(data_fd, buff, amt_read);
                send_to_db(buff);
	}
        return amt_read;
    
}

