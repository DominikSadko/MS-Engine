#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include "framework/core/platform.h"
#include "framework/lua/lua.h"
#include "framework/core/filestream.h"

class FileManager : public Lua::Shared
{
	public:
		FileManager();
		~FileManager();

		void init();
		void terminate();

		std::string getUserDir();
		std::string getBaseDir();
		std::string getWriteDir();

		bool addSearchPath(const std::string& path);
		bool removeSearchPath(const std::string& path);
		bool setWriteDir(const std::string& path);
		bool fileExists(const std::string& path);
		bool directoryExists(const std::string& path);
		std::string resolvePath(const std::string& path);
		std::string getExtension(const std::string& path);
		std::string getFileName(const std::string& path);
		std::list<std::string> getDirectoryFiles(const std::string& path);

		bool makeDirectory(const std::string path);
		bool deleteFile(const std::string& path);
		std::string readFileString(const std::string& path);
		bool writeFileBuffer(const std::string& path, const char* data, uint32_t size);
		bool writeFileString(const std::string& path, const std::string& data);
		FileStream* openFile(const std::string& path);
		FileStream* appendFile(const std::string& path);
		FileStream* writeFile(const std::string& path);

};
extern FileManager g_fileManager;


#endif // __FILEMANAGER_H__ //
