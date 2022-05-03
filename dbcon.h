#ifndef __dbcon__
#define __dbcon__

#ifndef DEBUG
#define DEBUG 0 //Debug vital values sent to server
#endif


struct db_result{
    int count; //Count of columns in table returned from query
    const unsigned char** res; //Results of the query
    const char** col_name; //Column names of the query
};

typedef struct db_result db_result;

void init_db(void);
void init_table(void);
int exec_stmt(const char*, db_result*);
int close_db(void);
#endif