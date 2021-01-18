#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D12.lib")

#include "kuro_gfx.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <assert.h>

namespace kuro
{
    static constexpr i32 _swapchain_buffer_count = 2;

    struct _Swapchain
    {
        IDXGISwapChain *swapchain;
        ID3D12Resource *swapchain_buffers[_swapchain_buffer_count];
        ID3D12Resource *depth_stencil_buffer;
    };

    struct _Gfx
    {
        IDXGIFactory4 *factory;
        ID3D12Device *device;

        ID3D12Fence *fence;
        u64 current_fence;

        u32 rtv_descriptor_size;
        u32 dsv_descriptor_size;
        u32 cbv_srv_descriptor_size;

        DXGI_FORMAT backbuffer_format;
        DXGI_FORMAT depth_stencil_format;

        bool msaa_state;
        u32 msaa_x4_quality;

        ID3D12CommandQueue *command_queue;
        ID3D12CommandAllocator *command_allocator;
        ID3D12GraphicsCommandList *command_list;

        ID3D12DescriptorHeap *rtv_heap;
        ID3D12DescriptorHeap *dsv_heap;

        i32 current_back_buffer;
    };

    void
    _gfx_command_queue_flush(Gfx self)
    {
        // advance fence value to mark commands up to this fence point
        self->current_fence++;

        // add an instruction to command queue to set a new fence point, because we
        // are on GPU timeline, the new fence point won't be set until GPU finishes
        // processing all commands prior to Signal()
        self->command_queue->Signal(self->fence, self->current_fence);

        // wait until GPU has completed commands up to this fence point
        if (self->fence->GetCompletedValue() < self->current_fence)
        {
            HANDLE event_handle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

            // fire event when GPU hits current fence
            HRESULT hr = self->fence->SetEventOnCompletion(self->current_fence, event_handle);
            assert(SUCCEEDED(hr));

            // wait until GPU hits current fence event is fired
            WaitForSingleObject(event_handle, INFINITE);
            CloseHandle(event_handle);
        }
    }

    // API

