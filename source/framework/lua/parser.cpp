#include "framework/core/platform.h"
#include "framework/lua/lua.h"

namespace Lua
{

Color Parser<Color>::get(lua_State* L, int32_t idx)
{
    Color color;
    Lua::Object object(L, idx);

    color.r = object["r"].get<int32_t>();
    color.g = object["g"].get<int32_t>();
    color.b = object["b"].get<int32_t>();
    color.a = object["a"].get<int32_t>();
    return color;
}

void Parser<Color>::put(lua_State* L, const Color& color)
{
    std::map<std::string, uint8_t> rectMap;
    rectMap["r"] = color.r;
    rectMap["g"] = color.g;
    rectMap["b"] = color.b;
    rectMap["a"] = color.a;

    Lua::Parser<std::map<std::string, uint8_t>>::put(L, rectMap);
}

bool Parser<Color>::is(lua_State* L, int32_t idx)  { return lua_istable(L, idx); }


Point Parser<Point>::get(lua_State* L, int32_t idx)
{
    Point point;
    Lua::Object object(L, idx);

    point.x = object["x"].get<int32_t>();
    point.y = object["y"].get<int32_t>();
    return point;
}

void Parser<Point>::put(lua_State* L, const Point& point)
{
    std::map<std::string, int32_t> pointMap;
    pointMap["x"] = point.x;
    pointMap["y"] = point.y;

    Lua::Parser<std::map<std::string, int32_t>>::put(L, pointMap);
}

bool Parser<Point>::is(lua_State* L, int32_t idx)  { return lua_istable(L, idx); }


Rect Parser<Rect>::get(lua_State* L, int32_t idx)
{
    Rect rect;
    Lua::Object object(L, idx);

    rect.x = object["x"].get<int32_t>();
    rect.y = object["y"].get<int32_t>();
    rect.w = object["w"].get<int32_t>();
    rect.h = object["h"].get<int32_t>();
    return rect;
}

void Parser<Rect>::put(lua_State* L, const Rect& rect)
{
    std::map<std::string, int32_t> rectMap;
    rectMap["x"] = rect.x;
    rectMap["y"] = rect.y;
    rectMap["w"] = rect.w;
    rectMap["h"] = rect.h;

    Lua::Parser<std::map<std::string, int32_t>>::put(L, rectMap);
}

bool Parser<Rect>::is(lua_State* L, int32_t idx) { return lua_istable(L, idx); }

stdex::Vertices Parser<stdex::Vertices>::get(lua_State* L, int32_t idx)
{
	stdex::Vertices vecrtices;
    lua_pushnil(L);

    while(lua_next(L, idx - 1))
    {
        lua_pushvalue(L, -2);

        if(Parser<int32_t>::is(L, -1) && Parser<int32_t>::is(L, -2))
        	vecrtices.data.push_back(Parser<int32_t>::get(L, -2));

        Lua::pop(L, 2);
    }

    vecrtices.size = vecrtices.data.size() / 2;
    return vecrtices;
}

void Parser<stdex::Vertices>::put(lua_State* L, const stdex::Vertices& vertices)
{
	std::vector<int32_t> vec;
	for(auto i : vertices.data)
		vec.push_back((int32_t)i);

    Lua::Parser<std::vector<int32_t>>::put(L, vec);
}

bool Parser<stdex::Vertices>::is(lua_State* L, int32_t idx)  { return lua_istable(L, idx); }
}

