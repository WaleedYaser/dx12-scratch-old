#pragma comment(lib, "user32.lib")

#include "kuro/window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>
#include <stdlib.h>

LRESULT CALLBACK
_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
            break;
    }
}

kr_window_t *
kr_window_create(const char *title, uint16_t width, uint16_t height)
{
    assert(width > 0 && height > 0 && "can't create a window with 0 width or height");

    kr_window_t *window = (kr_window_t *)malloc(sizeof(kr_window_t));
    window->title = title;
    window->width = width;
    window->height = height;

    // register window class
    WNDCLASSEX wnd_class = {0};
    wnd_class.cbSize = sizeof(wnd_class);
    wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd_class.lpfnWndProc = _window_proc;
    wnd_class.lpszClassName = "kuro_wnd_class";
    ATOM class_atom = RegisterClassEx(&wnd_class);
    assert(class_atom != 0 && "failed to register kuro_wnd_class");

    RECT window_rect = {0, 0, width, height};
    AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

    // create window
    window->handle = CreateWindowEx(
        0,
        wnd_class.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        window_rect.right - window_rect.left,
        window_rect.bottom - window_rect.top,
        NULL,
        NULL,
        NULL,
        NULL
    );
    assert(window->handle && "failed to create window");

    return window;
}

void
kr_window_destroy(kr_window_t *window)
{
    bool res = false;
    res = DestroyWindow(window->handle);
    assert(res && "failed to destroy window");
    free(window);
}

bool
kr_window_update(kr_window_t *window)
{
	MSG msg = {0};
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		switch (msg.message)
		{
			case WM_QUIT:
				return false;
		}
	}

	RECT rect;
	GetClientRect((HWND)window->handle, &rect);
	window->width = (uint16_t)(rect.right - rect.left);
	window->height = (uint16_t)(rect.bottom - rect.top);

	return true;
}