#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//serverport and buffer size
#define SERVER_PORT 37
#define BUFFER_SIZE 2048//
#define SERVER_IP "127.0.0.1"


int main(){
    int sd; socket(AF_INET, SOCK_DGRAM, 0); // socket

    //set and allow address
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; //declare that ip is ipv4 
    serveraddr.sin_port = htons(SERVER_PORT);
    if(inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr) != 1){
        printf("INET FAILED\n");
        close(sd);
        return -1;
    }//gör om ip strängen till rätt format

    const char *msg = "hello";
    if (sendto(sd, msg, sizeof(msg), 0,
        (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0) {
        printf("sendto\n");
        close(sd);
        return -1;
    }

    char buf[BUFFER_SIZE];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);

    ssize_t n = recvfrom(sd, buf, sizeof(buf)-1 , 0, (struct sockaddr*)&from, &fromlen);
    if(n < 0){
        printf("ERR: recieve\n");
        close(sd);
        return -1;
    }

    buf[n] = '\0';
    printf("Replied with:%s\n",buf);
    
    close(sd);
    return 0;
}