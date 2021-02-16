#pragma comment(lib, "user32.lib")

#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <kuro_gfx.h>

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
        L"playground",
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

    Kuro_Gfx gfx = kuro_gfx_create();
    Kuro_Gfx_Swapchain swapchain = kuro_gfx_swapchain_create(gfx, window_width, window_height, hwnd);
    Kuro_Gfx_Pass pass =kuro_gfx_pass_from_swapchain(gfx, swapchain);
    Kuro_Gfx_Commands commands = kuro_gfx_commands_create(gfx);

    const char shader[] = R"(
        struct PS_Input
        {
            float4 position : SV_POSITION;
            float3 color : COLOR;
        };

        PS_Input vs_main(float2 pos : POSITION, float3 color : COLOR)
        {
            PS_Input output;
            output.position = float4(pos, 0.0f, 1.0f);
            output.color = color;
            return output;
        }

        float4 ps_main(PS_Input input) : SV_TARGET
        {
            return float4(input.color, 1.0f);
        }
    )";

    Kuro_Gfx_Vertex_Shader vertex_shader = kuro_gfx_vertex_shader_create(gfx, shader, "vs_main");
    Kuro_Gfx_Pixel_Shader pixel_shader = kuro_gfx_pixel_shader_create(gfx, shader, "ps_main");

    Kuro_Gfx_Pipeline_Desc pipeline_desc = {};
    pipeline_desc.vertex_shader = vertex_shader;
    pipeline_desc.pixel_shader = pixel_shader;
    pipeline_desc.vertex_attribures[0].format = KURO_GFX_FORMAT_R32G32_FLOAT;
    pipeline_desc.vertex_attribures[1].format = KURO_GFX_FORMAT_R32G32B32_FLOAT;
    Kuro_Gfx_Pipeline pipeline = kuro_gfx_pipeline_create(gfx, pipeline_desc);

    float vertices[] = {
        // position     color
         0.0f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 0.0f, 1.0f
    };

    Kuro_Gfx_Buffer vertex_buffer = kuro_gfx_buffer_create(gfx, &vertices, sizeof(vertices));

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

            kuro_gfx_swapchain_resize(gfx, swapchain, window_width, window_height);
        }

        kuro_gfx_commands_begin(gfx, commands, pipeline);
        {
            kuro_gfx_commands_pass_begin(gfx, commands, pass);
            kuro_gfx_commands_viewport(gfx, commands, window_width, window_height);
            kuro_gfx_commands_pass_clear(gfx, commands, pass, {1.0f, 1.0f, 0.0f, 1.0f});

            Kuro_Gfx_Draw_Desc draw_desc = {};
            draw_desc.vertex_buffers[0].buffer = vertex_buffer;
            draw_desc.vertex_buffers[0].stride = 5 * sizeof(float);
            kuro_gfx_commands_draw(gfx, commands, draw_desc);

            kuro_gfx_commands_pass_end(gfx, commands, pass);
        }
        kuro_gfx_commands_end(gfx, commands);
        kuro_gfx_flush(gfx);

        kuro_gfx_swapchain_present(gfx, swapchain);
    }

    // release resources
    kuro_gfx_buffer_destroy(gfx, vertex_buffer);
    kuro_gfx_pipeline_destroy(gfx, pipeline);
    kuro_gfx_pixel_shader_destroy(gfx, pixel_shader);
    kuro_gfx_vertex_shader_destroy(gfx, vertex_shader);
    kuro_gfx_commands_destroy(gfx, commands);
    kuro_gfx_pass_free(gfx, pass);
    kuro_gfx_swapchain_destroy(gfx, swapchain);
    kuro_gfx_destroy(gfx);
    DestroyWindow(hwnd);

    return 0;
}