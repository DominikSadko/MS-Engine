#include "framework/core/module.h"
#include "framework/lua/lua.h"
#include "framework/core/filemanager.h"

Module::Module(const std::string& name, const std::string& path)
{
    m_name = name;

    if(!g_lua.doString(name, g_fileManager.readFileString(path).c_str()))
        LOGE("Unable to load module: %s", m_name.c_str());

    if(!g_lua[m_name]["init"].safeCall())
        LOGE("Unable to call function init: %s", m_name.c_str());
}

Module::~Module()
{
    if(!g_lua[m_name]["terminate"].safeCall())
        LOGE("Unable to call function init: %s", m_name.c_str());

    g_lua.remove(m_name);
}
