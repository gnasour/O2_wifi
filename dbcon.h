#ifndef __dbcon__
#define __dbcon__

#ifndef DEBUG
#define DEBUG 0 //Debug vital values sent to server
#endif

void init_db(void);
int exec_stmt(const char*);
int close_db(void);
#endif