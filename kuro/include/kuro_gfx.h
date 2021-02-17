#pragma once

#include <stdint.h>
#include <stdbool.h>

#define KURO_GFX_API

#ifdef __cplusplus
extern "C" {
#endif

/*
    max_render_targets = 8;
*/

typedef struct _Kuro_Gfx *Kuro_Gfx;
typedef struct _Kuro_Gfx_Swapchain *Kuro_Gfx_Swapchain;
typedef struct _Kuro_Gfx_Buffer *Kuro_Gfx_Buffer;
typedef struct _Kuro_Gfx_Vertex_Shader *Kuro_Gfx_Vertex_Shader;
typedef struct _Kuro_Gfx_Pixel_Shader *Kuro_Gfx_Pixel_Shader;
typedef struct _Kuro_Gfx_Pipeline *Kuro_Gfx_Pipeline;
typedef struct _Kuro_Gfx_Pass *Kuro_Gfx_Pass;
typedef struct _Kuro_Gfx_Commands *Kuro_Gfx_Commands;

typedef enum KURO_CONSTANT {
    KURO_CONSTANT_MAX_VERTEX_ATTRIPUTES = 16
} KURO_CONSTANT;

typedef enum KURO_GFX_USAGE {
    KURO_GFX_USAGE_STATIC,
    KURO_GFX_USAGE_DYNAMIC
} KURO_GFX_USAGE;

typedef enum KURO_GFX_FORMAT {
    KURO_GFX_FORMAT_NONE,
    KURO_GFX_FORMAT_R32G32_FLOAT,
    KURO_GFX_FORMAT_R32G32B32_FLOAT
} KURO_GFX_FORMAT;

typedef enum KURO_GFX_CLASS {
    KURO_GFX_CLASS_PER_VERTEX,
    KURO_GFX_CLASS_PER_INSTANCE,
} KURO_GFX_CLASS;

typedef enum KURO_GFX_PRIMITIVE {
    KURO_GFX_PRIMITIVE_TRIANGLE
} KURO_GFX_PRIMITIVE;

typedef struct Kuro_Gfx_Vertex_Attribute {
    KURO_GFX_FORMAT format;
    KURO_GFX_CLASS classification;
    uint32_t slot;
} Kuro_Gfx_Vertex_Attribure;

typedef struct Kuro_Gfx_Color {
    float r, g, b, a;
} Kuro_Gfx_Color;

typedef struct Kuro_Gfx_Vertex_Buffer_Desc {
    Kuro_Gfx_Buffer buffer;
    uint32_t stride;
} Kuro_Gfx_Vertex_Buffer_Desc;

typedef struct Kuro_Gfx_Pipeline_Desc {
    Kuro_Gfx_Vertex_Shader vertex_shader;
    Kuro_Gfx_Pixel_Shader pixel_shader;
    Kuro_Gfx_Vertex_Attribure vertex_attribures[KURO_CONSTANT_MAX_VERTEX_ATTRIPUTES];
} Kuro_Gfx_Pipeline_Desc;

typedef struct Kuro_Gfx_Draw_Desc {
    KURO_GFX_PRIMITIVE primitive;
    Kuro_Gfx_Vertex_Buffer_Desc vertex_buffers[KURO_CONSTANT_MAX_VERTEX_ATTRIPUTES];
} Kuro_Gfx_Draw_Desc;

KURO_GFX_API Kuro_Gfx kuro_gfx_create();
KURO_GFX_API void kuro_gfx_destroy(Kuro_Gfx gfx);

KURO_GFX_API Kuro_Gfx_Swapchain kuro_gfx_swapchain_create(Kuro_Gfx gfx, uint32_t width, uint32_t height, void *window_handle);
KURO_GFX_API void kuro_gfx_swapchain_destroy(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain);
KURO_GFX_API void kuro_gfx_swapchain_resize(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain, uint32_t width, uint32_t height);
KURO_GFX_API void kuro_gfx_swapchain_present(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain);

KURO_GFX_API Kuro_Gfx_Buffer kuro_gfx_buffer_create(Kuro_Gfx gfx, KURO_GFX_USAGE usage, void *initial_data, uint32_t size_in_bytes);
KURO_GFX_API void kuro_gfx_buffer_destroy(Kuro_Gfx gfx, Kuro_Gfx_Buffer buffer);

KURO_GFX_API Kuro_Gfx_Vertex_Shader kuro_gfx_vertex_shader_create(Kuro_Gfx gfx, const char *shader, const char *entry_point);
KURO_GFX_API void kuro_gfx_vertex_shader_destroy(Kuro_Gfx gfx, Kuro_Gfx_Vertex_Shader vertex_shader);
KURO_GFX_API Kuro_Gfx_Pixel_Shader kuro_gfx_pixel_shader_create(Kuro_Gfx gfx, const char *shader, const char *entry_point);
KURO_GFX_API void kuro_gfx_pixel_shader_destroy(Kuro_Gfx gfx, Kuro_Gfx_Pixel_Shader pixel_shader);

KURO_GFX_API Kuro_Gfx_Pipeline kuro_gfx_pipeline_create(Kuro_Gfx gfx, Kuro_Gfx_Pipeline_Desc desc);
KURO_GFX_API void kuro_gfx_pipeline_destroy(Kuro_Gfx gfx, Kuro_Gfx_Pipeline pipeline);

KURO_GFX_API Kuro_Gfx_Pass kuro_gfx_pass_from_swapchain(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain);
KURO_GFX_API void kuro_gfx_pass_free(Kuro_Gfx gfx, Kuro_Gfx_Pass pass);

KURO_GFX_API Kuro_Gfx_Commands kuro_gfx_commands_create(Kuro_Gfx gfx);
KURO_GFX_API void kuro_gfx_commands_destroy(Kuro_Gfx gfx, Kuro_Gfx_Commands commands);

KURO_GFX_API void kuro_gfx_commands_begin(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Pipeline pipeline);
KURO_GFX_API void kuro_gfx_commands_end(Kuro_Gfx gfx, Kuro_Gfx_Commands commands);
KURO_GFX_API void kuro_gfx_commands_upload(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Buffer static_buffer);
KURO_GFX_API void kuro_gfx_commands_pass_begin(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Pass pass);
KURO_GFX_API void kuro_gfx_commands_pass_end(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Pass pass);
KURO_GFX_API void kuro_gfx_commands_viewport(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, uint32_t width, uint32_t height);
KURO_GFX_API void kuro_gfx_commands_pass_clear(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Pass pass, Kuro_Gfx_Color color);
KURO_GFX_API void kuro_gfx_commands_draw(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Draw_Desc desc);

KURO_GFX_API void kuro_gfx_flush(Kuro_Gfx gfx);

#ifdef __cplusplus
} // extern "C"
#endif