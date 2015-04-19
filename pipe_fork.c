/* 12:27 2015-04-19 Sunday */
#include <head.h>

void read_pipe(int fd)
{
    int n;
    char buf[1024];

    while (1)
    {
        n = read(fd,buf,sizeof(buf)-1);
        buf[n] = '\0';

        fprintf(stdout,"Read %d types : %s\n",n,buf);

        if (strncmp(buf,"quit",4) == 0)
        {
            break;
        }
    }
    return;
}

void write_pipe(int fd)
{
    char buf[1024];

    while (1)
    {
        //fgets() will auto plus '\0';
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1] = '\0';

        write(fd,buf,strlen(buf));
        fprintf(stdout,"Write %d types : %s\n",strlen(buf),buf);

        if (strncmp(buf,"quit",4) == 0)
        {
            break;
        }
    }
    return;
}
int main(int argc, const char *argv[])
{
    pid_t pid;
    int fd[2];

    if (pipe(fd) < 0)
    {
        perror("Fail to pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        close(fd[1]);
        read_pipe(fd[0]);
    }

    if (pid > 0)
    {
        close(fd[0]);
        write_pipe(fd[1]);
    }
    return 0;
}
