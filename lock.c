/* 17:32 2015-04-18 星期六 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
char buf[1024];

void *rlock_read(void *rbuf)
{
    int n;
    while (1)
    {
        pthread_mutex_lock(&lock);
    if ((n = strlen(rbuf)) != 0) 
    {
        /*fputs(rbuf,stdout);*/
        fprintf(stdout,"Read %d types %s\n",n,(char *)rbuf);
    }
    pthread_mutex_unlock(&lock);
    if (strncmp(rbuf,"quit",4) == 0)
    {
        break;
    }
    }
    return;
}
void *wlock_write(void *wbuf)
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        fgets(wbuf,sizeof(wbuf),stdin);
        buf[strlen(wbuf)-1] = '\0';
        pthread_mutex_unlock(&lock);
        if (strncmp(wbuf,"quit",4) == 0)
        {
            break;
        }
    usleep(500);
    }
    return;
}

int main(int argc, const char *argv[])
{
    int ret;
    pthread_t pth[2];

    ret = pthread_create(&pth[0],NULL,rlock_read,buf);
    if (ret != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&pth[1],NULL,wlock_write,buf);
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
