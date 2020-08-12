#include"include/lua.h"
#include"include/lualib.h"
#include"include/lauxlib.h"



int New(lua_State* L){
    lua_setmetatable(L,1);
    return 1;
};

int luaopen_New(lua_State* L){
    lua_pushcfunction(L, &New);
    return 1;
}