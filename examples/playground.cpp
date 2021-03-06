#include <kuro/window.h>
#include <kuro/gfx.h>
#include <kuro/kuro_os.h>
#include <stdio.h>

int main()
{
    kr_window_t *window = kr_window_create("playground", 800, 600);

    kr_gfx_t gfx = kuro_gfx_create();
    kr_commands_t commands = kuro_gfx_commands_create(gfx);
    kr_swapchain_t swapchain = kuro_gfx_swapchain_create(gfx, window->width, window->height, window->handle);
    kr_image_t depth_target = kuro_gfx_image_create(gfx, window->width, window->height);

    const char shader[] = R"(
        cbuffer Constant_Buffer : register(b0)
        {
            float t;
        };

        struct PS_Input
        {
            float4 position : SV_POSITION;
            float3 color : COLOR;
        };

        PS_Input vs_main(float2 pos : POSITION, float3 color : COLOR)
        {
            PS_Input output;
            output.position = float4(pos, 0.0f, 1.0f);
            output.color = t * color;
            return output;
        }

        float4 ps_main(PS_Input input) : SV_TARGET
        {
            return float4(input.color, 1.0f);
        }
    )";

    kr_vshader_t vertex_shader = kuro_gfx_vertex_shader_create(gfx, shader, "vs_main");
    kr_pshader_t pixel_shader = kuro_gfx_pixel_shader_create(gfx, shader, "ps_main");

    Kuro_Gfx_Pipeline_Desc pipeline_desc = {};
    pipeline_desc.vertex_shader = vertex_shader;
    pipeline_desc.pixel_shader = pixel_shader;
    pipeline_desc.vertex_attribures[0].format = KURO_GFX_FORMAT_R32G32_FLOAT;
    pipeline_desc.vertex_attribures[1].format = KURO_GFX_FORMAT_R32G32B32_FLOAT;
    kr_pipeline_t pipeline = kuro_gfx_pipeline_create(gfx, pipeline_desc);

    float vertices[] = {
        // position     color
        -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, +0.5f,  0.0f, 1.0f, 0.0f,
        +0.5f, +0.5f,  0.0f, 0.0f, 1.0f,
        +0.5f, -0.5f,  0.0f, 1.0f, 1.0f
    };

    uint16_t indices[] = {
        0, 2, 1,
        0, 3, 2
    };

    kr_buffer_t vertex_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_NONE, vertices, sizeof(vertices));
    kr_buffer_t index_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_NONE, indices, sizeof(indices));
    kr_buffer_t constant_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_WRITE, nullptr, 256);

    uint16_t width = window->width;
    uint16_t height = window->height;

    double target_time = 1.0 / 60.0;
    double begin_time = kuro::os_seconds();
    double total_time = 0.0;
    while (kr_window_update(window))
    {
        if (width != window->width || height != window->height)
        {
            width = window->width;
            height = window->height;

            if (width == 0) width = 1;
            if (height == 0) height = 1;

            kuro_gfx_swapchain_resize(gfx, swapchain, width, height);
            kuro_gfx_image_destroy(gfx, depth_target);
            depth_target = kuro_gfx_image_create(gfx, width, height);
            begin_time = kuro::os_seconds();
        }

        kuro_gfx_commands_begin(gfx, commands, swapchain, depth_target);
        {
            kuro_gfx_set_pipeline(commands, pipeline);
            kuro_gfx_viewport(commands, width, height);

            kuro_gfx_clear(commands, {1.0f, 1.0f, 0.0f, 1.0f}, 1.0f);

            float t = (float)window->input.mouse_x / (float)window->width;
            kuro_gfx_buffer_write(commands, constant_buffer, &t, sizeof(t));
            kuro_gfx_buffer_bind(commands, constant_buffer, 0);

            Kuro_Gfx_Draw_Desc draw_desc = {};
            draw_desc.vertex_buffers[0].buffer = vertex_buffer;
            draw_desc.vertex_buffers[0].stride = 5 * sizeof(float);
            draw_desc.index_buffer.buffer = index_buffer;
            draw_desc.index_buffer.format = KURO_GFX_FORMAT_R16_UINT;
            draw_desc.count = 6;
            kuro_gfx_draw(commands, draw_desc);
        }
        kuro_gfx_commands_end(gfx, commands);
        kuro_gfx_sync(gfx);

        // timing
        double end_time = kuro::os_seconds();
        double frame_time = end_time - begin_time;
        if (frame_time < target_time)
            kuro::os_sleep(target_time - frame_time);
        double sleep_time = kuro::os_seconds() - end_time;
        double dt = frame_time + sleep_time;
        begin_time = end_time;
        total_time += dt;

        char title[256];
        sprintf_s(
            title, sizeof(title),
            "playground - total_time: %0.4f s | dt: %7.4f ms | fps: %2.0f | frame time: %7.4f ms | sleep time: %7.4f ms",
            total_time, dt * 1000.0, 1.0 / dt, frame_time * 1000.0, sleep_time * 1000.0);
        kr_window_title_set(window, title);
        printf("%s\n", title);
    }

    // release resources
    kuro_gfx_buffer_destroy(gfx, constant_buffer);
    kuro_gfx_buffer_destroy(gfx, index_buffer);
    kuro_gfx_buffer_destroy(gfx, vertex_buffer);
    kuro_gfx_pipeline_destroy(gfx, pipeline);
    kuro_gfx_pixel_shader_destroy(gfx, pixel_shader);
    kuro_gfx_vertex_shader_destroy(gfx, vertex_shader);
    kuro_gfx_image_destroy(gfx, depth_target);
    kuro_gfx_swapchain_destroy(gfx, swapchain);
    kuro_gfx_commands_destroy(gfx, commands);
    kuro_gfx_destroy(gfx);
    kr_window_destroy(window);

    return 0;
}