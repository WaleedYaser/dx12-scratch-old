#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _kr_gfx_t *kr_gfx_t;
typedef struct _kr_swapchain_t *kr_swapchain_t;
typedef struct _kr_image_t *kr_image_t;
typedef struct _kr_buffer_t *kr_buffer_t;
typedef struct _kr_vshader_t *kr_vshader_t;
typedef struct _kr_pshader_t *kr_pshader_t;
typedef struct _kr_pipeline_t *kr_pipeline_t;
typedef struct _kr_commands_t *kr_commands_t;

typedef enum KURO_CONSTANT {
    KURO_CONSTANT_MAX_RENDER_TARGETS = 8,
    KURO_CONSTANT_MAX_VERTEX_ATTRIPUTES = 16
} KURO_CONSTANT;

typedef enum KURO_GFX_ACCESS {
    KURO_GFX_ACCESS_NONE,
    KURO_GFX_ACCESS_READ,
    KURO_GFX_ACCESS_WRITE
} KURO_GFX_ACCESS;

typedef enum KURO_GFX_FORMAT {
    KURO_GFX_FORMAT_NONE,
    KURO_GFX_FORMAT_R16_UINT,
    KURO_GFX_FORMAT_R32_UINT,
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
    kr_buffer_t buffer;
    uint32_t stride;
} Kuro_Gfx_Vertex_Buffer_Desc;

typedef struct Kuro_Gfx_Index_Buffer_Desc {
    kr_buffer_t buffer;
    KURO_GFX_FORMAT format;
} Kuro_Gfx_Index_Buffer_Desc;

typedef struct Kuro_Gfx_Pipeline_Desc {
    kr_vshader_t vertex_shader;
    kr_pshader_t pixel_shader;
    Kuro_Gfx_Vertex_Attribure vertex_attribures[KURO_CONSTANT_MAX_VERTEX_ATTRIPUTES];
} Kuro_Gfx_Pipeline_Desc;

typedef struct Kuro_Gfx_Draw_Desc {
    KURO_GFX_PRIMITIVE primitive;
    Kuro_Gfx_Vertex_Buffer_Desc vertex_buffers[KURO_CONSTANT_MAX_VERTEX_ATTRIPUTES];
    Kuro_Gfx_Index_Buffer_Desc index_buffer;
    uint32_t count;
} Kuro_Gfx_Draw_Desc;

kr_gfx_t kuro_gfx_create();
void kuro_gfx_destroy(kr_gfx_t gfx);

kr_swapchain_t kuro_gfx_swapchain_create(kr_gfx_t gfx, uint32_t width, uint32_t height, void *window_handle);
void kuro_gfx_swapchain_destroy(kr_gfx_t gfx, kr_swapchain_t swapchain);
void kuro_gfx_swapchain_resize(kr_gfx_t gfx, kr_swapchain_t swapchain, uint32_t width, uint32_t height);

kr_image_t kuro_gfx_image_create(kr_gfx_t gfx, uint32_t width, uint32_t height);
void kuro_gfx_image_destroy(kr_gfx_t gfx, kr_image_t image);

kr_buffer_t kuro_gfx_buffer_create(kr_gfx_t gfx, KURO_GFX_ACCESS cpu_access, void *data, uint32_t size_in_bytes);
void kuro_gfx_buffer_destroy(kr_gfx_t gfx, kr_buffer_t buffer);

kr_vshader_t kuro_gfx_vertex_shader_create(kr_gfx_t gfx, const char *shader, const char *entry_point);
void kuro_gfx_vertex_shader_destroy(kr_gfx_t gfx, kr_vshader_t vertex_shader);
kr_pshader_t kuro_gfx_pixel_shader_create(kr_gfx_t gfx, const char *shader, const char *entry_point);
void kuro_gfx_pixel_shader_destroy(kr_gfx_t gfx, kr_pshader_t pixel_shader);

kr_pipeline_t kuro_gfx_pipeline_create(kr_gfx_t gfx, Kuro_Gfx_Pipeline_Desc desc);
void kuro_gfx_pipeline_destroy(kr_gfx_t gfx, kr_pipeline_t pipeline);

kr_commands_t kuro_gfx_commands_create(kr_gfx_t gfx);
void kuro_gfx_commands_destroy(kr_gfx_t gfx, kr_commands_t commands);

void kuro_gfx_commands_begin(kr_gfx_t gfx, kr_commands_t commands, kr_swapchain_t swapchain, kr_image_t depth_target);
void kuro_gfx_commands_end(kr_gfx_t gfx, kr_commands_t commands);

void kuro_gfx_set_pipeline(kr_commands_t commands, kr_pipeline_t pipeline);
void kuro_gfx_viewport(kr_commands_t commands, uint32_t width, uint32_t height);
void kuro_gfx_clear(kr_commands_t commands, Kuro_Gfx_Color color, float depth);
void kuro_gfx_buffer_write(kr_commands_t commands, kr_buffer_t buffer, void *data, uint32_t size_in_bytes);
void kuro_gfx_buffer_bind(kr_commands_t commands, kr_buffer_t buffer, uint32_t slot);
void kuro_gfx_draw(kr_commands_t commands, Kuro_Gfx_Draw_Desc desc);

void kuro_gfx_sync(kr_gfx_t gfx);

#ifdef __cplusplus
} // extern "C"
#endif