#ifndef EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_CHANAMI_SAFEQUEUE_H
#define EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_CHANAMI_SAFEQUEUE_H

#include <semaphore.h>
#include <pthread.h>
#include <queue>
#include <iostream>

template<class T,size_t SIZE = 5>
class Safe_queue {

public:
    Safe_queue();
    ~Safe_queue();
    T pop();
    void push(T &);
    /*void isEmpty();*/
private:
    std::queue <T> the_queue;
    pthread_mutex_t lock;
    sem_t pushSem;
    sem_t popSem;
    void initSynchronized();
    void freeSynchronized();

};

template< class T, size_t SIZE >
Safe_queue <T,SIZE>::Safe_queue()
{
    initSynchronized();
}

template< class T, size_t SIZE >
Safe_queue <T,SIZE>::~Safe_queue()
{
    freeSynchronized();
}
template< class T, size_t SIZE >
void Safe_queue <T,SIZE>::initSynchronized()
{
    sem_init(&pushSem, 0, SIZE);
    sem_init(&popSem, 0, 0);
    pthread_mutex_init(&lock, NULL);

}

template< class T, size_t SIZE >
void Safe_queue <T,SIZE>::freeSynchronized()
{
    sem_destroy(&pushSem);
    sem_destroy(&popSem);
    pthread_mutex_destroy(&lock);

}

template< class T, size_t SIZE >
void Safe_queue<T,SIZE>::push(T & item){
    sem_wait(&pushSem);
    pthread_mutex_lock(&lock);

    the_queue.push(item);

    pthread_mutex_unlock(&lock);
    sem_post(&popSem);
}
template< class T, size_t SIZE >
T Safe_queue<T,SIZE>::pop(){
    T temp;
    sem_wait(&popSem);
    pthread_mutex_lock(&lock);

    temp = the_queue.pop();

    pthread_mutex_unlock(&lock);
    sem_post(&pushSem);
    return temp;
}
#endif
