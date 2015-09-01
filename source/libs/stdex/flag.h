#ifndef __FLAG_H__
#define __FLAG_H__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

namespace stdex
{
template<typename T>
struct flag
{
    void set(T key, bool value)
    {
        if(value)
            data |=  key;
        else
        	data &= ~key;
    };

    bool has(T key)       const { return data & key; };

    private:
        T data;
};
} // stdex

#endif // __FLAG_H__ //
