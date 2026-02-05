#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 37

int main(){
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0); //IPPROTO_UDP
    if(udp_socket < 0){
        printf("ER:socket\n");
        return -1;
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr)); // fill struct with 0
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((bind(udp_socket, (struct sockaddr*) &serveraddr, sizeof(serveraddr))) < 0){
        close(udp_socket);
        printf("ER:bind\n");
        return -1;
    }

    //CONNECTION
    printf("bind on port:%d", SERVER_PORT);
    while(1){
        char buf[2048];
        struct sockaddr_in clientaddr;
        socklen_t clientlen = sizeof(clientaddr);

        ssize_t n = recvfrom(udp_socket, buf, sizeof(buf)-1, 0, (struct sockaddr*)&clientaddr, &clientlen);
        if(n <0 ){
            printf("ER: recive\n");
            continue;
        }
        buf[n] = '\0';

        printf("Got %d bytes from %s:%d: \"%s\"\n",
               n,
               inet_ntoa(clientaddr.sin_addr),
               ntohs(clientaddr.sin_port),
               buf);

        const char reply[] = "hello";
        if(sendto(udp_socket, reply, strlen(buf)+1, 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
           printf("ERROR: SEND\n");
        } 
    }
    
    printf("success\n");
    close(udp_socket);
    return 0;
}