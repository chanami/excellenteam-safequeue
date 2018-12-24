
#include <iostream>
#include "safequeue.h"
#define NUM_OF_THREADS 10

using namespace std;

struct arg_struct {
    SafeQueue<int> * arg1;
    int arg2;
};

void *PushRun(void *queue)
{
    struct arg_struct *args = (arg_struct*)queue;


    int i=-1;
    while(i < NUM_OF_THREADS)
    {
        std::cout<<"thread --> " << args->arg2 <<" PUSH"<<std::endl;
        (*(SafeQueue<int>*)args->arg1).push(++i) ;
    }
    
    return 0;
}

void *PopRun(void *queue)
{
    struct arg_struct *args = (arg_struct*)queue;

    int i=-1;
    while( ++i < NUM_OF_THREADS)
    {
        std::cout<<"thread --> " << args->arg2 << " POP ";
        std::cout << (*(SafeQueue<int>*)args->arg1).pop()<< std::endl;
    }
    return 0;
}

void test_push()
{
    int item, back;
    item = 9;

    SafeQueue<int> * sQueue = new SafeQueue<int>();

    sQueue -> push(item);
    back = sQueue->pop();
    if(back == item)
        std::cout<<"Test push OK\n";
    else
        std::cout<<"Test push FAILED\n";

}
void test_2_threads()
{
    pthread_t t1, t2;
    struct arg_struct args1,args2;
    SafeQueue<int> * sQueue = new SafeQueue<int>();

    args1.arg1 = sQueue;
    args1.arg2 = 1;

    args2.arg1 = sQueue;
    args2.arg2 = 2;

    pthread_create(&t1, NULL, PushRun, (void *)&args1);
    pthread_create(&t2, NULL, PopRun, (void *)&args2);


    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

}

int main()
{

    test_push();
    test_2_threads();

    return 0;
}
