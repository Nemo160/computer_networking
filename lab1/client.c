#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//serverport and buffer size
#define SERVER_PORT 8080
#define BUFFER_SIZE 50//
#define SERVER_IP "127.0.0.1"


int main(){
    int sd; //socket
    char buf[BUFFER_SIZE];
    struct sockaddr_in serveraddr;
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //

    //set and allow address
    memset(&serveraddr, 0, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET; //declare that ip is ipv4 
    inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);//gör om ip strängen till rätt format
    serveraddr.sin_port = htons(SERVER_PORT);
    
    //connect(socket, struct of serveraddr as a struct sockaddr, length of sockaddr)
    connect(sd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    //write and 
    const char *msg = "hello";
    write(sd, "hello world from client!\n", BUFFER_SIZE-1);

    //read(sd, buf, BUFFER_SIZE-1);
    read(sd, buf, BUFFER_SIZE-1);
    
    buf[BUFFER_SIZE-1] = '\0';
    printf("%s\n",buf);
    //
    close(sd);
    return 0;
}