#ifndef __FONT_H__
#define __FONT_H__

#include "framework/graphic/painter.h"
#include "framework/image/image.h"

class Font;
class TextCache
{
    friend class Font;

    public:
        TextCache()
        {
            m_vertices.clear();
            m_texVertices.clear();
            m_width = 0;
        }

        ~TextCache() { clear(); }

        void clear()
        {
            m_vertices.clear();
            m_texVertices.clear();
        }

        void setText(const Font* font, const std::string& text);
        bool empty() const { return m_vertices.empty(); }
        uint16_t getDrawWidth() const { return m_width; }

    private:
        uint16_t m_width;
        stdex::Vertices m_vertices;
        stdex::Vertices m_texVertices;
};

class Font : public Lua::Shared
{
    public:
        Font(const std::string& file, int32_t spaceSize);
        virtual ~Font();

        void draw(const TextCache* textCache);
        void drawGlyph(char glyph, const Point& point) const;
        Rect getTextSize(const std::string& text) const;
        void doUpdateTextVertices(TextCache* textCache, const std::string& text) const;

        void setMask(Image* image) { m_mask = image; }
        Image* getMask() const { return m_mask; }

    private:
        void getGlyphSize(const Surface* surface, uint16_t id, const Rect& rect);

    private:
        Image* m_image;
        Image* m_mask;
        uint8_t m_glyphSizes[0xFF];
        int32_t m_glyphSize;
};


#endif // __FONT_H__ //
