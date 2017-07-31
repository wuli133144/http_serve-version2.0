
#include "../inc/http_config.h"



// #define __info(x,y)     fprintf(stderr,x,y);
// #define mkstr(x)                                  #x

// #define   check(cond,err_table,fmt,...)          do{                 \
//           if(cond){                                                  \
//                 __info("******error occured******|%s",mkstr(cond));  \
//                printf(fmt,__VA_ARGS__);                              \
//                exit(0);                                             }\                                                     \               
//  }while(0)

#include<sys/wait.h>

#define DEF_PAGER "/bin/more"


#define unix_error(x)           \
 do{                            \
   fprintf(stderr,"error %s\n",x);\
  exit(0);                      \
 }while(0)

int main(int argc, char **argv) {
   //fork


   int n;
   int fd[2];
   int i;
   pid_t pid;
   char *pager,*argv;
   char line[BUFSIZE];
   FILE *fp;
   if(argc!=2){
       fprintf("usage :a.out<pathname>");
   }
   if((fp=fopen(argv[1],"r")!=NULL){
      unix_error("fopen");
   }
   if((pipe(fd))<0){
        unix_error("pipe error");
   }
   if((pid=fork())<0){
         unix_error("fork error!");
   }else if(pid>0){
        close(fd[0]);//close readport

        while((fgets(line,BUFSIZE,fp)!=NULL)){
                      int len=strlen(line);
                      write(fd[1],line,n);
        }
        
        if(!ferrror(fp)){
            unix_error("fopen error!");
        }
        close(fd[1]);
        wait(&i);
        return 0;
   }else{ 
          close(fd[1]);//close write pipe port
          if(fd[0]!=STDIN_FILENO)
            {   if((dup2(fp[0],STDIN_FILENO))!=STDIN_FILENO){
                    unix_error("dup2");
                }
             close(fd[0]);
            }
            
          exit(0);
   }

  //return 0;
}