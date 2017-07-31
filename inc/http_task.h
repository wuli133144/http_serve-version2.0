#ifndef __HTTP_TASK_H
#define __HTTP_TASK_H

#include "http_ipc.h"
#include "list.h" //========>freebsd kernelcode
#include "types.h"
#include "http_error.h"
#include "http_epoll.h"
#include "sys/wait.h"
#include <sys/resource.h>
#include <pthread.h>
#include <err.h>
#include<sys/file.h>

#include "../http_module/http_module.h"

#define NPROC_MAX_NUM 10
#define MAX_FD_NUM 200

#define PROC_POOL proc_pool

typedef struct __processor__node {
  i_32 pid;
  i_32 nleft;
  i_32 status;
  SLIST_ENTRY(__processor__node) entry;
} processor_t;

/*@global processor pool object@*/
SLIST_HEAD(s_pool_t, __processor__node)
proc_pool = SLIST_HEAD_INITIALIZER(proc_pool);

/*@global processor pool object@*/

void open_max_fd() {
  struct rlimit rlim;
  if ((getrlimit(RLIMIT_NOFILE, &rlim)) < 0) {
    err(1, "error:%s", "getrlimit");
    exit(0);
  }
  if (rlim.rlim_max > MAX_FD_NUM) {
    rlim.rlim_max = MAX_FD_NUM;
  }
}

processor_t *proc_alloc() {
  processor_t *rva = NULL;
  if ((rva = (processor_t *)malloc(sizeof(processor_t))) == NULL) {
    unix_error("malloc failed!");
  }
  rva->pid = -1;
  rva->nleft = MAX_FD_NUM;
  rva->status=0;
  return rva;
}

/*@create list @*/
void create_proc_pool() {
  processor_t *item = NULL;
  int i = 0;
  for (i = 0; i < NPROC_MAX_NUM; i++) {
    item = proc_alloc();
    SLIST_INSERT_HEAD(&proc_pool, item, entry);
  }
}
/*@create list end@*/

/*@insert object to pool@*/
void insert_pool_obj(pid_t pid) {
  processor_t *item = NULL;
  SLIST_FOREACH(item, &PROC_POOL, entry) {
    if (item->pid == -1) {
      item->pid = pid;
      PROC_POOL.n_proc++;
      break;
    }
  }
}
/*@insert object to pool end@*/

void delete_pool_obj(int pid) {
  processor_t *item = NULL;
  processor_t *pre = SLIST_FIRST(&PROC_POOL);
  SLIST_FOREACH(item, &proc_pool, entry) {
    if (item->pid == pid) {
      if (item == SLIST_FIRST(&PROC_POOL)) {
        SLIST_FIRST(&PROC_POOL) = SLIST_NEXT(item, entry);
        free(item);
        item = NULL;
        break;
      }
      SLIST_NEXT(pre, entry) = SLIST_NEXT(item, entry);
      free(item);
      item = NULL;
      break;
    }
    pre = item;
  }
  PROC_POOL.n_proc--;
}
/*@delete_pool_object@*/

void insert_pool(pid_t pid) {
  processor_t *item = NULL;
  if ((PROC_POOL.n_proc) < NPROC_MAX_NUM) {
    item = proc_alloc();
    item->pid = pid;
    SLIST_INSERT_HEAD(&PROC_POOL, item, entry);
  }
}


void *pthread_handler(void *argv) {
  int connfd = *((int *)argv);
  printf("pthread_handler=%d",connfd);
  Open_epoll(connfd);
  pthread_detach(pthread_self());
  return NULL;
}

// global var

int g_oconnfd;
int pipe_main_master[2];
int pipe_master_child[2];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conflock = PTHREAD_COND_INITIALIZER;


// /*unix_domain*/
// int S_pipe(int fd[2]){
      
//       return socketpair(AF_UNIX,SOCK_STREAM,0,fd);
// }
// /*unix end*/

void sig_handler_child(int sig){
     

}

int cross_ok(int pid){
      processor_t *item;
      SLIST_FOREACH(item,&PROC_POOL,entry)
      {
          if(item->pid=pid&&item->status==1){
            return 0;
          }
      }
      return -1;
}


