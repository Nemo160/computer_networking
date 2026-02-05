#include <time.h>
#include <stdio.h>
#include <sys/types.h>
int main(){
    time_t tidID = time(NULL);
    printf("TIME: %ld\n", tidID);
    struct tm *ltime = localtime(&tidID);
    printf("localtime %s", asctime(ltime));
    // int time = first-last;
    //printf("%ld", time);
    
    return 0;
}