#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


#define LUA_DLL __declspec(dllexport)
#define lua_args lua_State *L

#define lua_cleanargs(num) int top = lua_gettop(L); if (top > num) lua_pop(L, top-num);


void stackDump(lua_State *L)
{
	int i;
	int top = lua_gettop(L);

	for(i = 1; i <= top; i++) {
		int t = lua_type(L, i);
		switch(t) {
			case LUA_TNIL:
				printf("nil");
				break;
			case LUA_TBOOLEAN:
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;
			case LUA_TNUMBER:
				printf("%g", lua_tonumber(L, i));
				break;
			case LUA_TSTRING:
				printf("%s", lua_tostring(L, i));
				break;

			default:
				printf("%s", lua_typename(L, t));
		}
		printf(" ");
	}
	printf("\n\n");
}