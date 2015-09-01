#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500

// include platform
#include <windows.h>

// include SDL lib
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>

// include OpenGL lib
#include <SDL2/SDL_opengl.h>



#define __CLASS_NAME__ Platform::className(__PRETTY_FUNCTION__)

#define LOG(function, type, ...) {std::cout << "[" << type << " - " << function << "] "; printf(__VA_ARGS__); std::cout << std::endl;}
#define LOGLUA(...) LOG(__FUNCTION__, "info",    __VA_ARGS__);

#define LOGI(...) ({ LOG(__CLASS_NAME__, "info",    __VA_ARGS__); })
#define LOGD(...) ({ LOG(__CLASS_NAME__, "debug",   __VA_ARGS__); })
#define LOGE(...) ({ LOG(__CLASS_NAME__, "error",   __VA_ARGS__); })
#define LOGW(...) ({ LOG(__CLASS_NAME__, "warning", __VA_ARGS__); })

class Core
{
    public:
		void init();

        std::string getPath(const std::string& path);
        void openWebsite(const std::string& url);
        std::string GetIpAddr();
        void ShowAdMob(bool, const std::string&) {};
        void HideAdMob() {};
        std::string getPackagePath() { return ""; };
        std::string getCacheDir();

    private:
        static LRESULT CALLBACK WindowsHook(int nCode, WPARAM wParam, LPARAM lParam);

    private:
        HHOOK m_windowsHook;
        static std::map<uint16_t, uint16_t> KeyMap;
};

#endif // __WINDOWS_H__ //
