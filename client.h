#ifndef __client__
#define __client__

//Debug vital values sent to server
#ifndef DEBUG
#define DEBUG
#endif

int recv_data(int socket_fd);

#endif
