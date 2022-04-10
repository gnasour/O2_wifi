#ifndef __client__
#define __client__

//Debug vital values sent to server
#ifndef DEBUG
#define DEBUG 0
#endif

int read_data(int socket_fd);

#endif