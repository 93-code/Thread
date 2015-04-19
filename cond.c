/* 11:28 2015-04-19 Sunday */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char buf[1024];

void *rlock_read(void *rbuf)
{
    int n;
    while (1)
    {
        /*pthread_mutex_lock(&lock);*/
        if (pthread_cond_wait(&cond,&lock) < 0)
        {
           perror("Fail to pthread_cond_wait");
            exit(EXIT_FAILURE);
        }
        if ((n = strlen(buf)) != 0) 
        {
            /*fputs(buf,stdout);*/
            fprintf(stdout,"Read %d types %s\n",n,(char *)buf);
        }
        pthread_mutex_unlock(&lock);
        if (strncmp(buf,"quit",4) == 0)
        {
            break;
        }
    }
    pthread_exit(NULL);
}
void *wlock_write(void *wbuf)
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1] = '\0';
        pthread_mutex_unlock(&lock);

        if (pthread_mutex_lock(&lock) < 0)
        {
            perror("Fail to pthread_mutex_lock");
            exit(EXIT_FAILURE);
        }
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
        if (strncmp(buf,"quit",4) == 0)
        {
            break;
        }
        usleep(500);
    }
    pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
    int ret;
    pthread_t pth[2];

    ret = pthread_create(&pth[0],NULL,rlock_read,NULL);
    if (ret != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&pth[1],NULL,wlock_write,NULL);
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

//################
//##注释：如果把buf作为参数，通过pthread_create传给函数会造成数据缓存未清空（fflush）
//################
