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
    for (int32_t i = 0; i < PLATFORM_KEY_COUNT; ++i)
    {
        window->input.keys[i].pressed = false;
        window->input.keys[i].released = false;
        window->input.keys[i].press_count = 0;
        window->input.keys[i].release_count = 0;
    }
    window->input.mouse_wheel = 0.0f;

    MSG msg = {0};
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        switch (msg.message)
        {
            case WM_QUIT:
                return false;
            case WM_LBUTTONDOWN:
                SetCapture((HWND)window->handle);
                window->input.keys[PLATFORM_KEY_MOUSE_LEFT].pressed = true;
                window->input.keys[PLATFORM_KEY_MOUSE_LEFT].down    = true;
                window->input.keys[PLATFORM_KEY_MOUSE_LEFT].press_count++;
                break;
            case WM_LBUTTONUP:
                SetCapture(NULL);
                window->input.keys[PLATFORM_KEY_MOUSE_LEFT].released = true;
                window->input.keys[PLATFORM_KEY_MOUSE_LEFT].down     = false;
                window->input.keys[PLATFORM_KEY_MOUSE_LEFT].release_count++;
                break;

            case WM_RBUTTONDOWN:
                SetCapture((HWND)window->handle);
                window->input.keys[PLATFORM_KEY_MOUSE_RIGHT].pressed = true;
                window->input.keys[PLATFORM_KEY_MOUSE_RIGHT].down    = true;
                window->input.keys[PLATFORM_KEY_MOUSE_RIGHT].press_count++;
                break;
            case WM_RBUTTONUP:
                SetCapture(NULL);
                window->input.keys[PLATFORM_KEY_MOUSE_RIGHT].released = true;
                window->input.keys[PLATFORM_KEY_MOUSE_RIGHT].down     = false;
                window->input.keys[PLATFORM_KEY_MOUSE_RIGHT].release_count++;
                break;

            case WM_MBUTTONDOWN:
                SetCapture((HWND)window->handle);
                window->input.keys[PLATFORM_KEY_MOUSE_MIDDLE].pressed = true;
                window->input.keys[PLATFORM_KEY_MOUSE_MIDDLE].down    = true;
                window->input.keys[PLATFORM_KEY_MOUSE_MIDDLE].press_count++;
                break;
            case WM_MBUTTONUP:
                SetCapture(NULL);
                window->input.keys[PLATFORM_KEY_MOUSE_MIDDLE].released = true;
                window->input.keys[PLATFORM_KEY_MOUSE_MIDDLE].down     = false;
                window->input.keys[PLATFORM_KEY_MOUSE_MIDDLE].release_count++;
                break;

            case WM_MOUSEWHEEL:
                window->input.mouse_wheel += (float)GET_WHEEL_DELTA_WPARAM(msg.wParam) / (float)WHEEL_DELTA;
                break;
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                int32_t was_down = (msg.lParam & (1 << 30));
                int32_t is_down = !(msg.lParam & (1 << 31));

                switch(msg.wParam)
                {
                    case 'W':
                        window->input.keys[PLATFORM_KEY_W].pressed  = is_down;
                        window->input.keys[PLATFORM_KEY_W].released = was_down;
                        if(is_down)
                            window->input.keys[PLATFORM_KEY_W].down = true;
                        else if(was_down)
                            window->input.keys[PLATFORM_KEY_W].down = false;
                        break;
                    case 'A':
                        window->input.keys[PLATFORM_KEY_A].pressed  = is_down;
                        window->input.keys[PLATFORM_KEY_A].released = was_down;
                        if(is_down)
                            window->input.keys[PLATFORM_KEY_A].down = true;
                        else if(was_down)
                            window->input.keys[PLATFORM_KEY_A].down = false;
                        break;
                    case 'S':
                        window->input.keys[PLATFORM_KEY_S].pressed  = is_down;
                        window->input.keys[PLATFORM_KEY_S].released = was_down;
                        if(is_down)
                            window->input.keys[PLATFORM_KEY_S].down = true;
                        else if(was_down)
                            window->input.keys[PLATFORM_KEY_S].down = false;
                        break;
                    case 'D':
                        window->input.keys[PLATFORM_KEY_D].pressed  = is_down;
                        window->input.keys[PLATFORM_KEY_D].released = was_down;
                        if(is_down)
                            window->input.keys[PLATFORM_KEY_D].down = true;
                        else if(was_down)
                            window->input.keys[PLATFORM_KEY_D].down = false;
                        break;
                    case 'E':
                        window->input.keys[PLATFORM_KEY_E].pressed  = is_down;
                        window->input.keys[PLATFORM_KEY_E].released = was_down;
                        if(is_down)
                            window->input.keys[PLATFORM_KEY_E].down = true;
                        else if(was_down)
                            window->input.keys[PLATFORM_KEY_E].down = false;
                        break;
                    case 'Q':
                        window->input.keys[PLATFORM_KEY_Q].pressed  = is_down;
                        window->input.keys[PLATFORM_KEY_Q].released = was_down;
                        if(is_down)
                            window->input.keys[PLATFORM_KEY_Q].down = true;
                        else if(was_down)
                            window->input.keys[PLATFORM_KEY_Q].down = false;
                        break;
                }
            }
        }
    }

    POINT mouse_point;
	GetCursorPos(&mouse_point);
	ScreenToClient((HWND)window->handle, &mouse_point);

	window->input.mouse_dx = (int16_t)mouse_point.x - window->input.mouse_x;
	window->input.mouse_dy = (int16_t)mouse_point.y - window->input.mouse_y;
	window->input.mouse_x  = (int16_t)mouse_point.x;
	window->input.mouse_y  = (int16_t)mouse_point.y;

    RECT rect;
    GetClientRect((HWND)window->handle, &rect);
    window->width = (uint16_t)(rect.right - rect.left);
    window->height = (uint16_t)(rect.bottom - rect.top);

    return true;
}

void
kr_window_title_set(kr_window_t *window, const char *title)
{
    SetWindowText((HWND)window->handle, title);
}