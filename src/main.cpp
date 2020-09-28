
extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <iostream>
#include "Err.hpp"
#include <csignal>

void b()
{
	STARTNA("hello")
	
	RETURNV
}

void a()
{
	START;
	b();
	RETURNV;
}

int main(int argc, char **argv)
{
	HANDLE_SIGNAL(SIGSEGV)
	START;
	
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    luaL_dofile(L,"Lua/main.lua");

    lua_close(L);
    
    a();
    RETURN(0);
}
