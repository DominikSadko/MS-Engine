#include "framework/image/image.h"
#include "framework/core/filemanager.h"

static uint16_t power_of_two(int input)
{
    uint16_t value = 1;
    while(value < input)
        value <<= 1;

    return value;
}

Image::Image(const std::string& path, int32_t w/* = 0*/, int32_t h/* = 0*/)
{
	FileStream* file = g_fileManager.openFile(path);
    if(!file)
        LOGE("unable to open file: %s", path.c_str());

	char* buffer = file->getBuffer(file->size());
	SDL_RWops* rw = SDL_RWFromMem(buffer, file->size());

	Surface* surface = IMG_Load_RW(rw, 0);
    if(!surface)
        LOGE("%s: %s", path.c_str(), SDL_GetError());

    load(surface, w, h);

    SDL_FreeRW(rw);
    delete[] buffer;
    delete file;
}

Image::Image(Surface* surface)
{
    load(surface, 0, 0);
}

Image::Image(const char* buffer, int32_t size)
{
	SDL_RWops* rw = SDL_RWFromConstMem(buffer, size);
	Surface* surface = IMG_Load_RW(rw, 0);

    load(surface, 0, 0);

    SDL_FreeRW(rw);
}

Image::~Image()
{
    glDeleteTextures(1, &m_texture);
}

void Image::load(Surface* surface, int32_t w, int32_t h)
{
    m_width  = w ? w : surface->w;
    m_height = h ? h : surface->h;

    // Use the surface width and height expanded to powers of 2
    m_txwidth = power_of_two(m_width);
    m_txheight = power_of_two(m_height);

    Surface* image = SDL_CreateRGBSurface(SDL_SWSURFACE, (int32_t)m_txwidth, (int32_t)m_txheight, 32,
    #if(SDL_BYTEORDER == SDL_LIL_ENDIAN)     // RGBA masks
                                 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    #else                                    // ABGR masks
                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    #endif

    if(!image)
        LOGE("%s", SDL_GetError());

    // Save the alpha blending attributes
    BlendMode saved_mode;
    SDL_GetSurfaceBlendMode(surface, &saved_mode);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

    // Copy the surface into the GL texture image
    Rect area = {0, 0, m_width, m_height};
    SDL_UpperBlit(surface, &area, image, &area);

    // Restore the alpha blending attributes
    SDL_SetSurfaceBlendMode(surface, saved_mode);

    // Create an OpenGL texture for the image
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int32_t)m_txwidth, (int32_t)m_txheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    SDL_FreeSurface(surface);    // No longer needed
    SDL_FreeSurface(image);      // No longer needed
}
