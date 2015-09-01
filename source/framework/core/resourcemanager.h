#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include "framework/image/image.h"
#include "framework/font/font.h"
#include "framework/graphic/shader.h"
#include "framework/audio/audio.h"
#include "framework/core/module.h"

#include <json/json11.hpp>

class ResourceManager : public Lua::Shared
{
    public:
        ResourceManager();
        virtual ~ResourceManager() { clear(); }

        void clear();

        bool addBinary(const std::string& path);
        bool addImage(const std::string& name, const std::string& path);
        bool addFont(const std::string& name, const std::string& path, int32_t trueHeight);
        bool addShader(const std::string& name, const std::string& vertex, const std::string& fragment);
        bool addLayout(const std::string& name, const std::string& path);
        bool addModule(const std::string& name, const std::string& path);

        Image* getImage(const std::string& name);
        Font* getFont(const std::string& name);
        json11::Json* getLayout(const std::string& name);
        Shader* getShader(const std::string& name);
        //Module* getModule(const std::string& name);

        void removeImage(const std::string& name);
        void removeFont(const std::string& name);
        void removeShader(const std::string& name);
        void removeLayout(const std::string& name);
        void removeModule(const std::string& name);

    private:
        std::map<std::string, Image*> m_images;
        std::map<std::string, Font*> m_fonts;
        std::map<std::string, Shader*> m_shaders;
        std::map<std::string, json11::Json*> m_layouts;
        std::map<std::string, Module*> m_modules;
};


#endif // __RESOURCEMANAGER_H__ //
