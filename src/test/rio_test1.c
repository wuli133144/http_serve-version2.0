#include "../../utils/rio_r_w.h"

//
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define ngx_test_null(p, alloc, rc) \
    if ((p = alloc) == NULL)        \
    {                               \
        return rc;                  \
    }

#define zero_test_null(p, func) \
    if ((p = func) < 0)         \
    {                           \
        perror(#func);          \
        exit(0);                \
      }

int main()
{

    // int fd = open("info", O_RDONLY | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    // write(fd, "12345678", 8);
    // if (fd < 0)
    // {
    //     goto err;
    // }
    // rio_t rio;
    // char usrbuf[100];
    // bzero(usrbuf, 100);
    // rio_readinit(&rio, fd);
    // rio_read(&rio, usrbuf, 100);
    // printf("%s", usrbuf);

    // DIR *dir = opendir("../");
    // struct dirent *p;
    // while ((p = readdir(dir)) != NULL)
    // {
    //     printf("%s\n", p->d_name);
    // }
    // closedir(dir);
    pid_t pid;
    zero_test_null(pid, fork);
    if (pid == 0)
    {
        printf("fork child!");
    }
    else
    {
        printf("fork parent");
    }
    return 0;
}