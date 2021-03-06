#include <kuro/window.h>
#include <kuro/gfx.h>
#include <kuro/kuro_os.h>
#include <kuro/kuro_math.h>
#include <stdio.h>

struct Pass_Constants
{
    kuro::mat4 view;
    kuro::mat4 view_inv;
    kuro::mat4 proj;
    kuro::mat4 proj_inv;
    kuro::mat4 view_proj;
    kuro::mat4 view_proj_inv;
    kuro::vec3 cam_pos;
    char padding0[4];
    kuro::vec2 render_target_size;
    kuro::vec2 render_target_size_inv;
    float cam_near;
    float cam_far;
    float delta_time;
    float total_time;
    char padding1[80];
};

struct Object_Constants
{
    kuro::mat4 model;
    char padding[192];
};

int main()
{
    size_t size = 0;
    size = sizeof(Pass_Constants);

    kr_window_t *window = kr_window_create("playground", 800, 600);

    kr_gfx_t gfx = kuro_gfx_create();
    kr_commands_t commands = kuro_gfx_commands_create(gfx);
    kr_swapchain_t swapchain = kuro_gfx_swapchain_create(gfx, window->width, window->height, window->handle);
    kr_image_t depth_target = kuro_gfx_image_create(gfx, window->width, window->height);

    const char shader[] = R"(
        cbuffer Pass_Constants : register(b0)
        {
            float4x4 view;
            float4x4 view_inv;
            float4x4 proj;
            float4x4 proj_inv;
            float4x4 view_proj;
            float4x4 view_proj_inv;
            float3 cam_pos;
            float2 render_target_size;
            float2 render_target_size_inv;
            float cam_near;
            float cam_far;
            float delta_time;
            float total_time;
        };

        cbuffer Object_Constants : register(b1)
        {
            float4x4 model;
        };

        struct VS_Out
        {
            float4 position : SV_POSITION;
            float3 color : COLOR;
        };

        VS_Out vs_main(float2 pos : POSITION, float3 color : COLOR)
        {
            VS_Out output;
            output.position = mul(mul(float4(pos, 0.0f, 1.0f), model), view_proj);
            output.color = color;
            return output;
        }

        float4 ps_main(VS_Out input) : SV_TARGET
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

    kr_buffer_t pass_constants_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_WRITE, nullptr, sizeof(Pass_Constants));
    kr_buffer_t object_constants_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_WRITE, nullptr, sizeof(Object_Constants));

    uint16_t width = window->width;
    uint16_t height = window->height;

    double target_time = 1.0 / 60.0;
    double begin_time = kuro::os_seconds();
    double total_time = 0.0;
    double dt = 0.0;
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

            Pass_Constants pass_constants = {};
            pass_constants.view = kuro::mat4_identity();
            pass_constants.view_inv = kuro::mat4_inverse(pass_constants.view);
            pass_constants.proj = kuro::mat4_identity();
            pass_constants.proj_inv = kuro::mat4_inverse(pass_constants.proj);
            pass_constants.view_proj = pass_constants.view * pass_constants.proj;
            pass_constants.cam_pos = {};
            pass_constants.render_target_size = {(float)width, (float)height};
            pass_constants.render_target_size_inv = {1.0f / (float)width, 1.0f / (float)height};
            pass_constants.cam_near = 0.0f;
            pass_constants.cam_far = 0.0f;
            pass_constants.delta_time = (float)dt;
            pass_constants.total_time = (float)total_time;
            kuro_gfx_buffer_write(commands, pass_constants_buffer, &pass_constants, sizeof(pass_constants));
            kuro_gfx_buffer_bind(commands, pass_constants_buffer, 0);

            Object_Constants object_constants = {};
            object_constants.model = kuro::mat4_identity();
            kuro_gfx_buffer_write(commands, object_constants_buffer, &object_constants, sizeof(object_constants));
            kuro_gfx_buffer_bind(commands, object_constants_buffer, 1);

            Kuro_Gfx_Draw_Desc draw_desc = {};
            draw_desc.vertex_buffers[0].buffer = vertex_buffer;
            draw_desc.vertex_buffers[0].stride = 5 * sizeof(float);
            draw_desc.index_buffer.buffer = index_buffer;
            draw_desc.index_buffer.format = KURO_GFX_FORMAT_R16_UINT;
            draw_desc.count = 6;
            kuro_gfx_draw(commands, draw_desc);
        }
        kuro_gfx_commands_end(gfx, commands);

        // timing
        double end_time = kuro::os_seconds();
        double frame_time = end_time - begin_time;
        if (frame_time < target_time)
            kuro::os_sleep(target_time - frame_time);
        double sleep_time = kuro::os_seconds() - end_time;
        dt = frame_time + sleep_time;
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
    kuro_gfx_buffer_destroy(gfx, object_constants_buffer);
    kuro_gfx_buffer_destroy(gfx, pass_constants_buffer);
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