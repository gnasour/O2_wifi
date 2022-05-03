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
static char* patient_id;
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
        db_result* db_res = malloc(sizeof(db_result));
        sprintf(prepared_stmt, "SELECT * FROM patient_rcrd WHERE pt_first_name='%s' AND pt_last_name='%s';", first_name, last_name);
        exec_stmt(prepared_stmt, db_res);
        for(int i = 0;i<db_res->count; i++){
                if(strcmp(db_res->col_name[i], "pt_ID")==0){
                        patient_id = malloc(sizeof(char*)*(strlen((const char*)db_res->res[i])+1));
                        patient_id = strcpy(patient_id,(const char*)db_res->res[i]);
                        printf("%s\n", patient_id);
                }
        }
        free(db_res->res);
        free(db_res->col_name);
        free(db_res);
        
}

static int send_to_db(const char* info){
        char prepared_stmt[512];
        char hr[4] = {0};
        char spo2[4]= {0};
        int i;
        
        for(i = 0; i < 3; i++){
                hr[i] = info[i+4];
                spo2[i] = info[i+14];
        }
        printf("%s\n", info);
        sprintf(prepared_stmt, "INSERT INTO patient_data\
        VALUES('%s',%d,%d,37,DateTime('NOW'))", patient_id,atoi(hr),atoi(spo2));
        exec_stmt(prepared_stmt,NULL);
        return 0;
}

int recv_data(int socket_fd){
        char buff[512];
        int amt_read;
        while((amt_read = read(socket_fd, buff,(sizeof buff)-1))){
                buff[amt_read]='\0';
                if(buff[0] == 'H'){
                        send_to_db(buff);
                }
	}
    return amt_read;
}

