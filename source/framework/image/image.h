#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "framework/core/platform.h"
#include "framework/lua/lua.h"

class Image : public Lua::Shared
{
    public:
        Image(const std::string& file, int32_t w = 0, int32_t h = 0);
        Image(Surface* surface);
        Image(const char* buffer, int32_t size);
        virtual ~Image();

        Rect     getRect()           const  { return {0, 0, m_width, m_height}; }
        uint32_t getTexture()        const  { return m_texture; }
        uint16_t getWidth()          const  { return m_width; }
        uint16_t getHeight()         const  { return m_height; }
        uint16_t getTextureWidth()   const  { return m_txwidth; }
        uint16_t getTextureHeight()  const  { return m_txheight; }

    private:
        void load(Surface* surface, int32_t w, int32_t h);

    private:
        uint32_t m_texture;
        uint16_t m_width, m_height, m_txwidth, m_txheight;
};


#endif // __IMAGE_H__ //
