#ifndef EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_CHANAMI_SAFEQUEUE_H
#define EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_CHANAMI_SAFEQUEUE_H

#include <semaphore.h>
#include <pthread.h>
#include <queue>
#include <iostream>

template<class T,size_t SIZE = 5>
class SafeQueue {

public:
    SafeQueue();
    ~SafeQueue();

    T pop();
    void push(const T &);
    
private:
    
    std::queue <T> m_queue;
    pthread_mutex_t lock;
    sem_t pushSem;
    sem_t popSem;
    
    void initSynchronized();
    void freeSynchronized();

};

template< class T, size_t SIZE >
SafeQueue <T,SIZE>::SafeQueue()
{
    initSynchronized();
}

template< class T, size_t SIZE >
SafeQueue <T,SIZE>::~SafeQueue()
{
    freeSynchronized();
}

template< class T, size_t SIZE >
void SafeQueue <T,SIZE>::initSynchronized()
{
    int status1, status2, status3;
    status1 = sem_init(&pushSem, 0, SIZE);
    status2 = sem_init(&popSem, 0, 0);
    status3 = pthread_mutex_init(&lock, NULL);
    if((status1 == -1) || (status2 == -1) || (status3 != 0))
    {
        std::cout<<"\n init synchronized failed";
        freeSynchronized();
        throw std::invalid_argument(" init synchronized failed ");
    }

}

template< class T, size_t SIZE >
void SafeQueue <T,SIZE>::freeSynchronized()
{
    sem_destroy(&pushSem);
    sem_destroy(&popSem);
    pthread_mutex_destroy(&lock);

}

template< class T, size_t SIZE >
void SafeQueue<T,SIZE>::push(const T & item){

    sem_wait(&pushSem);
    pthread_mutex_lock(&lock);

    m_queue.push(item);

    pthread_mutex_unlock(&lock);
    sem_post(&popSem);
}

template< class T, size_t SIZE >
T SafeQueue<T,SIZE>::pop(){
    T temp;
    sem_wait(&popSem);
    pthread_mutex_lock(&lock);

    temp = m_queue.front();
    m_queue.pop();

    pthread_mutex_unlock(&lock);
    sem_post(&pushSem);

    return temp;
}
#endif
