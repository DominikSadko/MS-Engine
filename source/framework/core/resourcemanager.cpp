#include "framework/core/resourcemanager.h"
#include "framework/core/filemanager.h"

ResourceManager::ResourceManager()
{
}

void ResourceManager::clear()
{
    for(auto it = m_modules.rbegin(); it != m_modules.rend(); it++)
        delete it->second;
    m_modules.clear();

    for(auto it : m_images)
        delete it.second;
    m_images.clear();

    for(auto it : m_fonts)
        delete it.second;
    m_fonts.clear();

    for(auto it : m_shaders)
        delete it.second;
    m_shaders.clear();

    for(auto it : m_layouts)
        delete it.second;
    m_layouts.clear();
}

bool ResourceManager::addBinary(const std::string& path)
{
	/*File file;
	if(!file.open(path))
	{
		LOGE("unable to load file: %s", path.c_str());
		return false;
	}

    uint8_t type = file.get<uint8_t>();
    while(type != 0xFF)
    {
    	switch(type)
    	{
			case 0x01: // Image Type
			{
				std::string name = file.get<std::string>();
				int32_t size = file.get<int32_t>();
				char* buffer = file.getBuffer(size);

			    Image* image = new Image(buffer, size);
			    if(!image)
			        return false;

			    m_images.insert(std::pair<std::string, Image*>(name, image));

			    delete buffer;
				break;
			}

			default:
				break;
    	}

        type = file.get<uint8_t>();
    }

    file.close();*/
    return true;
}

bool ResourceManager::addImage(const std::string& name, const std::string& path)
{
    Image* image = new Image(path);
    if(!image)
        return false;

    m_images.insert(std::pair<std::string, Image*>(name, image));
    return true;
}

bool ResourceManager::addFont(const std::string& name, const std::string& path, int32_t trueHeight)
{
    Font* font = new Font(path, trueHeight);
    if(!font)
        return false;

    m_fonts.insert(std::pair<std::string, Font*>(name, font));
    return true;
}

bool ResourceManager::addShader(const std::string& name, const std::string& vertex, const std::string& fragment)
{
    Shader* shader = new Shader(vertex, fragment);
    if(!shader)
        return false;

    m_shaders.insert(std::pair<std::string, Shader*>(name, shader));
    return true;
}

bool ResourceManager::addLayout(const std::string& name, const std::string& path)
{
    std::string js = g_fileManager.readFileString(path.c_str());

    std::string err;
    json11::Json* layout = new json11::Json();
    json11::Json::parse(*layout, js.c_str(), err);

    if(!err.empty())
    {
        LOGE("Name: %s | Error %s", name.c_str(), err.c_str());
        delete layout;
        return false;
    }

    m_layouts.insert(std::pair<std::string, json11::Json*>(name, layout));
    return true;
}

bool ResourceManager::addModule(const std::string& name, const std::string& path)
{
    Module* module = new Module(name, path);
    if(!module)
        return false;

    m_modules.insert(std::pair<std::string, Module*>(name, module));
    return true;
}

Image* ResourceManager::getImage(const std::string& name)
{
    for(auto it : m_images)
    {
        if(it.first == name)
            return it.second;
    }

    LOGE("not found: %s", name.c_str());
    return nullptr;
}

Font* ResourceManager::getFont(const std::string& name)
{
    for(auto it : m_fonts)
    {
        if(it.first == name)
            return it.second;
    }

    LOGE("not found: %s", name.c_str());
    return nullptr;
}

Shader* ResourceManager::getShader(const std::string& name)
{
    for(auto it : m_shaders)
    {
        if(it.first == name)
            return it.second;
    }

    LOGE("not found: %s", name.c_str());
    return nullptr;
}

json11::Json* ResourceManager::getLayout(const std::string& name)
{
    for(auto it : m_layouts)
    {
        if(it.first == name)
            return it.second;
    }

    LOGE("not found: %s", name.c_str());
    return nullptr;
}

void ResourceManager::removeImage(const std::string& name)
{
    for(auto it = m_images.begin(); it != m_images.end(); it++)
    {
        if(it->first == name)
        {
            delete it->second;
            m_images.erase(it);
            return;
        }
    }

    LOGE("not found: %s", name.c_str());
}

void ResourceManager::removeFont(const std::string& name)
{
    for(auto it = m_fonts.begin(); it != m_fonts.end(); it++)
    {
        if(it->first == name)
        {
            delete it->second;
            m_fonts.erase(it);
            return;
        }
    }

    LOGE("not found: %s", name.c_str());
}

void ResourceManager::removeShader(const std::string& name)
{
    for(auto it = m_shaders.begin(); it != m_shaders.end(); it++)
    {
        if(it->first == name)
        {
            delete it->second;
            m_shaders.erase(it);
            return;
        }
    }

    LOGE("not found: %s", name.c_str());
}

void ResourceManager::removeLayout(const std::string& name)
{
    for(auto it = m_layouts.begin(); it != m_layouts.end(); it++)
    {
        if(it->first == name)
        {
            delete it->second;
            m_layouts.erase(it);
            return;
        }
    }

    LOGE("not found: %s", name.c_str());
}

void ResourceManager::removeModule(const std::string& name)
{
    for(auto it = m_modules.begin(); it != m_modules.end(); it++)
    {
        if(it->first == name)
        {
            delete it->second;
            m_modules.erase(it);
            return;
        }
    }

    LOGE("not found: %s", name.c_str());
}
