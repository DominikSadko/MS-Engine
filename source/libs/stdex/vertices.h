#ifndef __VERTICES_H__
#define __VERTICES_H__

class Image;
namespace stdex
{
struct Vertices
{
	Vertices() { size = 0; }

    void add(float value) { data.push_back(value); }
    void add(const Rect& rect);
    void add(const Image* image, const Rect& rect);

    void clear()  { size = 0; data.clear(); }
    bool empty() const { return data.empty(); }

    int32_t size;
    std::vector<float> data;
};
} // stdex

#endif // __VERTICES_H__ //
