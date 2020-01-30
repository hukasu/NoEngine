/**
 * Engine
 * © Lucas "Hukasu" Farias
 */

#ifndef __WINDOW__T__WIN64__HPP__
#define __WINDOW__T__WIN64__HPP__

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <Windows.h>
#include <cstdint>

namespace engine {
    struct Window_T_Win64 {
        HWND m_window;
        enum class WindowState {
            eHidden,
            eRestored,
            eMaximized,
            eMinimized,
            eFullscreen
        } m_window_state;
        uint32_t m_width, m_height, m_x, m_y;

        void returnFromFullscreen();

        void hide();
        void restore();
        void maximize();
        void minimize();
        void fullscreen();

        void resize(uint32_t _width, uint32_t _height);
        void move(uint32_t _x, uint32_t _y);
    };

    typedef Window_T_Win64 Window_T_PlatformSpecific;
}

#endif // __WINDOW__T__WIN64__HPP__