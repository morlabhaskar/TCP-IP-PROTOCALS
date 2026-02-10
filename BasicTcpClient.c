#include"headers.h"
int main(int argc,char *argv[]){
    if(argc<3){
        puts("input @cmdline : client.exe server portNo serverIP");
        return 0;
    }
    puts("creating client socket...");
    int fd;
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd<0){
        perror("socket");
        return 0;
    }
    puts("client socket created successfully");
    puts("connect:");
    puts("& initiating 3-way HS for CE with server");
    struct sockaddr_in addr;
    int len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[2]);
    addr.sin_port = htons(atoi(argv[1]));
    if(connect(fd,(struct sockaddr *)&addr,len)<0){
        perror("connect");
        return 0;
    }
    puts("connected to server");
    char buf[100];
    int status;
    while(1){
        puts("Enter any msg for server");
        fgets(buf,100,stdin);
        buf[strlen(buf)-1]='\0';
        puts("sending msg to server...");
        status=send(fd,buf,100,0);
        if(status<0){
            perror("send");
            break;
        }else{
            printf("msg sent to server : %s\n",buf);
            puts("waiting for echo reply...");
            status=recv(fd,buf,100,0);
            if(status<0){
                perror("recv");
                break;
            }else if(status==0){
                puts("server aborted without 3-way HS for CT");
                break;
            }else{
                printf("echo msg recvd from server : %s\n",buf);
                if(strcmp(buf,"quit")==0){
                    break;
                }
            }
        }
    }
    puts("client going down");
    close(fd);
}