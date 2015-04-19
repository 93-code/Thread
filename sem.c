/* 17:32 2015-04-18 星期六 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

sem_t rsem;
sem_t wsem;
char buf[1024];

void *rsem_read(void *rbuf)
{
    int n;
    while (1)
    {
        sem_wait(&rsem);
    if ((n = strlen(rbuf)) != 0) 
    {
        /*fputs(rbuf,stdout);*/
        fprintf(stdout,"Read %d types %s\n",n,(char *)rbuf);
    }
    sem_post(&wsem);
    if (strncmp(rbuf,"quit",4) == 0)
    {
        break;
    }
    }
    return;
}
void *wsem_write(void *wbuf)
{
    while (1)
    {
        sem_wait(&wsem);
        fgets(wbuf,sizeof(wbuf),stdin);
        buf[strlen(wbuf)-1] = '\0';
        sem_post(&rsem);
        if (strncmp(wbuf,"quit",4) == 0)
        {
            break;
        }
    }
    return;
}

int main(int argc, const char *argv[])
{
    int ret;
    pthread_t pth[2];
    sem_init(&rsem,0,0);
    sem_init(&wsem,0,1);

    ret = pthread_create(&pth[0],NULL,rsem_read,buf);
    if (ret != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&pth[1],NULL,wsem_write,buf);
    if (ret != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }
    //if no pthread_join,the program will terminate immediately
    pthread_join(pth[0],NULL);
    pthread_join(pth[1],NULL);

    return 0;
}
