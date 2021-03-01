#include <kuro/window.h>
#include <kuro/gfx.h>

int main()
{
    kr_window_t *window = kr_window_create("playground", 800, 600);

    Kuro_Gfx gfx = kuro_gfx_create();
    Kuro_Gfx_Commands commands = kuro_gfx_commands_create(gfx);
    Kuro_Gfx_Swapchain swapchain = kuro_gfx_swapchain_create(gfx, window->width, window->height, window->handle);
    Kuro_Gfx_Pass pass = kuro_gfx_pass_from_swapchain(gfx, swapchain);

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
        -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, +0.5f,  0.0f, 1.0f, 0.0f,
        +0.5f, +0.5f,  0.0f, 0.0f, 1.0f,
        +0.5f, -0.5f,  0.0f, 1.0f, 1.0f
    };

    uint16_t indices[] = {
        0, 2, 1,
        0, 3, 2
    };

    Kuro_Gfx_Buffer vertex_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_NONE, sizeof(vertices));
    Kuro_Gfx_Buffer vertex_upload_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_WRITE, sizeof(vertices));
    kuro_gfx_buffer_write(gfx, vertex_upload_buffer, vertices, sizeof(vertices));

    Kuro_Gfx_Buffer index_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_NONE, sizeof(indices));
    Kuro_Gfx_Buffer index_upload_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_WRITE, sizeof(indices));
    kuro_gfx_buffer_write(gfx, index_upload_buffer, indices, sizeof(indices));

    kuro_gfx_commands_begin(gfx, commands);
    Kuro_Gfx_Image depth_target = kuro_gfx_image_create(gfx, commands, window->width, window->height);
    kuro_gfx_commands_buffer_copy(commands, vertex_upload_buffer, vertex_buffer);
    kuro_gfx_commands_buffer_copy(commands, index_upload_buffer, index_buffer);
    kuro_gfx_commands_end(gfx, commands);
    kuro_gfx_sync(gfx);

    kuro_gfx_buffer_destroy(gfx, vertex_upload_buffer);
    kuro_gfx_buffer_destroy(gfx, index_upload_buffer);

    Kuro_Gfx_Buffer constant_buffer = kuro_gfx_buffer_create(gfx, KURO_GFX_ACCESS_WRITE, 256);
    kuro_gfx_pipeline_set_constant_buffer(gfx, pipeline, constant_buffer, 0);

    uint16_t width = window->width;
    uint16_t height = window->height;

    while (kr_window_update(window))
    {
        if (width != window->width || height != window->height)
        {
            width = window->width;
            height = window->height;

            kuro_gfx_swapchain_resize(gfx, swapchain, width, height);

            kuro_gfx_image_destroy(gfx, depth_target);
            kuro_gfx_commands_begin(gfx, commands);
            depth_target = kuro_gfx_image_create(gfx, commands, width, height);
            kuro_gfx_commands_end(gfx, commands);
            kuro_gfx_sync(gfx);
        }

        float t = 0.5f;
        kuro_gfx_buffer_write(gfx, constant_buffer, &t, sizeof(t));

        kuro_gfx_commands_begin(gfx, commands);
        {
            kuro_gfx_commands_set_pipeline(commands, pipeline);
            kuro_gfx_commands_pass_begin(commands, pass, depth_target);
            kuro_gfx_commands_viewport(commands, width, height);

            kuro_gfx_commands_clear_color(commands, pass, {1.0f, 1.0f, 0.0f, 1.0f});
            kuro_gfx_commands_clear_depth(commands, depth_target, 1.0f);

            Kuro_Gfx_Draw_Desc draw_desc = {};
            draw_desc.vertex_buffers[0].buffer = vertex_buffer;
            draw_desc.vertex_buffers[0].stride = 5 * sizeof(float);
            draw_desc.index_buffer.buffer = index_buffer;
            draw_desc.index_buffer.format = KURO_GFX_FORMAT_R16_UINT;
            draw_desc.count = 6;
            kuro_gfx_commands_draw(commands, draw_desc);

            kuro_gfx_commands_pass_end(commands, pass);
        }
        kuro_gfx_commands_end(gfx, commands);
        kuro_gfx_swapchain_present(gfx, swapchain);
        kuro_gfx_sync(gfx);
    }

    // release resources
    kuro_gfx_buffer_destroy(gfx, constant_buffer);
    kuro_gfx_buffer_destroy(gfx, index_buffer);
    kuro_gfx_buffer_destroy(gfx, vertex_buffer);
    kuro_gfx_image_destroy(gfx, depth_target);
    kuro_gfx_pipeline_destroy(gfx, pipeline);
    kuro_gfx_pixel_shader_destroy(gfx, pixel_shader);
    kuro_gfx_vertex_shader_destroy(gfx, vertex_shader);
    kuro_gfx_pass_free(gfx, pass);
    kuro_gfx_swapchain_destroy(gfx, swapchain);
    kuro_gfx_commands_destroy(gfx, commands);
    kuro_gfx_destroy(gfx);
    kr_window_destroy(window);

    return 0;
}