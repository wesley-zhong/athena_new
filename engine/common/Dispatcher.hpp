#ifndef DISPATCHER_H_
#define DISPATCHER_H_
#include "Disrupter.hpp"
class Dispatcher
{
private:
  static Disruptor<int>*disrupter;

public:
    Dispatcher(/* args */);
    ~Dispatcher();
    static  bool dispatcher(int a);
};

#endif