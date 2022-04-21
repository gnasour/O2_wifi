/**
 * @file server.c
 * @author George Nassour
 * @brief Server application that will open up a socket connection for an incoming device 
 *        and create a new process to handle reading and writing data from the said device
 * @version 0.1
 * @date 2021-10-18
 * 
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

#include "client.h"
#include "dbcon.h"


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
  bind(sockfd, res->ai_addr, res->ai_addrlen);
  listen(sockfd, 20);

  //Initialize database connection for clients to store data
  init_db();
  
  //Main loop
  while(1){
    newfd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t*)&addr_size);
    printf("Connection made with new device\n");
    //Create child process to handle the accepted connection
    int proc_id = fork();
    if(proc_id < 0){
      perror("Error on main loop fork");
    }else if(proc_id == 0){
      close(sockfd);
      setbuf(stdout, NULL);
      //Create temp file to write data
      char template[] = "./dataXXXXXX";
      int data_fd = mkstemp(template);
      if(data_fd == -1){
	      perror("Error creating temp data file in child");
	      exit(errno);
      }
      //      unlink(template);
      while(1){
	      recv_data(newfd, data_fd);
      }
    }else{
      printf("Parent process, closing socket\n");
      close(newfd);
    }
  }
}

