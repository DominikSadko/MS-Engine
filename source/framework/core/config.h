#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "framework/core/platform.h"
#include "framework/lua/lua.h"

class Config : public Lua::Shared
{
	public:
		Config(const std::string& fileName);
		~Config();

		void load();
		void save();

		void set(const std::string& key, const std::string& value);
		std::string get(const std::string& key);
		void erase(const std::string& key);
		void clear();

	private:
		std::string m_fileName;
		std::map<std::string, std::string> m_configs;
};

#endif // __CONFIG_H__ //
