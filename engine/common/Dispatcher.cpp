#include "Dispatcher.hpp"
Dispatcher::Dispatcher(/* args */)
{
}

Dispatcher::~Dispatcher()
{
}

Disruptor<int>* Dispatcher::disrupter = new Disruptor<int>(10);

bool Dispatcher::dispatcher(int a){
   disrupter->push(a);
   return true;
}