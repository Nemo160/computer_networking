
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>

//serverport and buffer size
#define SERVER_PORT 8080
#define BUFFER_SIZE 4096 //
#define INDEX_FILE "sample_website/index.html"


//
void print_error(const char *s);
void recv_req(int client_fd, int sock);
void parse_GET_req(char *filepath, int client_sock);
void send_JPG(char *file_name, int client_sock);
void send_HTML(char *file_name, int client_sock);
void send_404(int client_sock);

char *make_filepath(const char *filename);



int main(){
    int client_fd = 1; //socket
    int bytes, fd;
    int req_sd;
    char buf[BUFFER_SIZE];
    struct sockaddr_in addr;
    struct sockaddr_in clientaddr;
    int clientaddrlen;

    //SETUP REQUEST SOCKET:
    //SOCK_DGRAM for UDP CONNECTION
    req_sd = socket(AF_INET, SOCK_STREAM,0);
    if(req_sd <0) {
        print_error("SOCKET COULD NOT BE CREATED!\n");
    }
   // memset(&addr, 0, sizeof(addr)); // fill struct with 0
    addr.sin_family = AF_INET; //declare that ip is ipv4 
    addr.sin_addr.s_addr = INADDR_ANY; //declares what address to listen on - on serverside, listens on all own 
    addr.sin_port = htons(SERVER_PORT);
   
    //PASSIVE WAIT FOR CONNECTION
    //SERVER: BIND, WAIT, AND LISTEN TO CONNECTIONS
    int b = bind(req_sd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
    if(b<0){print_error("COULD NOT BIND ADDRESS!\n");}
    int l = listen(req_sd, 10);
    if(l<0){print_error("NOT LISTENING!\n");} //socket, queue size - MAX CONNECTION 
    printf("LISETING TO PORT: %d\n", SERVER_PORT);
    clientaddrlen = sizeof(struct sockaddr_in);
    
    //keeps server alive
    while(1){
        client_fd = accept(req_sd, (struct sockaddr*) &clientaddr, &clientaddrlen);
       // client_fd = accept(req_sd, 0, 0);
        if(client_fd < 0){
            printf("invalid SOCKET\n");
        }
        recv_req(client_fd, req_sd);
        printf("SENT!\n");
       
        close(client_fd);
        close(req_sd);
        return 0;
    }
    return 0;
}

void recv_req(int client_fd, int sock){
    printf("IN RECV REQ\n");
    char buffer[256] = {0};
    recv(client_fd, buffer, 256, 0);
    printf("BUFFER CLIENT: %s\n", buffer);
    char* file_name = buffer + 5; 

    *strchr(file_name, ' ') = 0; //assign spaces with null terminator leaving us with the file
    printf("\nFILENAME: %s\n", file_name);
    parse_GET_req(file_name, client_fd); //sends and closes file

}

//Parse what type of file client is requesting
void parse_GET_req(char *file_name, int client_sock){
    printf("IN SEND SERVER\n");
    //char *filepath = parse_filepath(file_name); 
    if(strstr(file_name, "jpg") != NULL){
        send_JPG(file_name, client_sock);
        printf("IS JPG!\n");
        return;
    }
    if(strstr(file_name, "html") != NULL){
        send_HTML(file_name, client_sock);
    }
    else{
        printf("IN 404\n");
        send_404(client_sock);
        //if invalid request maybe? send 404 not found
    }
    
}

void send_404(int client_sock){
    char *HEADER = "HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n";
    send(client_sock, HEADER, strlen(HEADER), 0);
}


void send_HTML(char *file_name, int client_sock){
    char bf[BUFFER_SIZE] = {0};
    FILE *f = fopen(file_name, "r");

    if(!f){print_error("COULD NOT OPEN FILE"); return;}

    size_t bytes;
    char *HEADER = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    send(client_sock, HEADER, strlen(HEADER), 0);

    while((bytes = fread(bf, 1, BUFFER_SIZE, f)) > 0){
        send(client_sock, bf, bytes, 0);
    }
    fclose(f);
}


void send_JPG(char *file_name, int client_sock){
    char bf[BUFFER_SIZE] = {0};
    //make_path(file_name) <- this function should return correct filepath for the requsted file
    FILE *f = fopen(file_name, "r");

    if(!f){print_error("COULD NOT OPEN FILE"); return;}

    size_t bytes;
    char *HEADER = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n\r\n";
    send(client_sock, HEADER, strlen(HEADER), 0);

    while((bytes = fread(bf, 1, BUFFER_SIZE, f)) > 0){
        send(client_sock, bf, bytes, 0);
    }
    fclose(f);
}

char *make_filepath(const char *filename){
    printf("IN PARSE FILE PATH\n");
    char *path = "sample_website/index.html";
    //printf("%s\n", filename);
    return path;
}
void print_error(const char *s){
    printf("%s\n", s);
}