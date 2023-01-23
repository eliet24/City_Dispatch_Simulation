#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

typedef struct Event{
    u_int8_t depart;
    char address[100];
    char event_info[100];
    char severity[10];
    struct Event *link;
} Event;


int main()
{ 
    Event event;
    int status;
    int pid;
    int num;
    char info[50];
    Event *ptr=(Event*)malloc(sizeof(Event));
    //Event *ptr;
    int fd5 = open("/dev/pts/7", O_RDWR | O_NOCTTY | O_NDELAY);
    int stop_flag =70000;  //set stop flag for the program execution time
    while(1 && (stop_flag != 0))         //by enable line 50
    {
        num = read(fd5,ptr,sizeof(Event));
        if(num!=-1)
        {
            printf("Event Description: %s\n", ptr->event_info);
            pid = fork();
            if(pid==0){
                printf("child:event processed\n");
                sleep(2);
                exit (1);
            }
            else if(pid!=0){
                wait(&status);
                printf("PARENT: Child's exit code is: %d\n", WEXITSTATUS(status));
                printf("PARENT: Event handling completed, Goodbye!\n");
            }
        }
        //stop_flag--;
    }
    free(ptr);
    close(fd5);
    return 0;
}







