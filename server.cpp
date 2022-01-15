<<<<<<< HEAD
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

int init(int);

int main(int argc, char* argv[]){
  int sockfd, newfd;
  struct addrinfo hints, *res, *p;
  char buff[INET6_ADDRSTRLEN];
  struct sockaddr_storage their_addr;
  int addr_size = sizeof their_addr;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if(getaddrinfo("192.168.1.199", "1025", &hints, &res) != 0){
    perror("ERROR AT GETADDRINFO");
    exit(2);
  }
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    exit(-1);
  }
  bind(sockfd, res->ai_addr, res->ai_addrlen);
  listen(sockfd, 20);
  while(1){
    newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if(!fork()){
      //init(newfd);
      char buff[512];
      read(newfd, buff,sizeof buff);
      exit(0);
    }else{
      printf("Parent process, closing socket\n");
      close(sockfd);
      exit(0);
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
=======
//#include <stdlib.h>
//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//#include <string.h>
//#include <netdb.h>
//
//int main(int argc, char* argv[]){
//  int sockfd, newfd;
//  struct addrinfo hints, *res, *p;
//  char buff[INET6_ADDRSTRLEN];
//  struct sockaddr_storage their_addr;
//  int addr_size = sizeof their_addr;
//  memset(&hints, 0, sizeof hints);
//  hints.ai_family = AF_INET;
//  hints.ai_socktype = SOCK_STREAM;
//  hints.ai_flags = AI_PASSIVE;
//  getaddrinfo("192.168.1.199", "1025", &hints, &res);
//  sockfd = socket(PF_INET, SOCK_STREAM, 0);
//  bind(sockfd, res->ai_addr, res->ai_addrlen);
//  listen(sockfd, 20);
//  while(1){
//    newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
//    if(!fork()){
//      
//    }else{
//      printf("Parent process, closing socket\n");
//      close(sockfd);
//      exit(1);
//    }
//  }
//}
//
//
//int init(int sock){
//  int oxy_val = 0;
//  while(1){
//    read(sock, &oxy_val, sizeof int);
//  }
>>>>>>> cdb86928e0ae53a38a2e510dd3807066f0a54fb7
