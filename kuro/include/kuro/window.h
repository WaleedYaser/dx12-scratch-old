#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kr_window_t
{
    const char *title;
    uint16_t width, height;
    void *handle;
} kr_window_t;

kr_window_t * kr_window_create(const char *title, uint16_t width, uint16_t height);
void kr_window_destroy(kr_window_t *window);
bool kr_window_update(kr_window_t *window);

#ifdef __cplusplus
} // extern "C"
#endif;