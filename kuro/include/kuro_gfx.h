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
typedef struct _Kuro_Gfx_Pipeline *Kuro_Gfx_Pipeline;
typedef struct _Kuro_Gfx_Pass *Kuro_Gfx_Pass;
typedef struct _Kuro_Gfx_Commands *Kuro_Gfx_Commands;

typedef struct Kuro_Gfx_Color {
    float r, g, b, a;
} Kuro_Gfx_Color;

KURO_GFX_API Kuro_Gfx kuro_gfx_create();
KURO_GFX_API void kuro_gfx_destroy(Kuro_Gfx gfx);

KURO_GFX_API Kuro_Gfx_Swapchain kuro_gfx_swapchain_create(Kuro_Gfx gfx, uint32_t width, uint32_t height, void *window_handle);
KURO_GFX_API void kuro_gfx_swapchain_destroy(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain);
KURO_GFX_API void kuro_gfx_swapchain_resize(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain, uint32_t width, uint32_t height);
KURO_GFX_API void kuro_gfx_swapchain_present(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain);

KURO_GFX_API Kuro_Gfx_Buffer kuro_gfx_buffer_create(Kuro_Gfx gfx, void *initial_data, uint32_t size_in_bytes);
KURO_GFX_API void kuro_gfx_buffer_destroy(Kuro_Gfx gfx, Kuro_Gfx_Buffer buffer);

KURO_GFX_API Kuro_Gfx_Pipeline kuro_gfx_pipeline_create(Kuro_Gfx gfx, const char *shader, uint32_t shader_size);
KURO_GFX_API void kuro_gfx_pipeline_destroy(Kuro_Gfx gfx, Kuro_Gfx_Pipeline pipeline);

KURO_GFX_API Kuro_Gfx_Pass kuro_gfx_pass_from_swapchain(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain);
KURO_GFX_API void kuro_gfx_pass_free(Kuro_Gfx gfx, Kuro_Gfx_Pass pass);

KURO_GFX_API Kuro_Gfx_Commands kuro_gfx_commands_create(Kuro_Gfx gfx);
KURO_GFX_API void kuro_gfx_commands_destroy(Kuro_Gfx gfx, Kuro_Gfx_Commands commands);

KURO_GFX_API void kuro_gfx_commands_begin(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Pipeline pipeline);
KURO_GFX_API void kuro_gfx_commands_end(Kuro_Gfx gfx, Kuro_Gfx_Commands commands);
KURO_GFX_API void kuro_gfx_commands_pass_begin(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Pass pass);
KURO_GFX_API void kuro_gfx_commands_pass_end(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Pass pass);
KURO_GFX_API void kuro_gfx_commands_pass_clear(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Pass pass, Kuro_Gfx_Color color);
KURO_GFX_API void kuro_gfx_commands_draw(Kuro_Gfx gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Buffer vertex_buffer);

KURO_GFX_API void kuro_gfx_flush(Kuro_Gfx gfx);

#ifdef __cplusplus
} // extern "C"
#endif