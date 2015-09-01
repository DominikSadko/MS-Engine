#ifndef __FILESTREAM_H__
#define __FILESTREAM_H__

#include "framework/core/platform.h"
#include "framework/lua/lua.h"

struct PHYSFS_File;

class FileStream : public Lua::Shared
{
	public:
		FileStream(const std::string& path, PHYSFS_File* file);
		~FileStream();

        void seek(int64_t pos);
        int64_t tell();
        void read(char* buffer, int32_t size);
        void write(const char* buffer, int32_t size);

        int64_t size()   const { return m_size; };
        bool    empty()  const { return m_size == 0; }

    //////   get methods  //////
        char* getBuffer(int32_t size)
        {
            char* buffer = new char[size];
            read(buffer, size);
            return buffer;
        }

        std::string getFileString()
        {
            std::string buffer((size_t)m_size, 0x00);

            int64_t pos = tell();

            seek(0);
            read(&buffer[0], m_size);
            seek(pos);

            return buffer;
        }

        template <typename T>
        T get()
        {
        	if(sizeof(T) + tell() > m_size) return T();

            char buffer[sizeof(T)];
            read(buffer, sizeof(T));

            return *(T*)(buffer);
        }

        std::string getString()
        {
            std::string str;
            uint16_t len = get<uint16_t>();
            if(len > 0 && len < 8192)
            {
                char buffer[len];
                read(buffer, len);
                str = std::string(buffer, len);
            }

            return str;
        }

	//////   put methods  //////
/*
        void putU8(uint8_t v);
        void put16(uint16_t v);
        void put32(uint32_t v);
        void put64(uint64_t v);
        void putU8(int8_t v);
        void putU16(int16_t v);
        void putU32(int32_t v);
        void putU64(int64_t v);
*/
        template <typename T>
        void put(const T& value) { write((const char*)&value, sizeof(T)); }
        void putString(const std::string& value) { putString(value.c_str()); }
        void putString(const char* value)
        {
        	int32_t size = strlen(value);
        	put<uint16_t>(size);
        	write(value, size);
        }

	private:
        PHYSFS_File* m_file;
        int64_t m_size;
        std::string m_path;
};

#endif // __FILESTREAM_H__ //
