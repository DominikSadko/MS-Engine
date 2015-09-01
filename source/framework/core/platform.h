#ifndef __PLATFORM_H__
#define __PLATFORM_H__
#include <stdlib.h>
#include <stdio.h>

#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <cstring>
#include <math.h>
#include "time.h"

#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <tuple>

#include <functional>

#include <stdex/flag.h>

#if defined(WIN32)
    #include "framework/core/windows.h"
#elif defined(ANDROID)
    #include "framework/core/android.h"
#endif

#define Window SDL_Window
#define Renderer SDL_Renderer
#define Surface SDL_Surface
#define Texture SDL_Texture

#define Context SDL_GLContext

#define Color SDL_Color
#define Point SDL_Point
#define Rect SDL_Rect

#define Event SDL_Event
#define DisplayMode SDL_DisplayMode
#define BlendMode SDL_BlendMode

#define getTicks SDL_GetTicks

class UIWidget;
class Platform
{
    public:
        static std::string className(std::string prettyFunction);
        static std::string getPath(const std::string& path);
        static void openWebsite(const std::string url);
        static std::string GetIpAddr();
        static void ShowAdMob(bool test, const std::string& unitId);
        static void HideAdMob();
        static std::string getPackagePath();
        static std::string getCacheDir();

        void init();
        void terminate();
        void loop();
        void render();

        bool hasWindowFocus() const { return m_hasWindowFocus; }

        void exit() { m_done = true; }

        void setRootWidget(UIWidget* widget)          { m_rootWidget = widget; }
        void setFocusedWidget(UIWidget* widget);
        void setGrabbedWidget(UIWidget* widget);
        void setOffsetPosition(const Point& point)    { m_offsetPosition = point; }

        UIWidget* getRootWidget()               const { return m_rootWidget; }
        UIWidget* getFocusedWidget()            const { return m_focusWidget; }
        UIWidget* getGrabbedWidget()            const { return m_grabWidget; }

        bool  isMousePressed(uint8_t id)        const { return m_mousePressed[id]; }
        Point getMousePosition(uint8_t id)      const { return m_mousePosition[id]; }
        Point getMouseLastPosition(uint8_t id)  const { return m_mouseLastPosition[id]; }
        Point getMouseClickPosition(uint8_t id) const { return m_mouseClickPosition[id]; }
        bool  isKeyPressed(uint16_t key)        const;

        Rect getRootRect() const;

    private:
        void onMouseMove(uint8_t id, const Point& position);
        void onMousePress(uint8_t id, const Point& position, bool executed);
        void onKeyPress(uint16_t key, bool executed);
        void onDragFile(const std::string& path);

    private:
        bool m_done;
        bool m_hasWindowFocus;

        bool m_mousePressed[6];
        Point m_mousePosition[6];
        Point m_mouseLastPosition[6];
        Point m_mouseClickPosition[6];
        Point m_offsetPosition;
        std::map<uint16_t, bool> m_keyState;

        UIWidget* m_rootWidget;
        UIWidget* m_focusWidget;
        UIWidget* m_grabWidget;
        UIWidget* m_hoveredWidget;
};
extern Platform g_platform;


#endif // __PLATFORM_H__ //
