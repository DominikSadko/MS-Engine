#ifndef __LUA_H__
#define __LUA_H__

#include <Box2D/Box2D.h>

#include <stdex/format.h>
#include <stdex/vertices.h>
#include <lua_binder/Lua.h>

void LuaRegister();

int32_t print(lua_State* L);

namespace Lua
{

template <> struct Parser<Point>
{
    static Point get(lua_State* L, int32_t idx);
    static void  put(lua_State* L, const Point& v);
    static bool  is(lua_State* L, int32_t idx);
};

template <> struct Parser<Rect>
{
    static Rect get(lua_State* L, int32_t idx);
    static void put(lua_State* L, const Rect& v);
    static bool  is(lua_State* L, int32_t idx);
};

template <> struct Parser<Color>
{
    static Color get(lua_State* L, int32_t idx);
    static void  put(lua_State* L, const Color& v);
    static bool   is(lua_State* L, int32_t idx);
};

template <> struct Parser<stdex::Vertices>
{
    static stdex::Vertices get(lua_State* L, int32_t idx);
    static void            put(lua_State* L, const stdex::Vertices& v);
    static bool             is(lua_State* L, int32_t idx);
};

template <> struct Parser<float>
{
    static float  get(lua_State* L, int32_t idx);
    static void   put(lua_State* L, const float v);
    static bool   is(lua_State* L, int32_t idx);
};

template <> struct Parser<b2Vec2>
{
    static b2Vec2 get(lua_State* L, int32_t idx);
    static void   put(lua_State* L, const b2Vec2& v);
    static bool   is(lua_State* L, int32_t idx);
};

template <> struct Parser<const b2Vec2&>
{
    static const b2Vec2 get(lua_State* L, int32_t idx);
    static void   put(lua_State* L, const b2Vec2& v);
    static bool   is(lua_State* L, int32_t idx);
};

}
extern Lua::State g_lua;

#endif // __LUA_H__ //
