#include "framework/lua/lua.h"

int32_t print(lua_State* L)
{
    const char *s;
    lua_getglobal(L, "tostring");
    int n = lua_gettop(L);  // number of arguments //

    for(int32_t i = 1; i < n; i++)
    {
        lua_pushvalue(L, -1);  // function to be called //
        lua_pushvalue(L, i);   // value to print //
        lua_call(L, 1, 1);
        s = lua_tostring(L, -1);  // get result //

        if(!s)
            return luaL_error(L, "'tostring' must return a string to 'print'");

        LOGI("%s", s);
        lua_pop(L, 1);  // pop result //
    }

    lua_pop(L, 1);  // pop tostring //
    return 0;
}
