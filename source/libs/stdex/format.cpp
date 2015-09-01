#include "framework/image/image.h"

#include <stdex/vertices.h>
#include <stdex/format.h>

namespace stdex
{
    bool inRange(const Point& point, const Rect& rect)
    {
        return point.x >= rect.x && point.x <= rect.x + rect.w &&
               point.y >= rect.y && point.y <= rect.y + rect.h;
    }

    bool inCircle(const Point& point, const Rect& rect)
    {
        return rect.h / 2 >= std::abs(std::sqrt(std::pow(point.x - rect.x - rect.w / 2, 2) + std::pow(point.y - rect.y - rect.h / 2, 2)));
    }

    Color toColor(int color)
    {
        if(color >= 216 || color <= 0)
            return {0xFF, 0xFF, 0xFF, 0xFF};

        uint8_t r = uint8_t(color / 36) % 6 * 51;
        uint8_t g = uint8_t(color / 6) % 6 * 51;
        uint8_t b = color % 6 * 51;
        return {r, g, b, 0xFF};
    }

    Color toColor(const char* str) { return stdex::toColor(std::string(str)); }
    Color toColor(const std::string& str)
    {
    	Color color;
    	if(str[0] == '#')
    	{
			if(str.length() == 7 || str.length() == 9)
			{
				color.r = (uint8_t)stdex::hex_to_dec(str.substr(1, 2));
				color.g = (uint8_t)stdex::hex_to_dec(str.substr(3, 2));
				color.b = (uint8_t)stdex::hex_to_dec(str.substr(5, 2));
				if(str.length() == 9)
					color.a = (uint8_t)stdex::hex_to_dec(str.substr(7, 2));
				else
					color.a = 0xFF;
			}
    	}
    	else if(str == "black")
    		return stdex::toColor("#000000");
    	else if(str == "white")
    		return stdex::toColor("#FFFFFF");
    	else if(str == "red")
    		return stdex::toColor("#FF0000");
    	else if(str == "green")
    		return stdex::toColor("#00FF00");
    	else if(str == "blue")
    		return stdex::toColor("#0000FF");

        return color;
    }

    Color toColor(const json11::Json& json)
    {
        Color color;

        const auto& items = json.array_items();
        color.r = (uint8_t)toInteger(items[0]);
        color.g = (uint8_t)toInteger(items[1]);
        color.b = (uint8_t)toInteger(items[2]);
        color.a = (uint8_t)toInteger(items[3]);

        return color;
    }

    Point toPoint(const json11::Json& json)
    {
        Point point;

        const auto& items = json.array_items();
        point.x = toInteger(items[0]);
        point.y = toInteger(items[1]);

        return point;
    }

    Rect toRect(const json11::Json& json)
    {
        Rect rect;

        const auto& items = json.array_items();
        rect.x = toInteger(items[0]);
        rect.y = toInteger(items[1]);
        rect.w = toInteger(items[2]);
        rect.h = toInteger(items[3]);

        return rect;
    }

    Rect toRectSize(const json11::Json& json)
    {
        Rect rect;

        const auto& items = json.array_items();
        rect.w = toInteger(items[0]);
        rect.h = toInteger(items[1]);

        return rect;
    }

    Anchor::Edge toEdge(const json11::Json& json)
    {
        std::string edge = stdex::toString(json);
        if(edge == "Left")
            return Anchor::Left;
        else if(edge == "Right")
            return Anchor::Right;
        else if(edge == "Top")
            return Anchor::Top;
        else if(edge == "Bottom")
            return Anchor::Bottom;
        else if(edge == "VerticalCenter")
            return Anchor::VerticalCenter;
        else if(edge == "HorizontalCenter")
            return Anchor::HorizontalCenter;
        else if(edge == "Width")
            return Anchor::Width;
        else if(edge == "Height")
            return Anchor::Height;
        else
            LOGE("unknown anchor type: %s", edge.c_str());

        return Anchor::None;
    }

    bool empty(const Point& point) { return !point.x && !point.y; }
    bool empty(const Rect& rect)  { return !rect.x && !rect.y && !rect.w && !rect.h; }
    int32_t toDistance(const Point& a, const Point& b) { return (int32_t)std::floor(std::abs(std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2)))); }
    int32_t toAngle(const Point& a, const Point& b) { return (int32_t)std::atan2(b.y - a.y, b.x - a.x) * (180.f / PI) + 180.f; }
    Point toPointByAngle(const Point& pos, int32_t angle, int32_t distance)
    {
    	angle = angle - 180;
    	return { (int32_t)std::floor(pos.x + std::cos(angle * PI / 180.f) * distance),
    		     (int32_t)std::floor(pos.y + std::cos(angle * PI / 180.f) * distance)
    	};
    }

    std::string replace(std::string str, const std::string& from, const std::string& to)
    {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }

        return str;
    }


    void Vertices::add(const Rect& rect)
    {
        add((float) rect.x);            // a
        add((float) rect.y);
        add((float) rect.x);            // b
        add((float)(rect.y + rect.h));
        add((float)(rect.x + rect.w));  // c
        add((float) rect.y);
        add((float) rect.x);            // d
        add((float)(rect.y + rect.h));
        add((float)(rect.x + rect.w));  // e
        add((float)(rect.y + rect.h));
        add((float)(rect.x + rect.w));  // f
        add((float) rect.y);

        size++;
    }

    void Vertices::add(const Image* image, const Rect& rect)
    {
        float x = ((float)(rect.x) / (float)image->getTextureWidth());
        float y = ((float)(rect.y) / (float)image->getTextureHeight());
        float w = ((float)(rect.x + rect.w) / (float)image->getTextureHeight());
        float h = ((float)(rect.y + rect.h) / (float)image->getTextureWidth());

        add(x); add(y); // a
        add(x); add(h); // b
        add(w); add(y); // c
        add(x); add(h); // d
        add(w); add(h); // e
        add(w); add(y); // f

        size++;
    }

} // stdex
