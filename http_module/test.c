

#include"http_module.h"



int main(int argc,char **argv){


  int fd=open("temp",O_CREAT|O_RDWR);
  char localPath[512]={0};
  int eNum=accept_request(fd,localPath);     
  printf("error : %d\n localPath is %s\n",eNum,localPath);

    return 0;
}