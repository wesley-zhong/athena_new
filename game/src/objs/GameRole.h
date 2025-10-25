#ifndef GAME_ROLE_HPP_
#define GAME_ROLE_HPP_
#include "ObjectPool.hpp"
#include "XLog.h"
class GameRole :public  ObjPool::PoolObjClass<GameRole>
{
private:
   uint32_t  pid;

    /* data */
public:
    GameRole(/* args */) {

    };
    GameRole(uint32_t pid) {
        this->pid = pid;
    }
    ~GameRole() {
      INFO_LOG("------ CALL  ~GameRole");
    };
    uint32_t  getPid(){
        return pid;
    }

    void setPid(uint32_t pid){
        this->pid = pid;
    }

};

#endif