/*@child proc handler proc@*/
void jump_task_pool_obj(int fd[2]) {

      int clientfd,result,fd_set,i,filefd;
      char buf[2];
      struct epoll_event events[EPOLLEVENTS];
      close(fd[1]);
      epollfd= Epoll_create(FDSIZE);
      add_event(epollfd, fd[0], EPOLLIN);
      for(;;){
        //wait pthread
        //no wait
         int cnt=Epoll_wait(epollfd,events,EPOLLEVENTS,-1);
         
         for(i=0;i<cnt;i++){
                 fd_set=events[i].data.fd;
                 if(fd_set==fd[0]){
                      Sock_fd_read(fd[0],buf,2,&clientfd);
                      struct epoll_event events;
                      events.data.fd=clientfd; 
                      events.events=EPOLLIN|EPOLLET;
                      
                      Epoll_ctl(epollfd,EPOLL_CTL_ADD,clientfd,&events);
                 }else if(events[i].events&EPOLLIN){
                             if(clientfd==fd_set){//clientfd read
                                  info_t *info=http_module_handler_request(epollfd,(void *)&clientfd);
                                  struct epoll_event events1;
                                  events1.data.fd=clientfd; 
                                  events1.events=EPOLLOUT;
                                  events1.data.ptr=(void *)info;
                                  Epoll_ctl(epollfd,EPOLL_CTL_MOD,clientfd,&events1);    
                                  
                                  //add file operation readable
                                  // struct epoll_event events2;
                                  // events2.data.fd=filefd;//file fd 
                                  // events2.events=EPOLLIN;
                                  // Epoll_ctl(epollfd,EPOLL_CTL_MOD,filefd,&events2);  
                             }
                 }
                 else if(events[i].events&EPOLLOUT){
                                  
                                  info_t *info=(info_t *)events[i].data.ptr;
                                  int err=info->errtype;
                                  char filename[BUFFSIZE];
                                  bzero(filename,BUFFSIZE);
                                  strcpy(filename,info->filename);
                                  //char *filename=info->filename;
                                  http_module_handler_response(filename,clientfd,err);
                                  close(clientfd);
                                 
                 }



         }

      }
     
        
       

     
           
}

/*@child proc handler proc@*/

/*@ handler_dead_processor@*/
void handler_dead_processor(pid_t pid,int fd[2]) {

  delete_pool_obj(pid);
  pid = fork();
  if (pid < 0) {
    unix_error("fork failed");
  } else if (pid == 0) {
    // new child
    jump_task_pool_obj(fd);
  }
  insert_pool(pid);
  return;
}

// get best suitable proc
pid_t notice_child() {
  processor_t *item = NULL;
  processor_t *pre=NULL;
  pid_t pid;
  int max = SLIST_FIRST(&PROC_POOL)->nleft;

  SLIST_FOREACH(item, &PROC_POOL, entry) {
    if (item->nleft >= max) {
     
      max = item->nleft;
      pid = item->pid;
   
      pre=item;
    }
  }
 
  pre->nleft--;

  return pid;
}

/*@TELL CHLD TO EXIT WHEN ACCEPT SIGINT SIGNAL@*/
void tell_chld_exit() {

  processor_t *item = NULL;
  SLIST_FOREACH(item, &PROC_POOL, entry) { kill(item->pid, SIGINT); }
}
/*@TELL CHLD TO EXIT WHEN ACCEPT SIGINT SIGNAL END@*/


void set_status_ok(pid_t pid){
    processor_t *item=NULL;

    SLIST_FOREACH(item,&PROC_POOL,entry){
        if(item->pid==pid){
          item->status=1;
        }
    }
}



/*@init manager proc@*/

void init_manager_proc(int fd[2]) {

  // get msg queue info from main proc
  int i = 0;
  pid_t pid;

  create_proc_pool();

  for (; i < NPROC_MAX_NUM; i++) {
    
    pid = fork();
    if (pid < 0) {
      unix_error("fork failed!");
    } else if (pid == 0) {
      sleep(2);
      jump_task_pool_obj(fd);
    }
     insert_pool_obj(pid);
  }

  return;
}
/*@init manager proc@*/


#endif