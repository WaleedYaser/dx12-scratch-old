#pragma once

#include <stdint.h>
#include <stdbool.h>

#define KURO_GFX_API

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Kuro_Gfx *Kuro_Gfx;
typedef struct _Kuro_Gfx_Swapchain *Kuro_Gfx_Swapchain;
typedef struct _Kuro_Gfx_Buffer *Kuro_Gfx_Buffer;

KURO_GFX_API Kuro_Gfx kuro_gfx_create();
KURO_GFX_API void kuro_gfx_destroy(Kuro_Gfx gfx);

KURO_GFX_API Kuro_Gfx_Swapchain kuro_gfx_swapchain_create(Kuro_Gfx gfx, uint32_t width, uint32_t height, void *window_handle);
KURO_GFX_API void kuro_gfx_swapchain_destroy(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain);
KURO_GFX_API void kuro_gfx_swapchain_resize(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain);
KURO_GFX_API void kuro_gfx_swapchain_present(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain);

KURO_GFX_API Kuro_Gfx_Buffer kuro_gfx_buffer_create(Kuro_Gfx gfx);
KURO_GFX_API void kuro_gfx_buffer_destroy(Kuro_Gfx gfx, Kuro_Gfx_Buffer buffer);

KURO_GFX_API void kuro_gfx_flush(Kuro_Gfx gfx);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <assert.h>

typedef struct _Kuro_Gfx {
    IDXGIFactory4 *factory;
    ID3D12Device *device;
    ID3D12CommandQueue *command_queue;
} _Kuro_Gfx;

typedef struct _Kuro_Gfx_Swapchain {
    DXGI_FORMAT backbuffer_format;
    uint32_t buffer_count;
    bool msaa_state;
    uint32_t msaa_x4_quality;
    IDXGISwapChain *swapchain;
} _Kuro_Gfx_Swapchain;

typedef struct _Kuro_Gfx_Buffer {

} _Kuro_Gfx_Buffer;


Kuro_Gfx
kuro_gfx_create()
{
    HRESULT hr = {};

    Kuro_Gfx gfx = (Kuro_Gfx)malloc(sizeof(_Kuro_Gfx));

    #if defined(DEBUG) || defined(_DEBUG)
    // Enable D3D12 debug layer
    {
        ID3D12Debug *debug_controller;
        hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller));
        assert(SUCCEEDED(hr));
        debug_controller->EnableDebugLayer();
    }
    #endif

    hr = CreateDXGIFactory1(IID_PPV_ARGS(&gfx->factory));
    assert(SUCCEEDED(hr));

    // get adpater with largest dedicated memory
    IDXGIAdapter1 *adapter1 = nullptr;
    IDXGIAdapter4 *adapter4 = nullptr;
    SIZE_T max_dedicated_video_memory = 0;
    for (UINT i = 0; gfx->factory->EnumAdapters1(i, &adapter1) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC1 adapter_desc = {};
        adapter1->GetDesc1(&adapter_desc);
        if ((adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && adapter_desc.DedicatedVideoMemory > max_dedicated_video_memory)
        {
            if (SUCCEEDED(D3D12CreateDevice(adapter1, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
            {
                max_dedicated_video_memory = adapter_desc.DedicatedVideoMemory;
                hr = adapter1->QueryInterface(&adapter4);
                assert(SUCCEEDED(hr));
            }
        }
        adapter1->Release();
    }

    DXGI_ADAPTER_DESC adapter_desc = {};
    adapter4->GetDesc(&adapter_desc);
    OutputDebugString(adapter_desc.Description);
    OutputDebugString(L"\n");

    hr = D3D12CreateDevice(adapter4, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&gfx->device));
    assert(SUCCEEDED(hr));
    adapter4->Release();

    D3D12_COMMAND_QUEUE_DESC queue_desc = {};
    queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    hr = gfx->device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&gfx->command_queue));

    return gfx;
}

void
kuro_gfx_destroy(Kuro_Gfx gfx)
{
    gfx->command_queue->Release();
    gfx->device->Release();
    gfx->factory->Release();
    free(gfx);
}

Kuro_Gfx_Swapchain
kuro_gfx_swapchain_create(Kuro_Gfx gfx, uint32_t width, uint32_t height, void *window_handle)
{
    HRESULT hr = {};

    Kuro_Gfx_Swapchain swapchain = (Kuro_Gfx_Swapchain)malloc(sizeof(_Kuro_Gfx_Swapchain));
    swapchain->backbuffer_format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain->buffer_count = 2;
    swapchain->msaa_state = false;

    if (swapchain->msaa_state)
    {
        // check 4x MSAA quality support
        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS ms_quality_levles = {};
        ms_quality_levles.Format = swapchain->backbuffer_format;
        ms_quality_levles.SampleCount = 4;
        ms_quality_levles.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
        ms_quality_levles.NumQualityLevels = 0;
        hr = gfx->device->CheckFeatureSupport(
            D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
            &ms_quality_levles,
            sizeof(ms_quality_levles));
        assert(SUCCEEDED(hr));
        swapchain->msaa_x4_quality = ms_quality_levles.NumQualityLevels;
        assert(swapchain->msaa_x4_quality > 0 && "unexpected MSAA quality level");
    }

    DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
    swapchain_desc.BufferDesc.Width = width;
    swapchain_desc.BufferDesc.Height = height;
    swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swapchain_desc.BufferDesc.Format = swapchain->backbuffer_format;
    swapchain_desc.SampleDesc.Count = swapchain->msaa_state ? 4 : 1;
    swapchain_desc.SampleDesc.Quality = swapchain->msaa_state ? (swapchain->msaa_x4_quality - 1) : 0;
    swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.BufferCount = swapchain->buffer_count;
    swapchain_desc.OutputWindow = (HWND)window_handle;
    swapchain_desc.Windowed = true;
    swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // swapchain needs the queue so that it can force a flush on it
    hr = gfx->factory->CreateSwapChain(gfx->command_queue, &swapchain_desc, &swapchain->swapchain);
    assert(SUCCEEDED(hr));

    return swapchain;
}

void
kuro_gfx_swapchain_destroy(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain)
{
    gfx = gfx;
    swapchain->swapchain->Release();
    free(swapchain);
}