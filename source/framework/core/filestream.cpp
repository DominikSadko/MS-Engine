#include "framework/core/filestream.h"
#include <physfs.h>

FileStream::FileStream(const std::string& path, PHYSFS_File* file)
{
	m_file = file;
	m_size = PHYSFS_fileLength(m_file);
	m_path = path;
}

FileStream::~FileStream()
{
	if(!PHYSFS_close(m_file))
        LOGE("failed to close file: %s", m_path.c_str());
}

void FileStream::seek(int64_t pos)
{
   	PHYSFS_seek(m_file, pos);
}

int64_t FileStream::tell()
{
   	return PHYSFS_tell(m_file);
}

void FileStream::read(char* buffer, int32_t size)
{
	PHYSFS_read(m_file, buffer, size,  1);
}

void FileStream::write(const char* buffer, int32_t size)
{
	PHYSFS_write(m_file, buffer, size,  1);
}
/*
void FileStream::putU8(uint8_t v)
{
	if(PHYSFS_write(m_file, &v, 1, 1) == -1)
		LOGE("Unable to put.");
}

void FileStream::put16(uint16_t v)
{
	if(!PHYSFS_writeSLE16(m_file, v))
		LOGE("Unable to put.");
}

void FileStream::put32(uint32_t v)
{
	if(!PHYSFS_writeSLE32(m_file, v))
		LOGE("Unable to put.");
}

void FileStream::put64(uint64_t v)
{
	if(!PHYSFS_writeSLE64(m_file, v))
		LOGE("Unable to put.");
}

void FileStream::putU8(int8_t v)
{
	if(PHYSFS_write(m_file, &v, 1, 1) == -1)
		LOGE("Unable to put.");
}

void FileStream::putU16(int16_t v)
{
	if(!PHYSFS_writeULE16(m_file, v))
		LOGE("Unable to put.");
}

void FileStream::putU32(int32_t v)
{
	if(!PHYSFS_writeULE32(m_file, v))
		LOGE("Unable to put.");
}

void FileStream::putU64(int64_t v)
{
	if(!PHYSFS_writeULE64(m_file, v))
		LOGE("Unable to put.");
}
*/