    Gfx
    gfx_init()
    {
        // TODO[Waleed]: allocate on heap
        static _Gfx self = {};

        HRESULT hr = {};

        #if defined(DEBUG) || defined(_DEBUG)
        // Enable D3D12 debug layer
        {
            ID3D12Debug *debug_controller;
            hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller));
            assert(SUCCEEDED(hr));
            debug_controller->EnableDebugLayer();
        }
        #endif

        hr = CreateDXGIFactory1(IID_PPV_ARGS(&self.factory));
        assert(SUCCEEDED(hr));

        // TODO[Waleed]: enumerate all the systems display adapters

        // try to create hardware device
        hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&self.device));

        // fallback to WARP device
        if (FAILED(hr))
        {
            IDXGIAdapter *warp_adapter = nullptr;
            hr = self.factory->EnumWarpAdapter(IID_PPV_ARGS(&warp_adapter));
            assert(SUCCEEDED(hr));

            hr = D3D12CreateDevice(warp_adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&self.device));
            assert(SUCCEEDED(hr));
        }

        // create fence for CPU/GPU sync
        hr = self.device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&self.fence));
        assert(SUCCEEDED(hr));

        // query descriptor size as it can vary accross GPUs
        self.rtv_descriptor_size =
            self.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        self.dsv_descriptor_size =
            self.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        self.cbv_srv_descriptor_size =
            self.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        // set backbuffer and depth stencil format
        self.backbuffer_format = DXGI_FORMAT_R8G8B8A8_UNORM;
        self.depth_stencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

        // check 4x MSAA quality support
        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS ms_quality_levles = {};
        ms_quality_levles.Format = self.backbuffer_format;
        ms_quality_levles.SampleCount = 4;
        ms_quality_levles.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
        ms_quality_levles.NumQualityLevels = 0;
        hr = self.device->CheckFeatureSupport(
            D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
            &ms_quality_levles,
            sizeof(ms_quality_levles));
        assert(SUCCEEDED(hr));
        self.msaa_x4_quality = ms_quality_levles.NumQualityLevels;
        assert(self.msaa_x4_quality > 0 && "unexpected MSAA quality level");

        // create command queue and command list
        D3D12_COMMAND_QUEUE_DESC queue_desc = {};
        queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        hr = self.device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&self.command_queue));
        assert(SUCCEEDED(hr));

        hr = self.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&self.command_allocator));
        assert(SUCCEEDED(hr));

        hr = self.device->CreateCommandList(
            0,
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            self.command_allocator,
            nullptr,
            IID_PPV_ARGS(&self.command_list));
        assert(SUCCEEDED(hr));

        // start off in a closed state, because the first time we refer to reset it and it needs to
        // be closed before calling reset.
        self.command_list->Close();

        // create descriptor heaps
        D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
        rtv_heap_desc.NumDescriptors = _swapchain_buffer_count;
        rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        hr = self.device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&self.rtv_heap));
        assert(SUCCEEDED(hr));

        D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
        dsv_heap_desc.NumDescriptors = 1;
        dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        hr = self.device->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(&self.dsv_heap));
        assert(SUCCEEDED(hr));

        return &self;
    }

    void
    gfx_deinit(Gfx self)
    {
        self->dsv_heap->Release();
        self->rtv_heap->Release();
        self->command_list->Release();
        self->command_allocator->Release();
        self->command_queue->Release();
        self->fence->Release();
        self->device->Release();
        self->factory->Release();
    }

    Swapchain
    gfx_swapchain_new(Gfx self, void *window, u32 width, u32 height)
    {
        // TODO[Waleed]: allocate on heap
        static _Swapchain swapchain = {};

        DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
        swapchain_desc.BufferDesc.Width = width;
        swapchain_desc.BufferDesc.Height = height;
        swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
        swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
        swapchain_desc.BufferDesc.Format = self->backbuffer_format;
        swapchain_desc.SampleDesc.Count = self->msaa_state ? 4 : 1;
        swapchain_desc.SampleDesc.Quality = self->msaa_state ? (self->msaa_x4_quality - 1) : 0;
        swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchain_desc.BufferCount = _swapchain_buffer_count;
        swapchain_desc.OutputWindow = (HWND)window;
        swapchain_desc.Windowed = true;
        swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        // NOTE: swapchain uses queue to perform flush
        HRESULT hr = self->factory->CreateSwapChain(self->command_queue, &swapchain_desc, &swapchain.swapchain);
        assert(SUCCEEDED(hr));

        gfx_swapchain_resize(self, &swapchain, width, height);

        return &swapchain;
    }

    void
    gfx_swapchain_free(Gfx self, Swapchain swapchain)
    {
        (self);
        swapchain->depth_stencil_buffer->Release();
        for (int i = 0; i < _swapchain_buffer_count; ++i)
        {
            swapchain->swapchain_buffers[i]->Release();
        }
        swapchain->swapchain->Release();
    }

    void
    gfx_swapchain_resize(Gfx self, Swapchain swapchain, u32 width, u32 height)
    {
        assert(self->device);
        assert(swapchain->swapchain);
        assert(self->command_allocator);

        // flush before changing any resource
        _gfx_command_queue_flush(self);

        HRESULT hr = self->command_list->Reset(self->command_allocator, nullptr);
        assert(SUCCEEDED(hr));

        // release previous resources we will create
        for (int i = 0; i < _swapchain_buffer_count; ++i)
            if (swapchain->swapchain_buffers[i]) swapchain->swapchain_buffers[i]->Release();
        if (swapchain->depth_stencil_buffer) swapchain->depth_stencil_buffer->Release();

        // create render target view
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_heap_handle = self->rtv_heap->GetCPUDescriptorHandleForHeapStart();
        for (u32 i = 0; i < _swapchain_buffer_count; ++i)
        {
            hr = swapchain->swapchain->GetBuffer(i, IID_PPV_ARGS(&swapchain->swapchain_buffers[i]));
            assert(SUCCEEDED(hr));
            self->device->CreateRenderTargetView(swapchain->swapchain_buffers[i], nullptr, rtv_heap_handle);
            rtv_heap_handle.ptr += self->rtv_descriptor_size;
        }

        // create depth stencil buffer
        D3D12_HEAP_PROPERTIES dsv_heap_properties = {};
        dsv_heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
        dsv_heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        dsv_heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

        D3D12_RESOURCE_DESC depth_stencil_desc = {};
        depth_stencil_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depth_stencil_desc.Width = width;
        depth_stencil_desc.Height = height;
        depth_stencil_desc.DepthOrArraySize = 1;
        depth_stencil_desc.MipLevels = 1;
        depth_stencil_desc.Format = self->depth_stencil_format;
        depth_stencil_desc.SampleDesc.Count = self->msaa_state ? 4 : 1;
        depth_stencil_desc.SampleDesc.Quality = self->msaa_state ? (self->msaa_x4_quality - 1) : 0;
        depth_stencil_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE optimized_clear_value = {};
        optimized_clear_value.Format = self->depth_stencil_format;
        optimized_clear_value.DepthStencil.Depth = 1.0f;
        optimized_clear_value.DepthStencil.Stencil = 0;

        hr = self->device->CreateCommittedResource(
            &dsv_heap_properties,
            D3D12_HEAP_FLAG_NONE,
            &depth_stencil_desc,
            D3D12_RESOURCE_STATE_COMMON,
            &optimized_clear_value,
            IID_PPV_ARGS(&swapchain->depth_stencil_buffer));
        assert(SUCCEEDED(hr));

        // create depth stencil view
        self->device->CreateDepthStencilView(
            swapchain->depth_stencil_buffer,
            nullptr,
            self->dsv_heap->GetCPUDescriptorHandleForHeapStart());

        // transition resource from initial state to be usec as a depth buffer
        D3D12_RESOURCE_BARRIER resource_barrier = {};
        resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        resource_barrier.Transition.pResource = swapchain->depth_stencil_buffer;
        resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
        resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
        self->command_list->ResourceBarrier(1, &resource_barrier);

        // close command list
        hr = self->command_list->Close();
        assert(SUCCEEDED(hr));

        // execute commands
        ID3D12CommandList *cmd_lists[] = {self->command_list};
        self->command_queue->ExecuteCommandLists(1, cmd_lists);

        // wait until creation is completed
        _gfx_command_queue_flush(self);
    }
}