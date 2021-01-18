#pragma comment(lib, "user32.lib")

#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <kuro_gfx.cpp>

LRESULT CALLBACK
window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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

int
main()
{
    // register window class
    {
        WNDCLASSEX wnd_class = {};
        wnd_class.cbSize = sizeof(wnd_class);
        wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wnd_class.lpfnWndProc = window_proc;
        wnd_class.lpszClassName = L"dx11_wnd_class";

        if (RegisterClassEx(&wnd_class) == 0)
        {
            OutputDebugString(L"Failed to register window class");
            return GetLastError();
        }
    }

    // create window
    HWND hwnd = CreateWindowEx(
        0,
        L"dx11_wnd_class",
        L"example cubes",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );
    if (hwnd == nullptr)
    {
        OutputDebugString(L"Failed to create window");
        return GetLastError();
    }

    // get window width and height
    RECT rect;
    GetClientRect(hwnd, &rect);
    int window_width = rect.right - rect.left;
    int window_height = rect.bottom - rect.top;

    kuro::Gfx gfx = kuro::gfx_init();
    kuro::Swapchain swapchain = kuro::gfx_swapchain_new(gfx, hwnd, window_width, window_height);

    bool running = true;
    while (running)
    {
        MSG msg = {};
        PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        switch (msg.message)
        {
            case WM_QUIT:
                running = false;
                break;
        }

        GetClientRect(hwnd, &rect);
        int current_width = rect.right - rect.left;
        int current_height = rect.bottom - rect.top;

        if (current_width != window_width || current_height != window_height)
        {
            window_width = current_width;
            window_height = current_height;

            kuro::gfx_swapchain_resize(gfx, swapchain, window_width, window_height);
        }
    }

    // release resources
    kuro::gfx_swapchain_free(gfx, swapchain);
    kuro::gfx_deinit(gfx);
    DestroyWindow(hwnd);

    return 0;
}