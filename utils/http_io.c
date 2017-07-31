
#include "http_io.h"
/*@readn disctrption start@*/
ssize_t http_Readn(int fd, void *buffer, size_t len)
{
    size_t nleft = len;
    size_t nread;
    char *ptr;
    ptr = buffer;

    while (nleft > 0)
    {
        if ((nread = read(fd, buffer, len)) < 0)
        { //error
            if (errno == EINTR)
            {
                //break;
                nread = 0;
            }
            else
            {
                break;
            }
        }
        else if (nread == 0)
        { //eof
            break;
        }
        nleft -= nread;
        ptr += nread;
    }
    return len - nleft;
}
/*@readn end@*/

/*@writen start@*/

ssize_t http_Writen(int fd, void *buffer, size_t len)
{
    size_t nleft = len;
    size_t nwrite = 0;
    void *ptr = buffer;
    while (nleft > 0)
    {
        if ((nwrite = write(fd, ptr, len)) < 0)
        {
            if (errno == EINTR)
            {
                nwrite = 0;
            }
            else
            {
                break;
            }
        }
        else if (nwrite == 0)
        {
            break;
        }
        nleft -= nwrite;
        ptr += nwrite;
    }
    return len - nleft;
}
/*@writen end@*/

/*@Fgets() iobuffer @*/
ssize_t http_Fgets_s(int fd, void *buffer, size_t sz)
{
    size_t nleft = sz, read_cnt;
    char c, *ptr;
    ptr = buffer;
    while (nleft > 0)
    {
        if ((read_cnt = read(fd, &c, 1)) < 0)
        {
            if (errno == EINTR) //intr
                read_cnt = 0;
            else
            {
                return (-1);
            }
        }
        else if (read_cnt == 0)
        { //eof
            *ptr = 0;
            goto end;
        }
        *ptr++ = c;
        nleft--;
        if (c == '\n')
        { //换行
            break;
        }
    }

    *ptr = 0;

end:
    return ((void *)ptr - buffer);
}
/*@Fgets() iobufffer@*/

/*@Fget() start@*/
char *http_Fgets(char *buffer, size_t n, FILE *fp)
{
    char c, *pc;
    pc = buffer;
    while (--n > 0 && (c = getc(fp)) != EOF)
    {
        *pc++ = c;
        if (c == '\n')
        {
            *pc = 0;
            break;
        }
    }
    *pc = 0;
    return ((c == EOF) && (pc == buffer)) ? NULL : buffer;
}
/*@Fget()end@*/