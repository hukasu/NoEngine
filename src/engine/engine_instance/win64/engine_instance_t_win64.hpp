/**
 * Engine
 * © Lucas "Hukasu" Farias
 */

#ifndef __ENGINE__INSTANCE__T__WIN64__HPP__
#define __ENGINE__INSTANCE__T__WIN64__HPP__

#include <Windows.h>

namespace engine {
    struct EngineInstance_T_Win64 {
        HINSTANCE m_windows_application_instance;
        ATOM m_windows_window_class;

        void initialize();
        void destroy();

        HINSTANCE getWindowsApplicationInstance();

        ATOM registerWindowClass();
        void unregisterWindowClass();
    };

    typedef EngineInstance_T_Win64 EngineInstance_T_PlatformSpecific;
}

#endif // __ENGINE__INSTANCE__T__WIN64__HPP__