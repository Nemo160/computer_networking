#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define SERVER_PORT 37 //DEFAULT
#define RFC_TIME 2208988800UL

int main(int argc, char *argv[]){
    int port = SERVER_PORT;
    if(argc > 1){
        port = atoi(argv[1]);
    }

    int opt = 1;
    int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //IPPROTO_UDP
    if(udp_socket < 0){
        printf("ER:socket\n");
        return -1;
    }
    if(setsockopt(udp_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        printf("setsocketopt to SO_REUSEADDR");
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr)); //init struct with 0
    serveraddr.sin_family = AF_INET; //use ipv4 address
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if((bind(udp_socket, (struct sockaddr*) &serveraddr, sizeof(serveraddr))) < 0){
        //close(udp_socket);
            printf("ER:bind %d\n", udp_socket);
        return -1;
    }

    //CONNECTION
    printf("bind on port:%d\n", port);
    while(1){
        char test[1];
        struct sockaddr_in clientaddr;
        socklen_t clientlen = sizeof(clientaddr);

        ssize_t n = recvfrom(udp_socket, test, sizeof(test), 0, (struct sockaddr*)& clientaddr, &clientlen);
        //recieves data and stores in test. Save who sent it into clientaddr. 
        //returns how many bytes were recieved.

        if(n < 0 ){
            printf("ER: recive\n");
            continue;
        }

        time_t tid = time(NULL);
        uint32_t rfc = (uint32_t)(tid + RFC_TIME);  //converts unix time (1970) to rfc (1900)
        uint32_t time = htonl(rfc); // convert to big endian so all computers understand
        

        if(sendto(udp_socket, &time, sizeof(time), 0, (struct sockaddr*)&clientaddr, clientlen) < 0){
            //send rfc time to saved client
            printf("ERROR: SEND\n");
        } 
    }
    
    printf("success\n");
    close(udp_socket);
    return 0;
}
