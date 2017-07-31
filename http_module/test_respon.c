#include "http_module.h"


#include "../inc/sock.h"


int main(){
  
   int clientfd;
   int listenfd=open_listenfd("127.0.0.1","8089");
   
   struct sockaddr_in sock;
   socklen_t len;
   int fd;

    while(1){
           
           clientfd=Accept(listenfd,(struct sockaddr*)&sock,&len);
           while(clientfd<0){
                 clientfd=Accept(listenfd,(struct sockaddr*)&sock,&len);
           }
            char buf[BUFFSIZE];
           // 505  http协议不支持


           fd=fork();
           if(fd==0){
                    close(listenfd);
                    char buf[BUFFSIZE];
                    bzero(buf,BUFFSIZE);

                    struct stat st;

                    int fd=open("temp",O_RDWR);
                    while(fd<0){
                        fd=open("temp",O_RDWR);
                    }
                    
                    while((fstat(fd,&st)<0)){
                        fstat(fd,&st);
                    }
                    int len=st.st_size;
                    printf("size=%d",len);
                     
                     /*
                        HTTP/1.1 200 OK
                        Date: Sat, 31 Dec 2005 23:59:59 GMT
                        Content-Type: text/html;charset=ISO-8859-1
                        Content-Length: 122
                     */

                    sprintf(buf, "HTTP/1.0 200 OK \r\n");
                    send(clientfd, buf, strlen(buf),0);
                    
                    sprintf(buf,"Content-Length:%d\r\n",7);
                    http_send(clientfd,buf,strlen(buf));
                    sprintf(buf,"Content-Type: application/octet-stream ");
                    http_send(clientfd,buf,strlen(buf));
                    sprintf(buf,"Date: Wed, 16 Nov 2005 01:57:54 GMT\r\n");
                    http_send(clientfd,buf,strlen(buf));
                    sprintf(buf,"Connection:close\r\n\r\n");
                    http_send(clientfd,buf,strlen(buf));
                    int i=0;
                    int nread=0; 
                    int nwrite=0;
                    write(clientfd,"fasdgh\0",1234);

                
                    // while(1){
                          
                    //       nread=readline(fd,buf,strlen(buf));
                    //       printf("%d\n",nread);
                    //       while(nread==-1){
                    //           nread=readline(fd,buf,strlen(buf));
                    //       }
                    //       http_send(clientfd,buf,strlen(buf));
                    //       if(nread==0){
                    //           break;
                    //       }
                          
                    
                    close(fd);
                    __info();
                    exit(127);
         }
    
    close(clientfd);
    }
     


}