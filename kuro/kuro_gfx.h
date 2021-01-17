//
// kuro_gfx - graphics API (dx12)
//
// Copyright (c) 2020-2021 Waleed Yaser
//
#pragma once

namespace kuro
{
    // =================================================================================================
    // == TYPEDEFS =====================================================================================
    // =================================================================================================

    using i8  = signed char;
    using i16 = short;
    using i32 = int;
    using i64 = long long;
    using u8  = unsigned char;
    using u16 = unsigned short;
    using u32 = unsigned int;
    using u64 = unsigned long long;
    using f32 = float;
    using f64 = double;

    // =================================================================================================
    // == API ==========================================================================================
    // =================================================================================================

    using Gfx = struct _Gfx *;
    using Swapchain = struct _Swapchain *;

    Gfx gfx_init();
    void gfx_deinit(Gfx self);

    Swapchain gfx_swapchain_new(Gfx self, void *window);
    void gfx_swapchain_free(Gfx self, Swapchain swapchain);
    void gfx_swapchain_present(Gfx self, Swapchain swapchain);
    void gfx_swapchain_resize(Gfx self, Swapchain swapchain, i32 width, i32 height);
}