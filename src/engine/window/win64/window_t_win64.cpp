#include "window_t_win64.hpp"

namespace engine {
	void Window_T_Win64::returnFromFullscreen() {
		SetWindowLongPtr(m_window, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_window, NULL, m_x, m_y, m_width, m_height, SWP_FRAMECHANGED);
	}

	void Window_T_Win64::hide() {
		if (m_window_state == WindowState::eFullscreen) returnFromFullscreen();
		if (ShowWindow(m_window, SW_HIDE)) m_window_state = WindowState::eHidden;
	}

	void Window_T_Win64::restore() {
		if (m_window_state == WindowState::eFullscreen) returnFromFullscreen();
		if (ShowWindow(m_window, SW_RESTORE)) m_window_state = WindowState::eRestored;
	}

	void Window_T_Win64::maximize() {
		if (m_window_state == WindowState::eFullscreen) returnFromFullscreen();
		if (ShowWindow(m_window, SW_MAXIMIZE)) m_window_state = WindowState::eMaximized;
	}

	void Window_T_Win64::minimize() {
		if (m_window_state == WindowState::eFullscreen) returnFromFullscreen();
		if (ShowWindow(m_window, SW_MINIMIZE)) m_window_state = WindowState::eMinimized;
	}

	void Window_T_Win64::fullscreen() {
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		SetWindowLongPtr(m_window, GWL_STYLE, WS_VISIBLE | WS_POPUP);
		SetWindowPos(m_window, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);
	}

	void Window_T_Win64::resize(uint32_t _width, uint32_t _height) {
		if (SetWindowPos(m_window, NULL, 0, 0, _width, _height, SWP_NOMOVE)) {
			m_width = _width;
			m_height = _height;
		}
	}

	void Window_T_Win64::move(uint32_t _x, uint32_t _y) {
		if (SetWindowPos(m_window, NULL, _x, _y, 0, 0, SWP_NOSIZE)) {
			m_x = _x;
			m_y = _y;
		}
	}
}