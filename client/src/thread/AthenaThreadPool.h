//
// Created by Wesly Zhong on 2025/10/12.
//

#ifndef ATHENA_ATHENATHREAD_H
#define ATHENA_ATHENATHREAD_H

#include "common/ThreadPool.h"

class AthenaThread : public  Thread::Worker{

};

class AthenaThreadPool : public Thread::ThreadPool {
public:
    Thread::Worker *createThread() ;

    void deleteThread(Thread::Worker *t);

    void completeTask(Thread::TaskPtr task) ;
};


#endif //ATHENA_ATHENATHREAD_H
