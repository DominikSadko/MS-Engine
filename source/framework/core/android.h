#ifndef __ADNROID_H__
#define __ADNROID_H__

// include platform
#include <android/log.h>
#include <jni.h>

// include SDL lib
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_thread.h>
#include <SDL_timer.h>
#include <SDL_net.h>

// include OpenGLES lib
#include <SDL_opengles.h>
#include <SDL_opengles2.h>


#define LOG_NAME(name) ("native-activity - " + name)


#define __CLASS_NAME__ Platform::className(__PRETTY_FUNCTION__)

#define LOG_THROW(...) ((void)__android_log_print(ANDROID_LOG_INFO,  "native-activity", __VA_ARGS__))
#define LOG(function, type, ...) ((void)__android_log_print(ANDROID_LOG_INFO,  function.c_str(), __VA_ARGS__))

#define LOGI(...) ({ LOG(LOG_NAME(__CLASS_NAME__), ANDROID_LOG_INFO,    __VA_ARGS__); })
#define LOGD(...) ({ LOG(LOG_NAME(__CLASS_NAME__), ANDROID_LOG_DEBUG,   __VA_ARGS__); })
#define LOGE(...) ({ LOG(LOG_NAME(__CLASS_NAME__), ANDROID_LOG_ERROR,   __VA_ARGS__); })
#define LOGW(...) ({ LOG(LOG_NAME(__CLASS_NAME__), ANDROID_LOG_WARNING, __VA_ARGS__); })

class Core
{
    public:
        void init();

        std::string getPath(const std::string& path);
        void openWebsite(const std::string& url);
        std::string GetIpAddr();
        void ShowAdMob(bool test, const std::string& unitId);
        void HideAdMob();
        std::string getPackagePath();
        std::string getCacheDir();

    private:
        JNIEnv* m_env;
        jobject m_activity;
        jclass m_class;

        jmethodID GetPackageCodePath,
                  GetIpAddress,
                  OpenWebside;
};

#endif // __ADNROID_H__
