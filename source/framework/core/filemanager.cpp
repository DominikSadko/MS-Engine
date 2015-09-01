#include "framework/core/filemanager.h"
#include <physfs.h>

FileManager g_fileManager;

FileManager::FileManager()
{

}

FileManager::~FileManager()
{

}

void FileManager::init()
{
    PHYSFS_init(NULL);
    PHYSFS_permitSymbolicLinks(1);
}

void FileManager::terminate()
{
    PHYSFS_deinit();
}

std::string FileManager::getUserDir()
{
    return resolvePath(PHYSFS_getUserDir());
}

std::string FileManager::getBaseDir()
{
    return resolvePath(PHYSFS_getBaseDir());
}

std::string FileManager::getWriteDir()
{
    return resolvePath(PHYSFS_getWriteDir());
}

bool FileManager::addSearchPath(const std::string& path)
{
    if(!path.empty() && !PHYSFS_addToSearchPath(resolvePath(path).c_str(), 1))
    {
    	LOGE("%s : %s", PHYSFS_getLastError(), path.c_str());
    	return false;
    }

    return true;
}

bool FileManager::removeSearchPath(const std::string& path)
{
    if(!path.empty() && !PHYSFS_removeFromSearchPath(resolvePath(path).c_str()))
    {
    	LOGE("%s : %s", PHYSFS_getLastError(), path.c_str());
    	return false;
    }

    return true;
}

bool FileManager::setWriteDir(const std::string& path)
{
    if(!path.empty() && !PHYSFS_setWriteDir(resolvePath(path).c_str()))
    {
    	LOGE("%s : %s", PHYSFS_getLastError(), path.c_str());
    	return false;
    }

    return true;
}

bool FileManager::fileExists(const std::string& path)
{
    return PHYSFS_exists(resolvePath(path).c_str()) && !PHYSFS_isDirectory(resolvePath(path).c_str());
}

bool FileManager::directoryExists(const std::string& path)
{
    return PHYSFS_isDirectory(resolvePath(path).c_str());
}

std::string FileManager::resolvePath(const std::string& path)
{
	std::string fullPath = stdex::replace(path, "\\", "/");
	fullPath = stdex::replace(fullPath, "//", "/");
    return fullPath;
}

std::string FileManager::getExtension(const std::string& path)
{
	return path.substr(path.rfind(".") + 1);
}

std::string FileManager::getFileName(const std::string& path)
{
	std::string fullPath = resolvePath(path);
	return fullPath.erase(0, fullPath.rfind("/") + 1);
}

std::list<std::string> FileManager::getDirectoryFiles(const std::string& path)
{
    std::list<std::string> files;
    auto list = PHYSFS_enumerateFiles(resolvePath(path).c_str());
    for(int i = 0; list[i] != NULL; i++)
        files.push_back(resolvePath(list[i]));

    PHYSFS_freeList(list);
    return files;
}

bool FileManager::makeDirectory(const std::string path)
{
    return PHYSFS_mkdir(resolvePath(path).c_str());
}

bool FileManager::deleteFile(const std::string& path)
{
	return PHYSFS_delete(resolvePath(path).c_str());
}

std::string FileManager::readFileString(const std::string& path)
{
	std::string fullPath = resolvePath(path);

	PHYSFS_File* file = PHYSFS_openRead(path.c_str());
	if(!file)
	{
    	LOGE("%s : %s", PHYSFS_getLastError(), path.c_str());
		return "";
	}

	int32_t size = PHYSFS_fileLength(file);
	std::string buffer(size, 0);
	PHYSFS_read(file, (void*)&buffer[0], 1, size);
	PHYSFS_close(file);

	return buffer;
}

bool FileManager::writeFileBuffer(const std::string& path, const char* data, uint32_t size)
{
	std::string fullPath = resolvePath(path);

    PHYSFS_file* file = PHYSFS_openWrite(fullPath.c_str());
    if(!file)
    {
    	LOGE("%s : %s", PHYSFS_getLastError(), path.c_str());
        return false;
    }

    PHYSFS_write(file, (void*)data, size, 1);
    PHYSFS_close(file);
    return true;
}

bool FileManager::writeFileString(const std::string& path, const std::string& data)
{
    return writeFileBuffer(resolvePath(path), data.c_str(), data.size());
}

FileStream* FileManager::openFile(const std::string& path)
{
    std::string fullPath = resolvePath(path);

    PHYSFS_File* file = PHYSFS_openRead(fullPath.c_str());
    if(!file)
    {
    	LOGE("%s : %s", PHYSFS_getLastError(), path.c_str());
        return nullptr;
    }

    return new FileStream(fullPath, file);
}

FileStream* FileManager::appendFile(const std::string& path)
{
    std::string fullPath = resolvePath(path);

    PHYSFS_File* file = PHYSFS_openAppend(fullPath.c_str());
    if(!file)
    {
    	LOGE("%s : %s", PHYSFS_getLastError(), path.c_str());
        return nullptr;
    }

    return new FileStream(fullPath, file);
}

FileStream* FileManager::writeFile(const std::string& path)
{
    std::string fullPath = resolvePath(path);

    PHYSFS_File* file = PHYSFS_openWrite(fullPath.c_str());
    if(!file)
    {
    	LOGE("%s : %s", PHYSFS_getLastError(), path.c_str());
        return nullptr;
    }

    return new FileStream(fullPath, file);
}
