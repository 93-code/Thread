/* 12:05 2015-04-19 Sunday */
#include <head.h>

int main(int argc, const char *argv[])
{
    int n;
    int fd[2];
    char buf[1024];
    char *string = "hello world";

    if (pipe(fd) < 0)
    {
        perror("Fail to pipe");
        exit(EXIT_FAILURE);
    }

    /*close(fd[0]);*/
    n = write(fd[1],string,strlen(string));
    
    fprintf(stdout,"Write %d types\n",n);

    /*close(fd[1]);*/
    n = read(fd[0],buf,sizeof(buf)-1);
    buf[n] = '\0';

    fprintf(stdout,"Read %d types : %s\n",n,buf);

    return 0;
}
