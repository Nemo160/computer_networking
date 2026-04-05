#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>


#define SERVER_PORT 3737 //DEFAULT
#define SERVER_IP "127.0.0.1" //localhost
#define RFC_TIME 2208988800UL


int main(int argc, char *argv[]){
    int port = SERVER_PORT;
    if(argc > 1){
        port = atoi(argv[1]);
    }
    int sd;

    struct sockaddr_in serveraddr;
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //
    if(sd <0){
        printf("ER:SOCK\n");
        return -1;
    }


    //set and allow address
    memset(&serveraddr, 0, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET; //declare that ip is ipv4 
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr); //from string to byte format

    //SEND FROM CLIENT
    char req = 0;
    if(sendto(sd, &req,1, 0, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0){
        printf("ER: SENDTO");
        close(sd);
        return -1;
    }

    uint32_t rfc_time;
    ssize_t n = recvfrom(sd, &rfc_time, sizeof(rfc_time), 0, NULL, NULL); //recv time from server
    if(n<0){
        printf("ER:READ\n");
        close(sd);
        return -1;
    }

    //convert time back to unix time and undo htonl conversion
    uint32_t new_rfc = ntohl(rfc_time); 
    time_t time_now = (time_t)(new_rfc - RFC_TIME);
    printf("%s", ctime(&time_now));

    close(sd);
    return 0;
}
