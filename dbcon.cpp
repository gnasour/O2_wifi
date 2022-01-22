#include <sqlite3.h>
#include <stdio.h>

#define DB_NAME "stats.db"

sqlite3* db_obj;

int open_db(){
  
  int err_code = sqlite3_open(DB_NAME, &db_obj);
  if(err_code){
    printf("Error in opening DB connection: ERR_CODE = %d\n", err_code);
    sqlite3_close(db_obj);
    return 1;
  }

  return 0;

}

int exec_stmt(const char* stmt){
  sqlite3_stmt* smt;
  char** buf;
  if(db_obj){
    sqlite3_exec(db_obj, 
		    "CREATE TABLE o2_rec(o2 varchar(30) primary key);",
		    NULL,
		    NULL,
		    NULL);
    //sqlite3_free(buf);
    if(smt)
    sqlite3_step(smt);
    else
      printf("error");
  }

  
  return 0;

}



int main(){
  open_db();
  exec_stmt("");
  if(db_obj){
    sqlite3_close(db_obj);
  }
  return 0;
}
