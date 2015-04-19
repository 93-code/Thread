/* 12:05 2015-04-19 Sunday */
#include <head.h>

int main(int argc, const char *argv[])
{
    int n = 0;
    char ch = 'a';
    int fd[2];

    if (pipe(fd) < 0)
    {
        perror("Fail to pipe");
        exit(EXIT_FAILURE);
    }
    
    fprintf(stdout,"fd[0] = %d,fd[1] = %d\n",fd[0],fd[1]);

    while (1)
    {
        write(fd[1],&ch,1);
        n ++;
        printf("n = %d\n",n);
    }
    
    fprintf(stdout,"Write %d types\n",n);

    return 0;
}
