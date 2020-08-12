#include"include/lua.h"
#include"include/lualib.h"
#include"include/lauxlib.h"



#define PtC_Collision(a, b) ( ((a[0] - b[0])*(a[0] - b[0]) + (a[1] - b[1])*(a[1] - b[1])) <= b[2]*b[2] )

#define PtS_Collision(a, b) ( (a[0] >= b[0] || a[0] <= b[0] + b[2] )&&(a[1] >= b[1] || a[1] <= b[1] + b[4]) )

#define CtC_Collision(a, b) ( ((a[0] - b[0])*(a[0] - b[0]) + (a[1] - b[1])*(a[1] - b[1])) <= b[2]*b[2] + a[2]*a[2] )



int CC_Collide(lua_State *L){
    lua_Number a[3], b[3];
    

    a[0] = lua_tonumber(L, 1);
    a[1] = lua_tonumber(L, 2);
    a[2] = lua_tonumber(L, 3);

    b[0] = lua_tonumber(L, 4);
    b[1] = lua_tonumber(L, 5);
    b[2] = lua_tonumber(L, 6);

    lua_pushboolean(L, CtC_Collision(a, b));
    return 1;
}

int PS_Collide(lua_State* L){
    lua_Number a[4], b[4];

    a[0] = lua_tonumber(L, 1);
    a[1] = lua_tonumber(L, 2);
    
    
    b[0] = lua_tonumber(L, 5);
    b[1] = lua_tonumber(L, 6);
    b[2] = lua_tonumber(L, 7);
    b[3] = lua_tonumber(L, 8);

    lua_pushboolean(L, PtS_Collision(a, b));
    return 1;
};

int PC_Collide(lua_State* L){
    lua_Number a[4], b[4];

    a[0] = lua_tonumber(L, 1);
    a[1] = lua_tonumber(L, 2);
    
    
    b[0] = lua_tonumber(L, 5);
    b[1] = lua_tonumber(L, 6);
    b[2] = lua_tonumber(L, 7);
    

    lua_pushboolean(L, PtC_Collision(a, b));
    return 1;
};

static const struct luaL_Reg funcs[] = {
   
    {"CtC", &CC_Collide},
    {"PtS", &PS_Collide},
    {"PtC", &PC_Collide},
    {NULL, NULL}
};

int __declspec(dllexport) luaopen_Collision(lua_State* L){
    luaL_newlibtable(L, funcs);
    luaL_setfuncs(L, funcs, 0);
    
    return 1;
};