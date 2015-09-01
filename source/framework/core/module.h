#ifndef __MODULE_H__
#define __MODULE_H__

#include "framework/core/platform.h"

class Module
{
    public:
        Module(const std::string& name, const std::string& path);
        ~Module();

    private:
        std::string m_name;
};



#endif // __MODULE_H__ //
