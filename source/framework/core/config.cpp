#include "framework/core/config.h"
#include "framework/core/filemanager.h"

Config::Config(const std::string& fileName)
{
	m_fileName = fileName;
}

Config::~Config()
{
}

void Config::load()
{
	if(!g_fileManager.fileExists(m_fileName))
		return;

	FileStream* file = g_fileManager.openFile(m_fileName);
	if(!file)
		return;

	uint32_t size = file->get<uint32_t>();

	std::string key, value;
	for(uint32_t i = 0; i < size; i++)
	{
		key = file->getString();
		value = file->getString();

		set(key, value);
	}

	delete file;
}

void Config::save()
{
	FileStream* file = g_fileManager.writeFile(m_fileName);
	if(!file)
		return;

	file->put<uint32_t>(m_configs.size());

	for(const auto& it : m_configs)
	{
		file->putString(it.first);
		file->putString(it.second);
	}
}

void Config::set(const std::string& key, const std::string& value)
{
	m_configs[key] = value;
}

std::string Config::get(const std::string& key)
{
	auto it = m_configs.find(key);
	if(it == m_configs.end())
		return "";

	return it->second;
}

void Config::erase(const std::string& key)
{
	auto it = m_configs.find(key);
	if(it != m_configs.end())
		m_configs.erase(it);
}

void Config::clear()
{
	m_configs.clear();
}
