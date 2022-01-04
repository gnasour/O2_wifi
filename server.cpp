#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

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
  getaddrinfo("192.168.1.199", "1025", &hints, &res);
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  bind(sockfd, res->ai_addr, res->ai_addrlen);
  listen(sockfd, 20);
  while(1){
    newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if(!fork()){
      fprintf(stderr, "Its open\n");
      exit(1);
    }else{
      printf("Parent process, closing socket\n");
      close(newfd);
      exit(1);
    }
  }
  /*  for(p = res; p != NULL; p= p -> ai_next){
    void* addr;
    char* ipver;

    if(p->ai_family == AF_INET){
      struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }else{
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }
    inet_ntop(p->ai_family, addr, buff, sizeof buff);
    printf("%s: %s\n", ipver, buff);

    }*/
  printf("%d\n", sockfd);

}
