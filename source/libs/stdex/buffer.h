#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "framework/core/platform.h"

namespace stdex
{
class buffer
{
    public:
        buffer(int32_t size)
        {
            m_buffer = new char[size];
            m_size = size;
            m_position = 0;
        }

        ~buffer()
        {
            delete[] m_buffer;
        }

        char*   getBuffer()   const { return m_buffer; }
        int32_t getSize()     const { return m_size; }
        int32_t getPosition() const { return m_position; }

        void setPosition(int32_t pos) { m_position = pos; }


        /////////////////       get value       /////////////////

	public:
        std::string getString(bool peek = false)
        {
        	int32_t size = get<uint16_t>(peek);
        	char* v = (char*)(m_buffer + m_position + sizeof(uint16_t));
        	if(!peek)
        		m_position += size + sizeof(uint16_t);

        	return std::string(v, size);
        }

        template <typename T>
        T get(bool peek = false)
        {
			T value = *(T*)(m_buffer + m_position);
        	if(!peek)
        		m_position += sizeof(T);

			return value;
        }

        /////////////////       put value       /////////////////

	public:
		template<typename T>
		void put(T value)
		{
			*(T*)(m_buffer + m_position) = value;
			m_position += sizeof(T);
			m_size += sizeof(T);
		}

		void putString(const char* value)
		{
			uint32_t size = (uint32_t)strlen(value);
			put<uint16_t>(size);
			strcpy((char*)(m_buffer + m_position), value);

			m_position += size;
			m_size += size;
		}

		void putString(const std::string& value) { put(value.c_str()); }

    private:
        char* m_buffer;
        int32_t m_size, m_position;
};
} // stdex

typedef stdex::buffer Buffer;

#endif // __BUFFER_H__ //
