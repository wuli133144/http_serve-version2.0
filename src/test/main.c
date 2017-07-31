
//
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include<stdio.h>
#include<string.h>

#include<errno.h>
#include<stdlib.h>



void unix_error(char *msg){
      fprintf(stderr,"%s %s",msg,strerror(errno));
      exit(0);
}


typedef struct __node
{
    void *data;
    int len;
} t_node;

// RB_ls

#include<stdarg.h>

 /*@execlp start@*/
/* $end forkwrapper */

void Execve(const char *filename, char *const argv[], char *const envp[]) 
{
    if (execve(filename, argv, envp) < 0)
	    //unix_error("Execve error");
        perror("execve error");
}

 /*@execlp end@*/
//popen的使用



// #define  exe_test_null(ret,func,arg)    \ 
//          if((ret=func(arg))==NULL){     \
//               perror(#func);            \
//               exit(0);                  \
//          }



//单向管道使用
#define PAGER_DEF         "/bin/more"


int main(int argc,char** argv)
{
    //char * argv[ ]={"ls","-al","/etc/passwd",(char *)0};
   // char * envp[ ]={"PATH=/bin"clang-format0};
   // Execve("/bin/ls",argv,NULL);
    


    return 0;
}
