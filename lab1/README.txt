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

After compiling run each program in seperate terminals.
Example run:
./server <port>
./client <port> 


