#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include "dbcon.h"



#define DB_NAME "stats.db"

//SQLite database object to facilitate database connection
sqlite3* db_obj;

/* 
 * Initialize the database object
 * Return: 0 on success, 1 on failure initializing the object
 */
int open_db(){
  
  //Open DB connection and initialize DB object
  int status_code = sqlite3_open(DB_NAME, &db_obj);
  if(status_code != SQLITE_OK){
    
    //Print error code associated with failure to open DB connection
    //TODO: change into switch statement
    printf("Error in opening DB connection: ERROR CODE = %d\n", status_code);
    sqlite3_close(db_obj);
    return 1;

  }else{
    printf("Database opened successfully!\n");
  }

  return 0;
}


/* 
 * Executes any SQL statement sent as an argument
 * Return: 0 on success, 1 if error executing statement
 */
int exec_stmt(const char* stmt){


  if(stmt == NULL){
    printf("NULL SQL statement given to exec_stmt\n");
    return 1;
  }
  
  sqlite3_stmt* smt;
  char** buf;
  char sql_smt[10];
  int step_status;
  
  if(db_obj != NULL){

    //Prepare the SQL statement
    sqlite3_prepare(db_obj, stmt, strlen(stmt), &smt, NULL);

    //Execute the statement
    if(smt){
      step_status = sqlite3_step(smt);
      if(step_status == SQLITE_ROW){

	      printf("%d\n", sqlite3_column_count(smt));
	      for(int i = 0; i < sqlite3_column_count(smt); i++){
	        printf("%s\n", sqlite3_column_text(smt, i));
	      }
      }
    }else{
      //Error executing statement
      printf("Error in executing the following statement:\n\"%s\"\n", stmt);
      return 1;
    }
  }
  
  return 0;

}

/**
 * Initialize the tables of the database
 * - For now the only two tables required are O2 and heart rate
 * Return: 0 on successfully initializing the DB tables and 1 on error
 */
int init_table(){
  
  if(db_obj == NULL){
    printf("Database object is null: Error in init_table");
    return 1;
  }
  
  if(exec_stmt("CREATE TABLE patient_data(pt_first_name VARCHAR(40), pt_last_name VARCHAR(40), pt_ID VARCHAR(12) PRIMARY KEY,\
   age TINYINT, HR TINYINT, SPO2 TINYINT, temperature TINYINT, time_recorded datetime)"))
  {
    printf("Error initializing DB tables\n");
    printf("Reason: %s\n", sqlite3_errmsg(db_obj));
  }
  exec_stmt("SELECT * FROM vitals");
  
  return 0;
}

void begin(){
  int data_file = open("data.txt", O_RDONLY);
  
  close(data_file);
  unlink("data.txt");
}

/**
 *
 *
 */
void segv_handler(int sig){
  printf("Bad memory access\n");
  //  exit(1);
}

int main(int argc, char** argv){
 
  if(argc > 1){
    if(strcmp(argv[1], "r") == 0){
      int x = system("bash rm_db.sh");
      printf("%d\n", x);
    }
  }

  open_db();
  init_table();
  begin();
  if(db_obj){
    sqlite3_close(db_obj);
  }
 
  return 0;
}
