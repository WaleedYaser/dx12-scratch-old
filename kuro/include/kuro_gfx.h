#pragma once

#include <stdint.h>
#include <stdbool.h>

#define KURO_GFX_API

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Kuro_Gfx *Kuro_Gfx;
typedef struct _Kuro_Gfx_Swapchain *Kuro_Gfx_Swapchain;
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

KURO_GFX_API Kuro_Gfx_Commands kuro_gfx_commands_create(Kuro_Gfx gfx);
KURO_GFX_API void kuro_gfx_commands_destroy(Kuro_Gfx gfx, Kuro_Gfx_Commands commands);
KURO_GFX_API void kuro_gfx_commands_begin(Kuro_Gfx gfx, Kuro_Gfx_Commands commands);
KURO_GFX_API void kuro_gfx_commands_end(Kuro_Gfx gfx, Kuro_Gfx_Commands commands);
KURO_GFX_API void kuro_gfx_commands_swapchain_begin(Kuro_Gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Swapchain swapchain);
KURO_GFX_API void kuro_gfx_commands_swapchain_end(Kuro_Gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Swapchain swapchain);
KURO_GFX_API void kuro_gfx_commands_swapchain_clear(Kuro_Gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Swapchain swapchain, Kuro_Gfx_Color color);

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

static const int MAX_SWAPCHAIN_BUFFER_COUNT = 3;

typedef struct _Kuro_Gfx {
    IDXGIFactory4 *factory;
    ID3D12Device *device;
    ID3D12CommandQueue *command_queue;
    uint32_t rtv_desctiptor_size;
    uint32_t dsv_descriptor_size;
    ID3D12Fence *fence;
    uint64_t current_fence;
} _Kuro_Gfx;

typedef struct _Kuro_Gfx_Swapchain {
    DXGI_FORMAT backbuffer_format;
    DXGI_FORMAT depth_stencil_format;
    uint32_t buffer_count;
    uint32_t current_buffer_index;
    bool msaa_state;
    uint32_t msaa_x4_quality;
    IDXGISwapChain *swapchain;
    ID3D12Resource *buffers[MAX_SWAPCHAIN_BUFFER_COUNT];
    ID3D12Resource *depth_stencil_buffer;
    ID3D12DescriptorHeap *rtv_heap;
    ID3D12DescriptorHeap *dsv_heap;
    D3D12_CPU_DESCRIPTOR_HANDLE rtv_descriptor[MAX_SWAPCHAIN_BUFFER_COUNT];
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_descriptor;
} _Kuro_Gfx_Swapchain;

typedef struct _Kuro_Gfx_Commands {
    ID3D12CommandAllocator *command_allocator;
    ID3D12GraphicsCommandList *command_list;
} _Kuro_Gfx_Commands;

static inline void
_kuro_gfx_swapchain_init(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain, uint32_t width, uint32_t height)
{
    HRESULT hr = {};

    swapchain->current_buffer_index = 0;

    for (uint32_t i = 0; i < swapchain->buffer_count; ++i)
    {
        hr = swapchain->swapchain->GetBuffer(i, IID_PPV_ARGS(&swapchain->buffers[i]));
        assert(SUCCEEDED(hr));
        gfx->device->CreateRenderTargetView(
            swapchain->buffers[i],
            nullptr,
            swapchain->rtv_descriptor[i]);
    }

    D3D12_HEAP_PROPERTIES dsv_heap_properties = {};
    dsv_heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC depth_stencil_desc = {};
    depth_stencil_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depth_stencil_desc.Width = width;
    depth_stencil_desc.Height = height;
    depth_stencil_desc.DepthOrArraySize = 1;
    depth_stencil_desc.MipLevels = 1;
    depth_stencil_desc.Format = swapchain->depth_stencil_format;
    depth_stencil_desc.SampleDesc.Count = swapchain->msaa_state ? 4 : 1;
    depth_stencil_desc.SampleDesc.Quality = swapchain->msaa_state ? (swapchain->msaa_x4_quality - 1) : 0;
    depth_stencil_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optimized_clear_value = {};
    optimized_clear_value.Format = swapchain->depth_stencil_format;
    optimized_clear_value.DepthStencil.Depth = 1.0f;
    optimized_clear_value.DepthStencil.Stencil = 0;

    hr = gfx->device->CreateCommittedResource(
        &dsv_heap_properties,
        D3D12_HEAP_FLAG_NONE,
        &depth_stencil_desc,
        D3D12_RESOURCE_STATE_COMMON,
        &optimized_clear_value,
        IID_PPV_ARGS(&swapchain->depth_stencil_buffer));
    assert(SUCCEEDED(hr));

    gfx->device->CreateDepthStencilView(
        swapchain->depth_stencil_buffer,
        nullptr,
        swapchain->dsv_descriptor);
}


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
    assert(SUCCEEDED(hr));

    // query descriptor size as it can vary accross GPUs
    gfx->rtv_desctiptor_size =
        gfx->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    gfx->dsv_descriptor_size =
        gfx->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

    hr = gfx->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&gfx->fence));
    assert(SUCCEEDED(hr));
    gfx->current_fence = 0;

    return gfx;
}

