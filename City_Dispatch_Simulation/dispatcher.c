#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <syslog.h>


typedef struct Event{
    u_int8_t depart;
    char address[100];
    char event_info[100];
    char severity[10];
    struct Event *link;
} Event;

int events_num=1;
int send_police = 0;
int send_fire = 0;
int send_ambulance = 0;
int send_maintenance = 0;
Event *front_disp; 
Event *rear_disp;

#define BUFFER_SIZE 30

void insert_event(Event event){
    events_num++;
    Event *temp=(Event*)malloc(sizeof(Event));
    printf("Adding event \n");
    *temp = event;
    temp->link = NULL;
    if (rear_disp  ==  NULL)
    {
        front_disp = rear_disp = temp;
    }
    else
    {
        rear_disp->link = temp;
        rear_disp = temp;
    }    
}

// delete elements from queue

Event pop_event()
{
    events_num--;
    Event *temp;
    Event event;
    temp = front_disp;
    printf("Deleting event \n");
    if (front_disp == NULL)
    {
        printf("queue is empty \n");
        front_disp = rear_disp = NULL;
    }
    else
    {    
        event = *front_disp;
        front_disp = front_disp->link;
    }
    free(temp);
    return event;
}

void simulate_events()
{
    FILE *log;
    char dataToAppend[BUFFER_SIZE];
    int ret;
    Event event;
    Event send_event;
    int i = 0;
    int num;
    int fd0, fd1, fd2, fd3, log_fd;
    fd0 = open("/dev/pts/2", O_RDWR | O_NOCTTY | O_NDELAY);
    fd1 = open("/dev/pts/6", O_RDWR | O_NOCTTY | O_NDELAY);
    fd2 = open("/dev/pts/9", O_RDWR | O_NOCTTY | O_NDELAY); 
    fd3 = open("/dev/pts/12", O_RDWR | O_NOCTTY | O_NDELAY);  
    char w_buffer[60];
    log = fopen("./log.txt","a");
    if (log == NULL){
        /* Unable to open file hence exit */
        printf("\nUnable to open file.\n");
        }   
    for(int j=0;j<500;j++)
    {
        if(i%4 == 0){
            event.depart = 0;
            strcpy(event.address,"police_station");
            strcpy(event.event_info,"bank robbery");
            strcpy(event.severity,"critical");            
            send_police++;
            strcpy(w_buffer,"NEW Event for the POLICE department");
            fputs(w_buffer, log);
        }
        else if(i%4 == 1){
            event.depart = 1;
            strcpy(event.address,"Fire_station");
            strcpy(event.event_info,"burning building");
            strcpy(event.severity,"critical");
            send_fire++;
            strcpy(w_buffer,"NEW Event for the FIRE department");
            fputs(w_buffer, log);
        }
        else if(i%4 == 2){
            event.depart = 2;
            strcpy(event.address,"ambulance");
            strcpy(event.event_info,"heart attack");
            strcpy(event.severity,"critical");
            send_ambulance++;
            strcpy(w_buffer,"NEW Event for the AMBULANCE department");
            fputs(w_buffer, log);
        }
        else if(i%4 == 3){
            event.depart = 3;
            strcpy(event.address,"shalom street 16/6");
            strcpy(event.event_info,"broken_road");
            strcpy(event.severity,"critical");
            send_maintenance++;
            strcpy(w_buffer,"NEW Event for the MAINTENANCE department");
            fputs(w_buffer, log);
        }
        insert_event(event);
        i++;
        if (i > 30)        //setting delay for sending Events to department
        {
            send_event = pop_event();
            printf("%s,  ",send_event.event_info);
            sleep(1);                           //approve functionality
            if(send_event.depart == 0){  
                num = write(fd0,&send_event,sizeof(Event));
                if(num!=-1){
                    printf(" Event -%d-  was sended to the POLICE department\n", i);
                    printf("The event info: %s\n", send_event.event_info);
                    strcpy(w_buffer,"Event was sended to the POLICE department");
                    fputs(w_buffer, log);
                    }
                }
            else if(send_event.depart == 1)
            {  
                num = write(fd1,&send_event,sizeof(Event));
                if(num!=-1){
                    printf("Event -%d-  was sended to the FIRE department\n", i);
                    printf("The event info: %s\n", send_event.event_info);
                    strcpy(w_buffer,"Event was sended to the FIRE department");
                    fputs(w_buffer, log);
                    }
            }
            else if(send_event.depart == 2)
            {
                num = write(fd2,&send_event,sizeof(Event));
                if(num!=-1){
                    printf("Event -%d-  was sended to the Ambulance department\n", i);
                    printf("The event info: %s\n", send_event.event_info);
                    strcpy(w_buffer,"Event was sended to the AMBULANCE department");
                    fputs(w_buffer, log);
                    }
            }
            else if(send_event.depart == 3)
            {
                num = write(fd3,&send_event,sizeof(Event));
                if(num!=-1){
                    printf("Event -%d-  was sended to the Maintenance department\n", i);
                    printf("The event info: %s\n", send_event.event_info);
                    strcpy(w_buffer,"Event was sended to the MAINTENANCE department");
                    fputs(w_buffer, log);
                    }
            }
            close(log_fd);
        }
    }
    for(int j=0; j<30; j++){
        send_event = pop_event();
        }   
    close(fd0); close(fd1); close(fd2); close(fd3);  //closing channels
    fclose(log);
}

int main(){
    simulate_events();
    return 0;
}