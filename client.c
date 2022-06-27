#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "dbcon.h"

static void register_pt();

static char first_name[41];
static char last_name[41];
static char* patient_id;
static int pt_age;


volatile sig_atomic_t sigint_flag = 0;

void SIG_INT_HANDLER(int signum){
        sigint_flag = 1;
}

void SIG_PIPE_HANDLER(int signum){
        _exit(signum);
}

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
        
        //Unix Domain Socket connection
        struct sockaddr_un addr;
        int sun_fd = socket(PF_UNIX, SOCK_STREAM, 0);
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, "./Sp_data");
        connect(sun_fd, (struct sockaddr *) &addr, sizeof(addr));

        //Read from arduino
        char buff[512] = {0};
        int amt_read;
        
        //While the user has NOT entered SIGINT and the device connection is still up
        while(!sigint_flag && (amt_read = read(socket_fd, buff,sizeof(buff)-1)) >0){    
	  
                //Debug what is written to data process
	        #ifdef DEBUG
                write(1,buff,amt_read);
                #endif

                //Send to python for processing
                write(sun_fd, buff, amt_read);

	}
    return amt_read;
}


//Initialize the child
//For now only signal handlers
void child_init(){
        struct sigaction sa;
        sa.sa_handler = SIG_PIPE_HANDLER;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGPIPE, &sa, NULL);
}
