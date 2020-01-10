#if defined(_WIN64)

#ifndef UNICODE
#define UNICODE
#endif

#define ENGINE_WINDOW_CLASS L"NoEngineWindowClass"

#include "engine_instance_t_win64.hpp"

namespace engine {
	static LRESULT CALLBACK WindowProc(
		_In_ HWND   hwnd,
		_In_ UINT   uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	) {
		// TODO
		switch (uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	};

	void EngineInstance_T_Win64::initialize() {
		m_windows_application_instance = getWindowsApplicationInstance();
		m_windows_window_class = registerWindowClass();
	}

	void EngineInstance_T_Win64::destroy() {
		unregisterWindowClass();
	}

	HINSTANCE EngineInstance_T_Win64::getWindowsApplicationInstance() {
		HINSTANCE application_instance = GetModuleHandle(nullptr);
		// TODO is there any checks to do here?
		return application_instance;
	}

	ATOM EngineInstance_T_Win64::registerWindowClass() {
		WNDCLASS wc{};
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = m_windows_application_instance;
		wc.lpszClassName = ENGINE_WINDOW_CLASS;

		ATOM w_class = RegisterClass(&wc);
		// TODO is there any checks to do here?
		return w_class;
	}

	void EngineInstance_T_Win64::unregisterWindowClass() {
		UnregisterClass(ENGINE_WINDOW_CLASS, m_windows_application_instance);
	}
}

#endif // _WIN64