#include "framework/core/platform.h"
#include <stdex/format.h>

void Core::init()
{
    m_env = (JNIEnv*)SDL_AndroidGetJNIEnv();
    m_activity = (jobject)SDL_AndroidGetActivity();
    m_class = m_env->GetObjectClass(m_activity);

    GetPackageCodePath = m_env->GetStaticMethodID(m_class, "GetPackageCodePath", "()Ljava/lang/String;");
    GetIpAddress = m_env->GetStaticMethodID(m_class, "GetIpAddress", "()Ljava/lang/String;");
    OpenWebside = m_env->GetStaticMethodID(m_class, "OpenWebside", "(Ljava/lang/String;)V");

/*
    LOGI("internal path: %s", SDL_AndroidGetInternalStoragePath());
    LOGI("external state: %i", SDL_AndroidGetExternalStorageState());
    LOGI("external path: %s", SDL_AndroidGetExternalStoragePath());*/
}

void Core::openWebsite(const std::string& url)
{
    jstring jurl = (jstring)(m_env->NewStringUTF(url.c_str()));
    m_env->CallStaticVoidMethod(m_class, OpenWebside, jurl);

    m_env->DeleteLocalRef(jurl);
}

std::string Core::GetIpAddr()
{
    jstring ret = (jstring)m_env->CallStaticObjectMethod(m_class, GetIpAddress);
	const char* str = m_env->GetStringUTFChars(ret, NULL);

    m_env->DeleteLocalRef(ret);

	return stdex::toString(str);
}

void Core::ShowAdMob(bool test, const std::string& unitId)
{
    jstring junitId = m_env->NewStringUTF(unitId.c_str());

    jmethodID jmethod = m_env->GetStaticMethodID(m_class, (test ? "showTestAdPopup" : "showAdPopup"), "(Ljava/lang/String;)V");
    m_env->CallStaticVoidMethod(m_class, jmethod, junitId);

    m_env->DeleteLocalRef(junitId);
}

void Core::HideAdMob()
{

}

std::string Core::getPackagePath()
{
    jstring ret = (jstring)m_env->CallStaticObjectMethod(m_class, GetPackageCodePath);
	const char* str = m_env->GetStringUTFChars(ret, NULL);

    m_env->DeleteLocalRef(ret);

	return stdex::toString(str);
}

std::string Core::getCacheDir()
{
    return stdex::toString(SDL_AndroidGetInternalStoragePath());
}

/*
jmethodID showTextInput = m_env->GetStaticMethodID(m_class, "showTextInput", "(IIII)Z");
m_env->CallStaticBooleanMethod(m_class, showTextInput, 0, 0, 100, 100);
*/
