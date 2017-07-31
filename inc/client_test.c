
#include"http_task.h"

#if 1

int main(){ 
   // init_main_proc();
   #if 1
      
//      FILE *fp=fopen(conf,"rw");
//      if(fp==NULL){
//          printf("FP");
//          }
//          else{
//                printf("fp open failed");
//          }
   
    
    char buf[BUFSIZE];
    bzero(buf,BUFSIZE);

    int fd=open_clientfd("127.0.0.1","8080");

    if(fd<0){
          check("open clientfd");
   }
    while((fgets(buf,BUFSIZE,stdin))!=NULL){
            
            write(fd,buf,BUFSIZE);
            
             read(fd,stdout,BUFSIZE);
         
    }

    return 1;
   #endif
   #if 0

         create_msg_queue();   
        //   if ((qid = msgget(0x223, IPC_CREAT |IPC_CREAT| 0666)) == -1) {
        //   check("msgget");
        //     }
        //     printf("%d",qid);
          sleep(5);
         destroy();
          
    
   #endif
}
#endif
#if 0

int main(int argc, char *argv[])    
{    
  
    
    int msgid = open_queue();  
     
     struct _msg msg;
     msg.mtype=1;
     int len=sizeof(struct _msg)-sizeof(long);

    // struct msgbuf *buf;    
    // buf = (struct msgbuf *)malloc(len + sizeof(msgbuf::mtype));    
    // buf->mtype = type;    
    if (msgsnd(msgid, &msg, len, IPC_NOWAIT) == -1)  //非阻塞   
        printf("23q4w5");  
      // send_queue(msgid,&msg);
}    

#endif
