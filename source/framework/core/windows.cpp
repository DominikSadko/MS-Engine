#include "framework/core/platform.h"
#include "framework/core/filemanager.h"
#include "framework/core/enums.h"

std::map<uint16_t, uint16_t> Core::KeyMap;

std::wstring utf8_to_utf16(const std::string& src)
{
    std::wstring res;
    wchar_t out[4096];
    if(MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, out, 4096))
        res = out;
    return res;
}

void Core::init()
{
    KeyMap[VK_ESCAPE] = VK::KEY_ESCAPE;
    KeyMap[VK_CANCEL] = VK::KEY_BREAK;
    KeyMap[VK_PAUSE] = VK::KEY_PAUSE;
    KeyMap[VK_BACK] = VK::KEY_BACKSPACE;
    KeyMap[VK_TAB] = VK::KEY_TAB;
    KeyMap[VK_PRIOR] = VK::KEY_PAGEUP;
    KeyMap[VK_NEXT] = VK::KEY_PAGEDOWN;
    KeyMap[VK_END] = VK::KEY_END;
    KeyMap[VK_HOME] = VK::KEY_HOME;
    KeyMap[VK_DELETE] = VK::KEY_DELETE;

    KeyMap[VK_LEFT] = VK::KEY_LEFT;
    KeyMap[VK_UP] = VK::KEY_UP;
    KeyMap[VK_RIGHT] = VK::KEY_RIGHT;
    KeyMap[VK_DOWN] = VK::KEY_DOWN;

    KeyMap[VK_F1] = VK::KEY_F1;
    KeyMap[VK_F2] = VK::KEY_F2;
    KeyMap[VK_F3] = VK::KEY_F3;
    KeyMap[VK_F4] = VK::KEY_F4;
    KeyMap[VK_F5] = VK::KEY_F5;
    KeyMap[VK_F6] = VK::KEY_F6;
    KeyMap[VK_F7] = VK::KEY_F7;
    KeyMap[VK_F8] = VK::KEY_F8;
    KeyMap[VK_F9] = VK::KEY_F9;
    KeyMap[VK_F10] = VK::KEY_F10;
    KeyMap[VK_F11] = VK::KEY_F11;
    KeyMap[VK_F12] = VK::KEY_F12;

    KeyMap[VK_VOLUME_MUTE] = VK::KEY_VOLUME_MUTE;
    KeyMap[VK_VOLUME_DOWN] = VK::KEY_VOLUME_DOWN;
    KeyMap[VK_VOLUME_UP] = VK::KEY_VOLUME_UP;
    KeyMap[VK_MEDIA_NEXT_TRACK] = VK::KEY_MEDIA_NEXT_TRACK;
    KeyMap[VK_MEDIA_PREV_TRACK] = VK::KEY_MEDIA_PREV_TRACK;
    KeyMap[VK_MEDIA_STOP] = VK::KEY_MEDIA_STOP;
    KeyMap[VK_MEDIA_PLAY_PAUSE] = VK::KEY_MEDIA_PLAY_PAUSE;

	SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    m_windowsHook = SetWindowsHookEx(WH_KEYBOARD_LL, WindowsHook, 0, 0);
}

void Core::openWebsite(const std::string& url)
{
	std::string path = url;
    if(path.find("http://") == std::string::npos)
    	path.insert(0, "http://");

    ShellExecuteW(NULL, L"open", utf8_to_utf16(path).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

std::string Core::GetIpAddr()
{
	// todo
	return "";
}

std::string Core::getCacheDir()
{
	return g_fileManager.getBaseDir() +"cache";
}

LRESULT Core::WindowsHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	static SDL_Event event;

    if(nCode == HC_ACTION)
    {
        switch(wParam)
        {
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

				if(wParam == WM_KEYDOWN)
					event.type = SDL_KEYDOWN;
				else if(wParam == WM_KEYUP)
					event.type = SDL_KEYUP;

				uint16_t character = 0;

				auto it = Core::KeyMap.find((uint16_t)p->vkCode);
				if(it != Core::KeyMap.end())
					character = it->second;
				else
				{
					static unsigned char keyboard_state[256];
					GetKeyboardState(keyboard_state);
					ToAscii((UINT)p->vkCode, p->scanCode, keyboard_state, &character, 1);
				}

				if(character)
				{
					event.key.keysym.sym = character;
					SDL_PushEvent(&event);
				}
			}

            break;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
