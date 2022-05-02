#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>

#include "dbcon.h"

static char first_name[40];
static char last_name[40];

int send_to_db(char* info){
        char prepared_stmt[512];
        char hr[3];
        char spo2[3];
        int i;
        
        for(i = 0; i < 3; i++){
                hr[i] = info[i+4];
                spo2[i] = info[i+14];
        }
        //printf("%d\t%d\n",atoi(hr), atoi(spo2));
        printf("%s", info);
        printf("%s\t%s\n", hr, spo2);
        sprintf(prepared_stmt, "INSERT INTO patient_data\
        VALUES(\"John\",\"Doe\",\"000000000001\",45,%d,%d,37,NULL)", atoi(hr),atoi(spo2));
        exec_stmt(prepared_stmt);
        return 0;
}
void printthis(char* bu){
        while(*bu!='\0'){
                if(*bu =='\n'){
                        putchar('*');}
                else{
                        putchar(*bu);}
                
                bu++;
        }
        putchar('\n');
}

int recv_data(int socket_fd){
    int c = 0;
        char buff[512];
        int amt_read;
        while(amt_read = read(socket_fd, buff,(sizeof buff)-1)){
	    buff[amt_read] = '\0';
            printthis(buff);
	//     if(strncmp(buff, "H", 1)){
                
        //             if(amt_read == 1){
        //                     //printf("%c\n", buff[0]);
        //             }else{
        //                     write(1, buff, amt_read);
        //             }
        //     }
                
	}
        return amt_read;
    
}

