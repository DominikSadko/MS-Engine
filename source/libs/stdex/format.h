#ifndef __FORMAT_H__
#define __FORMAT_H__

#include "framework/core/platform.h"
#include "framework/ui/anchor.h"

#include <json/json11.hpp>

#define PI 3.1415926535897932384626433832795

namespace stdex
{
// toString
inline std::string toString(const json11::Json& json)        { return json.string_value(); }
template<typename T>
inline std::string toString(T value)                         { std::stringstream ss; ss << value; return ss.str(); }
template<typename T, typename... Args>
inline std::string toString(const T& v, const Args&... args) { std::string str = stdex::toString(v); return str + stdex::toString(args...); }

                     inline bool    toBoolean(const json11::Json& json)  { return json.bool_value(); }
                     inline int64_t toInteger(const json11::Json& json)  { return json.int_value(); }
template<typename T> inline int64_t toInteger(const T& v)                { std::string str = stdex::toString(v); return atol(str.c_str()); }
                     inline double  toNumber(const json11::Json& json)   { return json.number_value(); }
template<typename T> inline double  toNumber(const T& v)                 { std::string str = stdex::toString(v); return atof(str.c_str()); }

bool  inRange(const Point&, const Rect& rect);
bool  inCircle(const Point& point, const Rect& rect);
Color toColor(const char* str);
Color toColor(const std::string& str);
Color toColor(int color);

Color toColor(const json11::Json& json);
Point toPoint(const json11::Json& json);
Rect  toRect(const json11::Json& json);
Rect  toRectSize(const json11::Json& json);
Anchor::Edge toEdge(const json11::Json& json);

bool empty(const Point& point);
bool empty(const Rect& rect);
int32_t toDistance(const Point& a, const Point& b);
int32_t toAngle(const Point& a, const Point& b);
Point toPointByAngle(const Point& a, int32_t angle, int32_t distance);

inline uint64_t hex_to_dec(const std::string& str)
{
    uint64_t num;
    std::istringstream i(str);
    i >> std::hex >> num;
    return num;
}

std::string replace(std::string str, const std::string& from, const std::string& to);

} // stdex

#endif // __FORMAT_H__ //
