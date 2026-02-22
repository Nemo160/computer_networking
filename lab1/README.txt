Build instructions - Webb and Tid
Each program is ran with port as an argument.
If the argument is left empty the default port is set to 8080 

WEBB:
Example build -> gcc webb.c -o webb
Example run -> ./web <port> 

TID
Build seperate
Example build:
gcc server.c -o server
gcc client.c -o client

Example run:
./server <port>
./client <port> 



    inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);
