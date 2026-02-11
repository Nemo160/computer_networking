#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>


#define SERVER_PORT 8080 //DEFAULT
#define SERVER_IP "127.0.0.1"
#define RFC_TIME 2208988800UL


int main(int argc, char *argv[]){
    int port = SERVER_PORT;
    if(argc > 1){
        port = atoi(argv[1]);
    }
    
    int opt =1;
    int sd;

    struct sockaddr_in serveraddr;
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //
    if(sd <0){
        printf("ER:SOCK\n");
        return -1;
    }
    // if(setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &opt, sizeof(opt)) < 0){
    //      printf("setsocketopt to SO_REUSEADDR");
    //  }

    //set and allow address
    memset(&serveraddr, 0, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET; //declare that ip is ipv4 
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);

    //SEND FROM CLIENT
    char req = 0;
    sendto(sd, NULL,0, 0, (struct sockaddr*) &serveraddr, sizeof(serveraddr));

    uint32_t rfc_time;
    ssize_t n = recvfrom(sd, &rfc_time, sizeof(rfc_time), 0, NULL, NULL);
    if(n<0){
        printf("ER:READ\n");
        close(sd);
        return -1;
    }

    //convert time
    uint32_t new_rfc = ntohl(rfc_time);
    time_t time_now = (time_t)(new_rfc - RFC_TIME);
    printf("%s", ctime(&time_now));

    close(sd);
    return 0;
}
