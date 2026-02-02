#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//serverport and buffer size
#define SERVER_PORT 8080
#define BUFFER_SIZE 50 //

int main(){
    int sd, l, on = 1; //socket
    int req_sd;
    char buf[BUFFER_SIZE];
    struct sockaddr_in channel;
    struct sockaddr_in clientaddr;
    int clientaddrlen;

   // pid_t pid;
    //set and allow address
    memset(&channel, 0, sizeof(channel)); // fill struct with 0
    channel.sin_family = AF_INET; //declare that ip is ipv4 
    channel.sin_addr.s_addr = htonl(INADDR_ANY); //declares what address to listen on - on serverside, listens on all own 
    channel.sin_port = htons(SERVER_PORT);
    
    
    //SETUP REQUEST SOCKET:
    //SOCK_DGRAM for UDP CONNECTION
    req_sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    //PASSIVE WAIT FOR CONNECTION
    //SERVER: BIND, WAIT, AND LISTEN TO CONNECTIONS
    bind(req_sd, (struct sockaddr *)&channel, sizeof(struct sockaddr_in));
    listen(req_sd, SOMAXCONN); //socket, queue size - MAX CONNECTION 
    printf("LISETING TO PORT: %d\n", SERVER_PORT);
    clientaddrlen = sizeof(struct sockaddr_in);
    
    //keeps server alive
    while(1){
        sd = accept(req_sd, (struct sockaddr*) &clientaddr, &clientaddrlen);

        read(sd, buf, BUFFER_SIZE-1);
        buf[BUFFER_SIZE-1] = '\0';
        printf("%s\n",buf);
        write(sd, "HELLO WORLD FROM SERVER\n", BUFFER_SIZE-1);

        close(sd);
    }
    close(req_sd);
    return 0;
}