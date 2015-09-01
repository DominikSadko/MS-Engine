#include "framework/font/font.h"
#include "framework/core/filemanager.h"

#define GLYPHS_COUNT 16

Font::Font(const std::string& path, int32_t spaceSize)
{
	FileStream* file = g_fileManager.openFile(path);
    if(!file)
        LOGE("unable to open file: %s", path.c_str());

	char* buffer = file->getBuffer(file->size());
	SDL_RWops* rw = SDL_RWFromMem(buffer, file->size());

	Surface* surface = IMG_Load_RW(rw, 0);
    if(!surface)
        LOGE("%s\n%s", path.c_str(), SDL_GetError());

    m_glyphSize = surface->w / GLYPHS_COUNT;

    uint16_t id = 0;
    for(int32_t y = 0; y < GLYPHS_COUNT; y++)
    {
        for(int32_t x = 0; x < GLYPHS_COUNT; x++)
            getGlyphSize(surface, id++, {x * m_glyphSize, y * m_glyphSize, m_glyphSize, m_glyphSize});
    }

    m_glyphSizes[0] = (uint8_t)(spaceSize / 2.5f);
    m_image = new Image(surface);
    m_mask = nullptr;

    SDL_FreeRW(rw);
    delete[] buffer;
    delete file;
}

Font::~Font()
{
    delete m_image;
}

void Font::draw(const TextCache* textCache)
{
    g_painter.setImage(m_image);
    if(m_mask)
    	g_painter.setImage(m_mask, 1);

    g_painter.setVertex(textCache->m_vertices);
    g_painter.setCoord(textCache->m_texVertices);

    g_painter.draw();
}

void Font::drawGlyph(char glyph, const Point& point) const
{
    g_painter.setImage(m_image);
    if(m_mask)
    	g_painter.setImage(m_mask, 1);

    g_painter.setVertex({point.x, point.y, m_glyphSize, m_glyphSize});
    g_painter.setCoord(m_image, {(glyph - 32) * m_glyphSize, (glyph - 32) / GLYPHS_COUNT * m_glyphSize,
                                 m_glyphSize, m_glyphSize});

    g_painter.draw();
}

void Font::getGlyphSize(const Surface* surface, uint16_t id, const Rect& rect)
{
    uint8_t size = 0;
    uint8_t* pixels = (uint8_t*)surface->pixels;

    for(int32_t y = rect.y; y < rect.y + rect.h; y++)
    {
        for(int32_t x = rect.x; x < rect.x + rect.w; x++)
        {
            if((uint8_t)pixels[3 + ((y * (surface->w * 4)) + (x * 4))] > 0 && x - rect.x > size)
                size = x - rect.x;
        }
    }

    m_glyphSizes[id] = size + 1;
}

Rect Font::getTextSize(const std::string& text) const
{
    Rect rect = {0, 0, 0, m_glyphSize};
    for(auto c : text)
        rect.w += m_glyphSizes[c - 32];

    return rect;
}

void Font::doUpdateTextVertices(TextCache* textCache, const std::string& text) const
{
    Rect rect = {0, 0, m_glyphSize, m_glyphSize};
    Rect imageRect = {0, 0, m_glyphSize, m_glyphSize};

    textCache->clear();

    for(auto c : text)
    {
        imageRect.x = (c - 32) * m_glyphSize;
        imageRect.y = (c - 32) / GLYPHS_COUNT * m_glyphSize;

        textCache->m_vertices.add(rect);
        textCache->m_texVertices.add(m_image, imageRect);

        rect.x += m_glyphSizes[c - 32];
    }
}

void TextCache::setText(const Font* font, const std::string& text)
{
    font->doUpdateTextVertices(this, text);
    m_width = font->getTextSize(text).w;
}
