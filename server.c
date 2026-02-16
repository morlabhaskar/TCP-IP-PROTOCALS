#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
int main(int argc,char *argv[]){
    if(argc<2){
        printf("cmdLine : a.out  portNo\n");
        return 0;
    }
    int sfd,cfd,status;
    char rbuff[100];
    printf("Creating Socket :\n");
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd<0){
        perror("socket");
        return 0;
    }
    printf("Socket Creating Successfully\n");
    printf("bind : \n");
    struct sockaddr_in saddr;
    int size = sizeof(saddr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(atoi(argv[1]));
    saddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sfd,(struct sockaddr *)&saddr,size)<0){
        perror("bind");
        return 0;
    }
    printf("Bind Successfully\n");
    printf("Creating a Connection Queue for 1 client\n");
    if(listen(sfd,1)<0){
        perror("listen");
        return 0;
    }
    printf("Waiting passively connection for any client\n");
    struct sockaddr_in caddr;
    int clen = sizeof(caddr);
    cfd=accept(sfd,(struct sockaddr *)&caddr,&clen);
    if(cfd<0){
        perror("accept");
        return 0;
    }
    printf("Client is Connected :\n");
    char *clientIp = inet_ntoa(caddr.sin_addr);
    unsigned short port = ntohs(caddr.sin_port);
    printf("Client IP : %s\n",clientIp);
    printf("Client Port : %hu\n",port);
    while(1){
        printf("Waiting for any msg from client...\n");
        status = recv(cfd,rbuff,sizeof(rbuff),0);
        char len[50];
        int l=strlen(rbuff);
        snprintf(len, sizeof(len), "%d", l);
        if(status<0){
            perror("recv");
            break;
        }
        else if(status==0){
            printf("Client is Aborted without 3-way HS\n");
            break;
        }
        else{
            printf("Client msg : %s\n",rbuff);
            status = send(cfd,rbuff,sizeof(rbuff),0);
            // status = send(cfd,len,sizeof(len),0);
            if(status<0){
                perror("send");
                break;
            }
            if(strcmp(rbuff,"quit")==0){
                break;
            }
        }
    }
    printf("closing server connection with client\n");
    close(cfd);
    printf("Server going down\n");
    close(sfd);
}

