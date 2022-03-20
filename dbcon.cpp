#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define DB_NAME "stats.db"

//SQLite database object to facilitate database connection
sqlite3* db_obj;

/* 
* Purpose: Initialize the database object
* Return: 0 on success, 1 on failure initializing the object
*/

int open_db(){
  
  int err_code = sqlite3_open(DB_NAME, &db_obj);
  if(err_code != SQLITE_OK){
    printf("Error in opening DB connection: ERR_CODE = %d\n", err_code);
    sqlite3_close(db_obj);
    //Error in initializing database object
    return 1;
  }else{
    printf("Database opened successfully!\n");
  }

  //Success
  return 0;

}


/* 
* Purpose: Executes any SQL statement sent as an argument
* Return: 0 on success, 1 if error executing statement
*/
int exec_stmt(const char* stmt){

  sqlite3_stmt* smt;
  char** buf;

  //Check if the database object has been initialized
  if(db_obj){

    //Prepare the SQL statement
    sqlite3_prepare(db_obj, stmt, strlen(stmt), &smt, NULL);

    //Execute the statement
    //Otherwise, report the error
    if(smt){
      sqlite3_step(smt);
      printf("Successfully executed statement\n");
    }else{
      printf("error\n");
      //Error executing statement
      return 1;
    }
  }
  
  //Success
  return 0;

}


int init_table(){
  

  exec_stmt("CREATE TABLE O2_val(O2 INT)");

  return 0;
}

void segv_handler(int sig){
  printf("Bad memory access\n");
  //  exit(1);
}

int main(int argc, char** argv){

  if(argc > 1){
    if(strcmp(argv[1], "r") == 0){
      system("bash rm_db.sh");
    }
  }

  struct sigaction sa;
  sa.sa_handler = segv_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &sa, NULL);
  char* c = 0x0;
  puts(c);
  open_db();
  init_table();
  if(db_obj){
    sqlite3_close(db_obj);
  }
  return 0;
}
