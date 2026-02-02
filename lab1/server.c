
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
#define BF_SIZE 4096 //
#define BF_METADATA_SIZE 256
#define INDEX_FILE "sample_website/index.html"


//
void print_error(const char *s);
void recv_client_req(int client_sock);
void parse_GET_req(char *filepath, int client_sock);
int send_JPG(char *file_name, int client_sock);
int send_HTML(char *file_name, int client_sock);
void send_404(int client_sock);

char *make_filepath(const char *filename);



int main(){
    int client_fd, opt = 1;
    int req_sd;
    struct sockaddr_in addr;
    struct sockaddr_in clientaddr;
    int clientaddrlen;

    //SETUP REQUEST SOCKET:
    //SOCK_DGRAM for UDP CONNECTION

    req_sd = socket(AF_INET, SOCK_STREAM, 0);
    if(req_sd <0) {
        print_error("SOCKET COULD NOT BE CREATED!\n");
        exit(1);
    }
    if(setsockopt(req_sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        printf("setsocketopt to SO_REUSEADDR");
    }

    printf("SOOCKET CREATED\n");
    memset(&addr, 0, sizeof(addr)); // fill struct with 0
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //declares what address to listen on - on serverside, listens on all own 
    addr.sin_port = htons(SERVER_PORT);
   
    //PASSIVE WAIT FOR CONNECTION
    //SERVER: BIND, WAIT, AND LISTEN TO CONNECTIONS

    if(bind(req_sd, (struct sockaddr*) &addr, sizeof(addr)) < 0){
        print_error("ER:bind\n");
        exit(1);
    }

    if((listen(req_sd,10)) < 0){
        print_error("ER:listen\n");
        exit(1);
    }

    printf("LISETING TO PORT: %d\n", SERVER_PORT);
    clientaddrlen = sizeof(struct sockaddr_in);
    
    //keeps server alive
    while(1){
        client_fd = accept(req_sd, (struct sockaddr*) &clientaddr, &clientaddrlen);
        printf("ACCEPTED\n");
       // client_fd = accept(req_sd, 0, 0);

        recv_client_req(client_fd);
        printf("CLOSING CLIENT_FD\n");
        close(client_fd);

    }
    
    close(req_sd);
    return 0;
}

void recv_client_req(int client_sock){

    char buffer[BF_METADATA_SIZE] = {0};
    int n = recv(client_sock, buffer, BF_METADATA_SIZE-1, 0);
    printf("recv returned n=%d\n", n);

    if(n < 0){
        return;
    }
    buffer[n] = '\0';
    printf("BUFFER CLIENT: %s\n", buffer);

    //potentially handle other than GET requests
    //
    //
    if(strncmp(buffer, "GET ", 4) != 0){
        send_404(client_sock);
        return;
    }

    char* file_name = buffer + 4;  
    char* sp = strchr(file_name, ' '); 
    if(sp == NULL){
        send_404(client_sock);    
        return;
    }
    // printf("SP:::BEFORE\n%s\n",sp);
    *sp = '\0';
  //  printf("SP:::%s\n",sp);


    printf("\nFILENAME:%s\n", file_name);
    parse_GET_req(file_name, client_sock); //sends and closes file
    printf("SENT!\n");

}

//Parse what type of file client is requesting
void parse_GET_req(char *file_type, int client_sock){
    //char *filepath = parse_filepath(file_name); 
    //file_name looks like this -> /file.html with its slash intact /

    if(strstr(file_type, "jpg") != NULL){
        send_JPG(file_type, client_sock);
        printf("IS JPG!\n");

    }
    else if(strstr(file_type, "html") != NULL){
        send_HTML(file_type, client_sock);

    }
    else if(strcmp(file_type, "/") == 0){
        send_HTML(file_type, client_sock);

    }
    // if(strstr(file_name, " ") != NULL){
    //     char *HEADER = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n";
    //     send(client_sock, HEADER, strlen(HEADER), 0);
    // }
    else{
        printf("IN 404\n");
        send_404(client_sock);
    }
    
}

void send_404(int client_sock){
    char header[BF_METADATA_SIZE];
    int size = 0;
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 404 NOT FOUND\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n",
        size
    );
    send(client_sock, header, header_len, 0);
}


int send_HTML(char *file_name, int client_sock){
    char buffer[BF_SIZE] = {0};
    char* filepath = make_filepath(file_name);
    printf("HERE IS FILEPATH:%s\n",filepath);
    FILE *f = fopen(filepath, "r");

    if(!f){
        print_error("COULD NOT OPEN FILE");
        free(filepath);
        return -1;
    }
    fseek(f, 0, SEEK_END); //"sets the file position indicator to end of file."
    long size = ftell(f); // "obtains current value of file position indicator for stream pointed to by stream"
    rewind(f); // "sets file position indicator to the begining"

    char header[BF_METADATA_SIZE];
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n",
        size
    );
    send(client_sock, header, header_len, 0);
    size_t bytes;
    while((bytes = fread(buffer, 1, BF_SIZE, f)) > 0){
        send(client_sock, buffer, bytes, 0);
    }
    fclose(f);
    free(filepath);
    return 1;
}


int send_JPG(char *file_name, int client_sock){
    char buffer[BF_SIZE] = {0};
    char* filepath = make_filepath(file_name); //<- this function should return correct filepath for the requsted file
    printf("HERE IS FILEPATH:%s\n",filepath);

    FILE *f = fopen(filepath, "rb");
    if(!f){
        print_error("COULD NOT OPEN FILE");
        free(filepath);
        return -1;
    }

    fseek(f, 0, SEEK_END); //"sets the file position indicator to end of file."
    long size = ftell(f); // "obtains current value of file position indicator for stream pointed to by stream"
    rewind(f); // "sets file position indicator to the begining"

    char header[BF_METADATA_SIZE];
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: image/jpeg\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n",
        size
    );
    send(client_sock, header, header_len, 0);
    size_t bytes;
    while((bytes = fread(buffer, 1, BF_SIZE, f)) > 0){
        send(client_sock, buffer, bytes, 0);
    }
    fclose(f);
    free(filepath);
    return 1;
}

char *make_filepath(const char *filename){
    printf("IN PARSE FILE PATH\n");
    char *index = "sample_website/index.html";
    char *base = "sample_website/";

    if((strcmp(filename, "/")) == 0){
        char *path = malloc(strlen(index)+1);
        strcpy(path, index);
        return path;
    }
    if(filename[0] == '/'){ filename++; }

    char *path = malloc(strlen(filename) + strlen(base)+1);
    strcpy(path,base);
    strcat(path,filename);
    return path;
}
void print_error(const char *s){
    printf("%s\n", s);
}