#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>

#include "dbcon.h"

static void register_pt();

static char first_name[41];
static char last_name[41];
static char patient_id[12];
static int pt_age;

void get_pt_info(){
        printf("Please enter the first name of the patient: ");
        scanf("%40s", first_name);
        printf("Please enter the last name of the patient: ");
        scanf("%40s", last_name);
        printf("Please enter the age of the patient: ");
        scanf("%d", &pt_age);
        register_pt();
}

static void register_pt(){
        char prepared_stmt[512];
        sprintf(prepared_stmt, "SELECT * FROM patient_rcrd WHERE pt_first_name='%s' AND pt_last_name='%s'", first_name, last_name);
        exec_stmt(prepared_stmt);

}

static int send_to_db(char* info){
        char prepared_stmt[512];
        char hr[3];
        char spo2[3];
        int i;
        
        for(i = 0; i < 3; i++){
                hr[i] = info[i+4];
                spo2[i] = info[i+14];
        }
        printf("%s", info);
        printf("%s\t%s\n", hr, spo2);
        sprintf(prepared_stmt, "INSERT INTO patient_data\
        VALUES('%s',%d,%d,37,DateTime('NOW'))", patient_id,atoi(hr),atoi(spo2));
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
            //printthis(buff);
	    if(strncmp(buff, "H", 1)){
                
                    if(amt_read == 1){
                            //printf("%c\n", buff[0]);
                    }else{
                            write(1, buff, amt_read);
                    }
            }
                
	}
        return amt_read;
    
}

