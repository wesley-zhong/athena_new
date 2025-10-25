extern "C" {
#include "lua/lua.h"  
#include "lua/lauxlib.h"  
#include "lua/lualib.h"  
}

#include <stdio.h>
#include <stdlib.h>
#include "../common/XLog.h"
#include "sol/sol.hpp"
#include "pb.h"

#include <map>

extern void luabind_netserver(sol::state & lua);
extern void luabind_kcpserver(sol::state & lua);
extern void luabind_websocket(sol::state & lua);
extern void luabind_eventloop(sol::state & lua);
extern void luabind_mysql(sol::state & lua);
extern void luabind_redis(sol::state & lua);
extern void luabind_basepacket(sol::state & lua);
extern void luabind_common(sol::state & lua);
extern void luabind_csvpar(sol::state & lua);
extern void luabind_json(sol::state & lua);
extern void luabind_httpserver(sol::state & lua);


void init_lua_pb(lua_State* L)
{
	luaL_requiref(L, "pb", luaopen_pb, 0);
	luaL_requiref(L, "pb.slice", luaopen_pb_slice, 0);
	luaL_requiref(L, "pb.buffer", luaopen_pb_buffer, 0);
	luaL_requiref(L, "pb.conv", luaopen_pb_conv, 0);
}

int main(int argc, char* argv[])
{
    xLogInitLog(LogLevel::LL_INFO, "../logs/engine_lua_app.log");

//	sol::state lua;
//	lua.open_libraries();
//
//	// bind c++ && lua
//	luabind_netserver(lua);
//	luabind_kcpserver(lua);
//	luabind_websocket(lua);
//	luabind_eventloop(lua);
//	luabind_mysql(lua);
//	luabind_redis(lua);
//	luabind_basepacket(lua);
//	luabind_common(lua);
//	luabind_csvpar(lua);
//	luabind_json(lua);
//	luabind_httpserver(lua);
//
//	lua.script_file(argv[1]);
//	init_lua_pb(lua.lua_state());

//	if (luaL_dofile(lua.lua_state(), argv[1]) == 1) {
//		if (lua_isstring(lua.lua_state(), -1)) {
//			printf("load lua file error:%s\n", lua_tostring(lua.lua_state(), -1));
//		}
//	}

    auto p = std::make_shared<int>(1);
    INFO_LOG("HAHA ={}",*p);

//	system("pause");

//ObjPool::init<std::string>(10);
//ObjPool::init<int>(6);
//    {
//        auto at =  ObjPool::acquire<std::string>();
//        at->append("ooo");
//        INFO_LOG("string SIZE ={}", ObjPool::size<std::string>());
//        INFO_LOG("int  SIZE ={}", ObjPool::size<int>());
//    }
//    INFO_LOG(" string SIZE ={}", ObjPool::size<std::string>());
//
//    {
//        auto at =  ObjPool::create<std::string>();
//        at->append("ooo");
//        INFO_LOG("string SIZE ={}", ObjPool::size<std::string>());
//        ObjPool::release(at);
//        INFO_LOG("int  SIZE ={}", ObjPool::size<int>());
//    }
//
//    INFO_LOG(" SIZE ={}", ObjPool::size<std::string>());
//// for tetst
//
//  void * buff = new char [1024];
//
//  std::string*  pstr = new (buff) std::string("hello");
//  INFO_LOG("hha ={}", pstr->c_str());


}