void
kuro_gfx_destroy(Kuro_Gfx gfx)
{
    gfx->fence->Release();
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
    swapchain->depth_stencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
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

    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
    rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtv_heap_desc.NumDescriptors = swapchain->buffer_count;
    hr = gfx->device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&swapchain->rtv_heap));
    assert(SUCCEEDED(hr));

    for (uint32_t i = 0; i < swapchain->buffer_count; ++i)
    {
        swapchain->rtv_descriptor[i] = swapchain->rtv_heap->GetCPUDescriptorHandleForHeapStart();
        swapchain->rtv_descriptor[i].ptr += (i * gfx->rtv_desctiptor_size);
    }

    D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
    dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsv_heap_desc.NumDescriptors = 1;
    hr = gfx->device->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(&swapchain->dsv_heap));
    assert(SUCCEEDED(hr));

    swapchain->dsv_descriptor = swapchain->dsv_heap->GetCPUDescriptorHandleForHeapStart();

    _kuro_gfx_swapchain_init(gfx, swapchain, width, height);

    return swapchain;
}

void
kuro_gfx_swapchain_destroy(Kuro_Gfx, Kuro_Gfx_Swapchain swapchain)
{
    swapchain->dsv_heap->Release();
    swapchain->rtv_heap->Release();
    swapchain->depth_stencil_buffer->Release();
    for (uint32_t i = 0; i < swapchain->buffer_count; ++i)
        swapchain->buffers[i]->Release();
    swapchain->swapchain->Release();
    free(swapchain);
}

void
kuro_gfx_swapchain_resize(Kuro_Gfx gfx, Kuro_Gfx_Swapchain swapchain, uint32_t width, uint32_t height)
{
    HRESULT hr = {};

    swapchain->depth_stencil_buffer->Release();
    for (uint32_t i = 0; i < swapchain->buffer_count; ++i)
        swapchain->buffers[i]->Release();

    hr = swapchain->swapchain->ResizeBuffers(
        swapchain->buffer_count,
        width, height,
        swapchain->backbuffer_format,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
    assert(SUCCEEDED(hr));

    _kuro_gfx_swapchain_init(gfx, swapchain, width, height);
}

void
kuro_gfx_swapchain_present(Kuro_Gfx, Kuro_Gfx_Swapchain swapchain)
{
    HRESULT hr = swapchain->swapchain->Present(0, 0);
    assert(SUCCEEDED(hr));
    swapchain->current_buffer_index = (swapchain->current_buffer_index + 1) % swapchain->buffer_count;
}

Kuro_Gfx_Commands
kuro_gfx_commands_create(Kuro_Gfx gfx)
{
    HRESULT hr = {};

    Kuro_Gfx_Commands commands = (Kuro_Gfx_Commands)malloc(sizeof(_Kuro_Gfx_Commands));

    hr = gfx->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commands->command_allocator));
    assert(SUCCEEDED(hr));

    hr = gfx->device->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        commands->command_allocator,
        nullptr,
        IID_PPV_ARGS(&commands->command_list));
    assert(SUCCEEDED(hr));

    hr = commands->command_list->Close();
    assert(SUCCEEDED(hr));

    return commands;
}

void
kuro_gfx_commands_destroy(Kuro_Gfx, Kuro_Gfx_Commands commands)
{
    commands->command_list->Release();
    commands->command_allocator->Release();
}

void
kuro_gfx_commands_begin(Kuro_Gfx, Kuro_Gfx_Commands commands)
{
    HRESULT hr = commands->command_allocator->Reset();
    assert(SUCCEEDED(hr));

    hr = commands->command_list->Reset(commands->command_allocator, nullptr);
    assert(SUCCEEDED(hr));
}

void
kuro_gfx_commands_end(Kuro_Gfx gfx, Kuro_Gfx_Commands commands)
{
    HRESULT hr = commands->command_list->Close();
    assert(SUCCEEDED(hr));

    ID3D12CommandList *cmd_lists[] = { commands->command_list };
    gfx->command_queue->ExecuteCommandLists(1, cmd_lists);
}

void
kuro_gfx_commands_swapchain_begin(Kuro_Gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Swapchain swapchain)
{
    D3D12_RESOURCE_BARRIER resource_barrier = {};
    resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resource_barrier.Transition.pResource = swapchain->buffers[swapchain->current_buffer_index];
    resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    commands->command_list->ResourceBarrier(1, &resource_barrier);

    commands->command_list->OMSetRenderTargets(
        1,
        &swapchain->rtv_descriptor[swapchain->current_buffer_index],
        true,
        nullptr);
}

void
kuro_gfx_commands_swapchain_end(Kuro_Gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Swapchain swapchain)
{
    D3D12_RESOURCE_BARRIER resource_barrier = {};
    resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resource_barrier.Transition.pResource = swapchain->buffers[swapchain->current_buffer_index];
    resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    commands->command_list->ResourceBarrier(1, &resource_barrier);
}

void
kuro_gfx_commands_swapchain_clear(Kuro_Gfx, Kuro_Gfx_Commands commands, Kuro_Gfx_Swapchain swapchain, Kuro_Gfx_Color color)
{
    commands->command_list->ClearRenderTargetView(swapchain->rtv_descriptor[swapchain->current_buffer_index], &color.r, 0, nullptr);
}

void
kuro_gfx_flush(Kuro_Gfx gfx)
{
    // advance fence value to mark commands up to this fence point
    gfx->current_fence++;

    // add an instruction to command queue to set a new fence point, because we
    // are on GPU timeline, the new fence point won't be set until GPU finishes
    // processing all commands prior to Signal()
    gfx->command_queue->Signal(gfx->fence, gfx->current_fence);

    // wait until GPU has completed commands up to this fence point
    if (gfx->fence->GetCompletedValue() < gfx->current_fence)
    {
        HANDLE event_handle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

        // fire event when GPU hits current fence
        HRESULT hr = gfx->fence->SetEventOnCompletion(gfx->current_fence, event_handle);
        assert(SUCCEEDED(hr));

        // wait until GPU hits current fence event is fired
        WaitForSingleObject(event_handle, INFINITE);
        CloseHandle(event_handle);
    }
}