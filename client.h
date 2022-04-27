#ifndef __client__
#define __client__

//Debug vital values sent to server
#ifndef DEBUG
#define DEBUG
#endif

void get_pt_info();

int recv_data(int socket_fd);

#endif
