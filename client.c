#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
int main(int argc,char *argv[]){
    if(argc<3){
        printf("cmdLine : a.out  portNo  serverIP\n");
        return 0;
    }
    int fd,status;
    char buff[100];
    printf("Creating a Socket:\n");
    fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd<0){
        perror("socket");
        return 0;
    }
    printf("Socket Created Successfully\n");
    printf("Connection with Server:\n");
    struct sockaddr_in addr;
    int size=sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);
    if(connect(fd,(struct sockaddr *)&addr,size)<0){
        perror("connect");
        return 0;
    }
    printf("Connection Establised with Server\n");
    while(1){
        printf("Enter the Message to send Server :\n");
        fgets(buff,sizeof(buff),stdin);
        buff[strlen(buff)-1]='\0';
        status = send(fd,buff,sizeof(buff),0);
        if(status<0){
            perror("send");
            break;
        }
        printf("Client Sending Msg : %s\n",buff);
        printf("Waiting for Server Reply :\n");
        status = recv(fd,buff,sizeof(buff),0);
        if(status<0){
            perror("recv");
            break;
        }
        else if(status == 0){
            printf("Server is Aborted with out 3 way HandShake\n");
            break;
        }
        else{
            printf("Server Reply Msg : %s\n",buff);
            if(strcmp(buff,"quit")==0){
                printf("Server is Terminated With 3-way HS\n");
                break;
            }
        }
    }
    printf("Client is Going Down\n");
    close(fd);
}