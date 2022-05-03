#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#include "dbcon.h"



#define DB_NAME "stats.db"

//SQLite database object to facilitate database connection
static sqlite3* db_obj;

db_result db_res;

/* 
 * Initialize the database object
 * Return: 0 on success, 1 on failure initializing the object
 */
static int open_db(){
  
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
 * Executes any SQL statement sent as an argument and print the results
 * Return: 0 on success, 1 if error executing statement
 */
int exec_stmt(const char* stmt){

  if(stmt == NULL){
    printf("NULL SQL statement given to exec_stmt\n");
    return 1;
  }else if(db_obj == NULL){
    printf("NULL DB object");
    return 2;
  }
  
  sqlite3_stmt* smt;
  int step_status;
  

  //Prepare the SQL statement
  int prepare_err_code = sqlite3_prepare(db_obj, stmt, strlen(stmt), &smt, NULL);
  //Execute the statement
  if(smt){
    while((step_status = sqlite3_step(smt)) == SQLITE_ROW){
      int col_cnt = sqlite3_column_count(smt);
      db_res.count = col_cnt;
      db_res.res = calloc(col_cnt+1, sizeof(char**));
      for(int i = 0; i < col_cnt; i++){
        char* col_res = sqlite3_column_text(smt, i);
        db_res.res[i] = col_res;
        //printf("%s\n", col_res);
      }
    }
  }else{
    //Error executing statement
    printf("Error in executing the following statement:\n\"%s\"\n", stmt);
    printf("Reason: %s\n Error Code: %s", sqlite3_errmsg(db_obj), sqlite3_errstr(sqlite3_errcode(db_obj)));
    //raise(SIGQUIT);
    return 1;
  }
  sqlite3_finalize(smt);
  
  return 0;

}


/**
 * Initialize the tables of the database
 * Tables:
 *  *Patient data - HR, SpO2, Temperature, ID, date of collection
 *  *Patient record - First name, Last name, ID
 */
 void init_table(){
  char table_stmts[512];
  char* sql_stmt;
  int stmt_fd = open("init_stmt", O_RDONLY);
  int amt_read;
  amt_read = read(stmt_fd, table_stmts, sizeof(table_stmts));  
  sql_stmt = strtok(table_stmts, "\n");
  exec_stmt(sql_stmt);
  while((sql_stmt = strtok(NULL, "\n"))){
    exec_stmt(sql_stmt);

  }
  printf("Tables initialized successfully!\n");
}

/**
 * @brief Closes the active DB connection if there is one
 * 
 * @return int: sqlite_close() return value if non-null DB object and -1 on null DB object
 * 
 */
int close_db(void){
  if(db_obj){
    return sqlite3_close(db_obj);
  }else{
    return -1;
  }
}
/**
 * @brief Initializes the DB object and tables for data collection
 * 
 */
void init_db(void){
  open_db();
  init_table();  
}

