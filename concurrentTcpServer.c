#include"headers.h"
int sfd,cfd,status;
struct sockaddr_in saddr,caddr;
int slen=sizeof(saddr),clen=sizeof(caddr);
char rbuff[100],*clientIp;
unsigned short clientPort;
void clientInfo(){
    clientIp=inet_ntoa(caddr.sin_addr);
    clientPort=ntohs(caddr.sin_port);
    printf("Client's IP : %s\n",clientIp);
    printf("Client's PortNo : %hu\n",clientPort);
}
void signalHandler(int status){
    wait(&status);
    puts("Child exiting...");
}
//./concSr 3000
//./cl 3000 127.0.0.1
int main(int argc,char *argv[]){
    signal(SIGCHLD,signalHandler);
    if(argc<2){
        puts("input @cmdline:server.exe  server  portNo");
        return 0;
    }
    puts("Creating Server Socket:");
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd<0){
        perror("socket");
        return 0;
    }
    puts("server socket created successfully");
    puts("bind");
    puts("associating server socket file with IP & PortNo");
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=INADDR_ANY;
    saddr.sin_port=htons(atoi(argv[1]));
    if(bind(sfd,(const struct sockaddr*)&saddr,slen)<0){
        perror("bind");
        return 0;
    }
    puts("bind success");
    puts("linstenning");
    puts("creating connection queue for 1 client(s)");
    if(listen(sfd,1)<0){
        perror("listen");
        return 0;
    }
    puts("connection Queue created");
    while(1){
        puts("waiting pssively for connection from any client");
        cfd=accept(sfd,(struct sockaddr*)&caddr,&clen);
        if(cfd<0){
            perror("accept");
        }
        puts("connection from client accepted");
        if(fork()==0){
            while(1){
                puts("waiting for any msg from client");
                status = recv(cfd,rbuff,100,0);
                if(status<0){
                    perror("recv");
                    break;
                }
                else if(status==0){
                    puts("Client aborted without 3-way HS for CT");
                    break;
                }
                else{
                    printf("msg recvd from client : %s\n",rbuff);
                    puts("echoing recvd msg..");
                    if(send(cfd,rbuff,100,0)<0){
                        perror("send");
                        break;
                    }
                    printf("echoed to client : %s\n",rbuff);
                    clientInfo();
                    if(strcmp(rbuff,"quit")==0){
                        puts("client formally terminating intimation");
                        break;
                    }
                }
            }
            puts("server closing with connected client");
            clientInfo();
            close(cfd);
            puts("child of server exiting...");
            exit(0);
        }
    }
    puts("server going down");
    close(sfd);
}