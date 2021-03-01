#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum PLATFORM_KEY
{
    PLATFORM_KEY_MOUSE_LEFT,
    PLATFORM_KEY_MOUSE_MIDDLE,
    PLATFORM_KEY_MOUSE_RIGHT,
    PLATFORM_KEY_W,
    PLATFORM_KEY_A,
    PLATFORM_KEY_S,
    PLATFORM_KEY_D,
    PLATFORM_KEY_E,
    PLATFORM_KEY_Q,

    PLATFORM_KEY_COUNT
};

typedef struct kr_key_state_t
{
    bool pressed; // once we press
    bool released; // once we release
    bool down;

    int press_count;
    int release_count;
} kr_key_state_t;

typedef struct kr_input_t
{
    int16_t mouse_x, mouse_y;
    int16_t mouse_dx, mouse_dy;
    float mouse_wheel;
    kr_key_state_t keys[PLATFORM_KEY_COUNT];
} kr_input_t;

typedef struct kr_window_t
{
    const char *title;
    uint16_t width, height;
    void *handle;
    kr_input_t input;
} kr_window_t;

kr_window_t * kr_window_create(const char *title, uint16_t width, uint16_t height);
void kr_window_destroy(kr_window_t *window);
bool kr_window_update(kr_window_t *window);

#ifdef __cplusplus
} // extern "C"
#endif;