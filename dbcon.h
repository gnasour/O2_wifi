#ifndef __dbcon__
#define __dbcon__

#ifndef DEBUG
#define DEBUG 0 //Debug vital values sent to server
#endif

struct db_result{
    int count;
    char** res;
};

typedef struct db_result db_result;

void init_db(void);
void init_table(void);
int exec_stmt(const char*);
int close_db(void);
#endif