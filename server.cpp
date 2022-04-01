#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>


//After connection, initiate main server
int init(int);

int main(int argc, char* argv[]){

  //Server Socket FD, New Request FD
  int sockfd, newfd;
  //Finding the address of the ESP8266
  struct addrinfo hints, *res, *p;
  //IPv4 Address of the ESP8266
  char buff[INET_ADDRSTRLEN];
  struct sockaddr_storage their_addr;
  int addr_size = sizeof their_addr;

  //Packing addrinfo struct to retrieve necessary address information
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  //Retrieving necessary address information of the local server
  if(getaddrinfo(NULL, "1025", &hints, &res) != 0){
    perror("ERROR AT GETADDRINFO");
    exit(2);
  }

  //Creating server socket to listen for connections
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  
  if(sockfd < 0){
    exit(-1);
  }

  //Naming socket and listening
  int x = bind(sockfd, res->ai_addr, res->ai_addrlen);
  listen(sockfd, 20);
  
  //Main loop
  while(1){
    newfd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t*)&addr_size);

    //Create child process to handle the accepted connection
    int proc_id = fork();
    if(proc_id == 0){

      //init(newfd);

      close(sockfd);
      
      char buff[512];
      int amt_read;
      while(amt_read = read(newfd, buff,(sizeof buff)-1)){
	buff[amt_read] = '\0';
	printf("%s\n", buff);
      }

      //close(newfd);
      //exit(0);
    }else{
      printf("Parent process, closing socket\n");
      close(newfd);
    }
  }
}


int init(int sock){
  int oxy_val = 0;
  while(1){
    read(sock, &oxy_val, sizeof(int));
    printf("%d\n", oxy_val);
  }
  return -1;
}
