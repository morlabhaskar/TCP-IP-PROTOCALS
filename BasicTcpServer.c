#include "headers.h"
//./a.out  3000  
int main(int argc,char *argv[]){
    if(argc<2){
        puts("Input @cmdline:server.exe portNo");
        return 0;
    }
    puts("Creating Server Socket...\n");
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd < 0){
        perror("socket");
        return 0;
    }
    puts("Server Socket created successfully");
    puts("bind :");
    puts("Associating Server Socket file with IP & PortNo");
    struct sockaddr_in saddr;
    int slen = sizeof(saddr);
    saddr.sin_family = AF_INET;//Specifies IPv4 addressing
    saddr.sin_port = htons(atoi(argv[1]));
    saddr.sin_addr.s_addr = INADDR_ANY;//Server accepts connections on any local IP ,Works for: (127.0.0.1) (192.168.x.x)
    if(bind(sfd,(const struct sockaddr *)&saddr,slen)<0){
        perror("bind");
        return 0;
    }
    puts("bind success");
    puts("listening:");
    puts("creating connection queue for 1 client(s)");
    if(listen(sfd,1)<0){
        perror("listen");
        return 0;
    }
    puts("Connection queue created");
    puts("waiting passively connection for any client");
    struct sockaddr_in caddr;
    int clen=sizeof(caddr),cfd;
    cfd=accept(sfd,(struct sockaddr *)&caddr,&clen);
    if(cfd<0){
        perror("accept");
        return 0;
    }
    puts("connection from client accepted");
    char *clientIP=inet_ntoa(caddr.sin_addr);
    unsigned short clientPort = ntohs(caddr.sin_port);
    printf("Client's IP : %s\n",clientIP);
    printf("Client's PortNo : %hu\n",clientPort);
    char rbuff[100];
    int status;
    while(1){
        puts("waiting for any msg from client...");
        status = recv(cfd,rbuff,100,0);
        if(status<0){
            perror("recv");
            break;
        }else if(status==0){
            puts("client aborted without 3-way HS for CT");
            break;
        }else{
            printf("msg recvd from client : %s\n",rbuff);
            puts("echoing recvd msg...");
            if(send(cfd,rbuff,100,0)<0){
                perror("send");
                break;
            }
            printf("echoed to client : %s\n",rbuff);
            if(strcmp(rbuff,"quit")==0){
                break;
            }
        }
    }
    puts("closing server connection with client");
    close(cfd);
    puts("server going down");
    close(sfd);
}
