/*
    TODO[Waleed]:
    * use pool
    * test MSAA
 */

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "kuro/gfx.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <assert.h>

static const int MAX_SWAPCHAIN_BUFFER_COUNT = 3;

typedef struct _kr_gfx_t {
    IDXGIFactory4 *factory;
    ID3D12Device *device;
    ID3D12CommandQueue *command_queue;
    uint32_t rtv_desctiptor_size;
    uint32_t dsv_descriptor_size;
    ID3D12Fence *fence;
    uint64_t current_fence;
} _kr_gfx_t;

typedef struct _kr_swapchain_t {
    DXGI_FORMAT backbuffer_format;
    uint32_t buffer_count;
    bool msaa_state;
    uint32_t msaa_x4_quality;
    IDXGISwapChain3 *swapchain;
    ID3D12Resource *buffers[MAX_SWAPCHAIN_BUFFER_COUNT];
    ID3D12DescriptorHeap *rtv_heap;
    D3D12_CPU_DESCRIPTOR_HANDLE rtv_descriptor[MAX_SWAPCHAIN_BUFFER_COUNT];
} _kr_swapchain_t;

typedef struct _kr_image_t {
    DXGI_FORMAT depth_stencil_format;
    bool msaa_state;
    uint32_t msaa_x4_quality;
    ID3D12Resource *depth_stencil_buffer;
    ID3D12DescriptorHeap *dsv_heap;
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_descriptor;
} _kr_image_t;

typedef struct _kr_buffer_t {
    KURO_GFX_ACCESS cpu_access;
    ID3D12Resource *buffer;
    uint32_t size_in_bytes;
} _kr_buffer_t;

typedef struct _kr_vshader_t {
    ID3DBlob *blob;
} _kr_vshader_t;

typedef struct _kr_pshader_t {
    ID3DBlob *blob;
} _kr_pshader_t;

typedef struct _kr_pipeline_t {
    ID3D12PipelineState *pipeline_state;
    ID3D12RootSignature *root_signature;
    ID3D12DescriptorHeap *cbv_heap;
} _kr_pipeline_t;

typedef struct _kr_pass_t {
    kr_swapchain_t swapchain;
} _kr_pass_t;

typedef struct _kr_commands_t {
    ID3D12CommandAllocator *command_allocator;
    ID3D12GraphicsCommandList *command_list;
} _kr_commands_t;

static inline DXGI_FORMAT
_kuro_gfx_format_to_dx(KURO_GFX_FORMAT format)
{
    switch (format)
    {
        case KURO_GFX_FORMAT_R16_UINT:
            return DXGI_FORMAT_R16_UINT;
        case KURO_GFX_FORMAT_R32_UINT:
            return DXGI_FORMAT_R32_UINT;
        case KURO_GFX_FORMAT_R32G32_FLOAT:
            return DXGI_FORMAT_R32G32_FLOAT;
        case KURO_GFX_FORMAT_R32G32B32_FLOAT:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        default:
            assert(false); return DXGI_FORMAT_UNKNOWN;
    }
}

static inline D3D12_INPUT_CLASSIFICATION
_kuro_gfx_class_to_dx(KURO_GFX_CLASS classification)
{
    switch (classification)
    {
        case KURO_GFX_CLASS_PER_VERTEX:
            return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        case KURO_GFX_CLASS_PER_INSTANCE:
            return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
        default:
            assert(false); return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    }
}

kr_gfx_t
kuro_gfx_create()
{
    HRESULT hr = {};

    kr_gfx_t gfx = (kr_gfx_t)malloc(sizeof(_kr_gfx_t));

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
    OutputDebugStringW(adapter_desc.Description);
    OutputDebugString("\n");

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
kuro_gfx_destroy(kr_gfx_t gfx)
{
    gfx->fence->Release();
    gfx->command_queue->Release();
    gfx->device->Release();
    gfx->factory->Release();
    free(gfx);
}

kr_swapchain_t
kuro_gfx_swapchain_create(kr_gfx_t gfx, uint32_t width, uint32_t height, void *window_handle)
{
    HRESULT hr = {};

    kr_swapchain_t swapchain = (kr_swapchain_t)malloc(sizeof(_kr_swapchain_t));
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

    IDXGISwapChain *swapchain_tmp = nullptr;
    hr = gfx->factory->CreateSwapChain(gfx->command_queue, &swapchain_desc, &swapchain_tmp);
    assert(SUCCEEDED(hr));
    hr = swapchain_tmp->QueryInterface(&swapchain->swapchain);
    assert(SUCCEEDED(hr));
    swapchain_tmp->Release();

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

    for (uint32_t i = 0; i < swapchain->buffer_count; ++i)
    {
        hr = swapchain->swapchain->GetBuffer(i, IID_PPV_ARGS(&swapchain->buffers[i]));
        assert(SUCCEEDED(hr));
        gfx->device->CreateRenderTargetView(
            swapchain->buffers[i],
            nullptr,
            swapchain->rtv_descriptor[i]);
    }

    return swapchain;
}

void
kuro_gfx_swapchain_destroy(kr_gfx_t, kr_swapchain_t swapchain)
{
    swapchain->rtv_heap->Release();
    for (uint32_t i = 0; i < swapchain->buffer_count; ++i)
        swapchain->buffers[i]->Release();
    swapchain->swapchain->Release();
    free(swapchain);
}

void
kuro_gfx_swapchain_resize(kr_gfx_t gfx, kr_swapchain_t swapchain, uint32_t width, uint32_t height)
{
    HRESULT hr = {};

    for (uint32_t i = 0; i < swapchain->buffer_count; ++i)
        swapchain->buffers[i]->Release();

    hr = swapchain->swapchain->ResizeBuffers(
        swapchain->buffer_count,
        width, height,
        swapchain->backbuffer_format,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
    assert(SUCCEEDED(hr));

    for (uint32_t i = 0; i < swapchain->buffer_count; ++i)
    {
        hr = swapchain->swapchain->GetBuffer(i, IID_PPV_ARGS(&swapchain->buffers[i]));
        assert(SUCCEEDED(hr));
        gfx->device->CreateRenderTargetView(
            swapchain->buffers[i],
            nullptr,
            swapchain->rtv_descriptor[i]);
    }
}

void
kuro_gfx_swapchain_present(kr_gfx_t, kr_swapchain_t swapchain)
{
    HRESULT hr = swapchain->swapchain->Present(0, 0);
    assert(SUCCEEDED(hr));
}

kr_image_t
kuro_gfx_image_create(kr_gfx_t gfx, kr_commands_t commands, uint32_t width, uint32_t height)
{
    kr_image_t image = (kr_image_t)malloc(sizeof(_kr_image_t));
    image->depth_stencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    image->msaa_state = false;
    image->msaa_x4_quality = 0;

    HRESULT hr = {};

    D3D12_HEAP_PROPERTIES dsv_heap_properties = {};
    dsv_heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC depth_stencil_desc = {};
    depth_stencil_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depth_stencil_desc.Width = width;
    depth_stencil_desc.Height = height;
    depth_stencil_desc.DepthOrArraySize = 1;
    depth_stencil_desc.MipLevels = 1;
    depth_stencil_desc.Format = image->depth_stencil_format;
    depth_stencil_desc.SampleDesc.Count = image->msaa_state ? 4 : 1;
    depth_stencil_desc.SampleDesc.Quality = image->msaa_state ? (image->msaa_x4_quality - 1) : 0;
    depth_stencil_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optimized_clear_value = {};
    optimized_clear_value.Format = image->depth_stencil_format;
    optimized_clear_value.DepthStencil.Depth = 1.0f;
    optimized_clear_value.DepthStencil.Stencil = 0;

    hr = gfx->device->CreateCommittedResource(
        &dsv_heap_properties,
        D3D12_HEAP_FLAG_NONE,
        &depth_stencil_desc,
        D3D12_RESOURCE_STATE_COMMON,
        &optimized_clear_value,
        IID_PPV_ARGS(&image->depth_stencil_buffer));
    assert(SUCCEEDED(hr));

    D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
    dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsv_heap_desc.NumDescriptors = 1;
    hr = gfx->device->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(&image->dsv_heap));
    assert(SUCCEEDED(hr));

    image->dsv_descriptor = image->dsv_heap->GetCPUDescriptorHandleForHeapStart();

    gfx->device->CreateDepthStencilView(
        image->depth_stencil_buffer,
        nullptr,
        image->dsv_descriptor);

    D3D12_RESOURCE_BARRIER resource_barrier = {};
    resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resource_barrier.Transition.pResource = image->depth_stencil_buffer;
    resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
    commands->command_list->ResourceBarrier(1, &resource_barrier);

    return image;
}

void
kuro_gfx_image_destroy(kr_gfx_t, kr_image_t image)
{
    image->dsv_heap->Release();
    image->depth_stencil_buffer->Release();
    free(image);
}

kr_buffer_t
kuro_gfx_buffer_create(kr_gfx_t gfx, KURO_GFX_ACCESS cpu_access, uint32_t size_in_bytes)
{
    kr_buffer_t buffer = (kr_buffer_t)malloc(sizeof(_kr_buffer_t));

    buffer->cpu_access = cpu_access;
    buffer->buffer = nullptr;
    buffer->size_in_bytes = size_in_bytes;

    HRESULT hr = {};

    D3D12_HEAP_PROPERTIES heap_properties = {};
    switch (buffer->cpu_access)
    {
        case KURO_GFX_ACCESS_NONE:
            heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
            break;
        case KURO_GFX_ACCESS_WRITE:
            heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
            break;
        default:
            assert(false); break;
    }

    D3D12_RESOURCE_STATES initial_state = {};
    switch (buffer->cpu_access)
    {
        case KURO_GFX_ACCESS_NONE:
            initial_state = D3D12_RESOURCE_STATE_COMMON;
            break;
        case KURO_GFX_ACCESS_WRITE:
            initial_state = D3D12_RESOURCE_STATE_GENERIC_READ;
            break;
        default:
            assert(false); break;
    }

    D3D12_RESOURCE_DESC resource_desc = {};
    resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resource_desc.Width = size_in_bytes;
    resource_desc.Height = 1;
    resource_desc.DepthOrArraySize = 1;
    resource_desc.MipLevels = 1;
    resource_desc.SampleDesc.Count = 1;
    resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    hr = gfx->device->CreateCommittedResource(
        &heap_properties,
        D3D12_HEAP_FLAG_NONE,
        &resource_desc,
        initial_state,
        nullptr,
        IID_PPV_ARGS(&buffer->buffer));
    assert(SUCCEEDED(hr));

    return buffer;
}

void
kuro_gfx_buffer_destroy(kr_gfx_t, kr_buffer_t buffer)
{
    buffer->buffer->Release();
    free(buffer);
}

void
kuro_gfx_buffer_write(kr_gfx_t, kr_buffer_t buffer, void *data, uint32_t size_in_bytes)
{
    assert(buffer->cpu_access == KURO_GFX_ACCESS_WRITE);

    HRESULT hr = {};

    void *mapped_data = nullptr;
    hr = buffer->buffer->Map(0, nullptr, &mapped_data);
    assert(SUCCEEDED(hr));
    memcpy(mapped_data, data, size_in_bytes);
    buffer->buffer->Unmap(0, nullptr);
}

kr_vshader_t
kuro_gfx_vertex_shader_create(kr_gfx_t, const char *shader, const char *entry_point)
{
    kr_vshader_t vertex_shader = (kr_vshader_t)malloc(sizeof(_kr_vshader_t));

    UINT compile_flags = 0;
    #if defined(DEBUG) || defined(_DEBUG)
    {
        compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    }
    #endif

    ID3DBlob *error_blob = nullptr;
    HRESULT hr = D3DCompile(shader, ::strlen(shader), nullptr, nullptr, nullptr, entry_point, "vs_5_0", compile_flags, 0, &vertex_shader->blob, &error_blob);
    if (error_blob)
    {
        OutputDebugStringA("vertex shader error: ");
        OutputDebugStringA((char *)error_blob->GetBufferPointer());
        OutputDebugStringA("\n");
        error_blob->Release();
    }
    assert(SUCCEEDED(hr));

    return vertex_shader;
}

void
kuro_gfx_vertex_shader_destroy(kr_gfx_t, kr_vshader_t vertex_shader)
{
    vertex_shader->blob->Release();
    free(vertex_shader);
}

kr_pshader_t
kuro_gfx_pixel_shader_create(kr_gfx_t, const char *shader, const char *entry_point)
{
    kr_pshader_t pixel_shader = (kr_pshader_t)malloc(sizeof(_kr_pshader_t));

    UINT compile_flags = 0;
    #if defined(DEBUG) || defined(_DEBUG)
    {
        compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    }
    #endif

    ID3DBlob *error_blob = nullptr;
    HRESULT hr = D3DCompile(shader, ::strlen(shader), nullptr, nullptr, nullptr, entry_point, "ps_5_0", compile_flags, 0, &pixel_shader->blob, &error_blob);
    if (error_blob)
    {
        OutputDebugStringA("pixel shader error: ");
        OutputDebugStringA((char *)error_blob->GetBufferPointer());
        OutputDebugStringA("\n");
        error_blob->Release();
    }
    assert(SUCCEEDED(hr));

    return pixel_shader;
}

void
kuro_gfx_pixel_shader_destroy(kr_gfx_t, kr_pshader_t pixel_shader)
{
    pixel_shader->blob->Release();
    free(pixel_shader);
}

kr_pipeline_t
kuro_gfx_pipeline_create(kr_gfx_t gfx, Kuro_Gfx_Pipeline_Desc desc)
{
    assert(desc.vertex_shader);

    kr_pipeline_t pipeline = (kr_pipeline_t)malloc(sizeof(_kr_pipeline_t));

    HRESULT hr = {};

    D3D12_DESCRIPTOR_RANGE descriptor_range = {};
    descriptor_range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    descriptor_range.NumDescriptors = 1;
    descriptor_range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    D3D12_ROOT_PARAMETER root_parameter = {};
    root_parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    root_parameter.DescriptorTable.NumDescriptorRanges = 1;
    root_parameter.DescriptorTable.pDescriptorRanges = &descriptor_range;
    root_parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {};
    root_signature_desc.NumParameters = 1;
    root_signature_desc.pParameters = &root_parameter;
    root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob *signature_blob = nullptr;
    ID3DBlob *error_blob = nullptr;
    hr = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature_blob, &error_blob);
    if (FAILED(hr))
    {
        OutputDebugStringA((char *)error_blob->GetBufferPointer());
        OutputDebugStringA("\n");
        assert(false);
    }
    hr = gfx->device->CreateRootSignature(0, signature_blob->GetBufferPointer(), signature_blob->GetBufferSize(), IID_PPV_ARGS(&pipeline->root_signature));
    assert(SUCCEEDED(hr));
    signature_blob->Release();

    ID3D12ShaderReflection *reflection = nullptr;
    hr = D3DReflect(desc.vertex_shader->blob->GetBufferPointer(), desc.vertex_shader->blob->GetBufferSize(), IID_PPV_ARGS(&reflection));
    assert(SUCCEEDED(hr));

    D3D12_SHADER_DESC shader_desc = {};
    reflection->GetDesc(&shader_desc);

    if (shader_desc.ConstantBuffers > 0)
    {
        D3D12_DESCRIPTOR_HEAP_DESC cbv_heap_desc = {};
        cbv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbv_heap_desc.NumDescriptors = shader_desc.ConstantBuffers;
        cbv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        hr = gfx->device->CreateDescriptorHeap(&cbv_heap_desc, IID_PPV_ARGS(&pipeline->cbv_heap));
        assert(SUCCEEDED(hr));
    }

    D3D12_INPUT_ELEMENT_DESC *input_element_desc = (D3D12_INPUT_ELEMENT_DESC *)calloc(shader_desc.InputParameters, sizeof(D3D12_INPUT_ELEMENT_DESC));
    for (uint32_t i = 0; i < shader_desc.InputParameters; ++i)
    {
        D3D12_SIGNATURE_PARAMETER_DESC parameter_desc = {};
        hr = reflection->GetInputParameterDesc(i, &parameter_desc);

        input_element_desc[i].SemanticName = parameter_desc.SemanticName;
        input_element_desc[i].SemanticIndex = parameter_desc.SemanticIndex;
        input_element_desc[i].Format = _kuro_gfx_format_to_dx(desc.vertex_attribures[i].format);
        input_element_desc[i].InputSlot = desc.vertex_attribures[i].slot;
        input_element_desc[i].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
        input_element_desc[i].InputSlotClass = _kuro_gfx_class_to_dx(desc.vertex_attribures[i].classification);
        input_element_desc[i].InstanceDataStepRate = 0;
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_desc = {};
    pipeline_desc.pRootSignature = pipeline->root_signature;
    pipeline_desc.VS.pShaderBytecode = desc.vertex_shader->blob->GetBufferPointer();
    pipeline_desc.VS.BytecodeLength = desc.vertex_shader->blob->GetBufferSize();
    if (desc.pixel_shader)
    {
        pipeline_desc.PS.pShaderBytecode = desc.pixel_shader->blob->GetBufferPointer();
        pipeline_desc.PS.BytecodeLength = desc.pixel_shader->blob->GetBufferSize();
    }
    for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    {
        pipeline_desc.BlendState.RenderTarget[i].BlendEnable = FALSE;
        pipeline_desc.BlendState.RenderTarget[i].LogicOpEnable = FALSE;
        pipeline_desc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
        pipeline_desc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
        pipeline_desc.BlendState.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
        pipeline_desc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
        pipeline_desc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
        pipeline_desc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        pipeline_desc.BlendState.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
        pipeline_desc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    }
    pipeline_desc.SampleMask = UINT_MAX;
    pipeline_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    pipeline_desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    pipeline_desc.RasterizerState.FrontCounterClockwise = TRUE;
    pipeline_desc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    pipeline_desc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    pipeline_desc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    pipeline_desc.RasterizerState.DepthClipEnable = TRUE;
    pipeline_desc.RasterizerState.MultisampleEnable = FALSE;
    pipeline_desc.RasterizerState.AntialiasedLineEnable = FALSE;
    pipeline_desc.RasterizerState.ForcedSampleCount = 0;
    pipeline_desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    pipeline_desc.DepthStencilState.DepthEnable = FALSE;
    pipeline_desc.DepthStencilState.StencilEnable = FALSE;
    pipeline_desc.InputLayout.pInputElementDescs = input_element_desc;
    pipeline_desc.InputLayout.NumElements = shader_desc.InputParameters;
    pipeline_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipeline_desc.NumRenderTargets = 1;
    pipeline_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pipeline_desc.SampleDesc.Count = 1;
    hr = gfx->device->CreateGraphicsPipelineState(&pipeline_desc, IID_PPV_ARGS(&pipeline->pipeline_state));
    assert(SUCCEEDED(hr));

    reflection->Release();
    free(input_element_desc);

    return pipeline;
}

void
kuro_gfx_pipeline_destroy(kr_gfx_t, kr_pipeline_t pipeline)
{
    pipeline->pipeline_state->Release();
    pipeline->root_signature->Release();
    pipeline->cbv_heap->Release();
}

void
kuro_gfx_pipeline_set_constant_buffer(kr_gfx_t gfx, kr_pipeline_t pipeline, kr_buffer_t buffer, uint32_t slot)
{
    assert(buffer->size_in_bytes % 256 == 0);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
    cbv_desc.BufferLocation = buffer->buffer->GetGPUVirtualAddress();
    cbv_desc.SizeInBytes = buffer->size_in_bytes;

    // TODO[Waleed]: get cbv_descriptor size
    D3D12_CPU_DESCRIPTOR_HANDLE cbv_descreptor = pipeline->cbv_heap->GetCPUDescriptorHandleForHeapStart();
    cbv_descreptor.ptr += (slot * gfx->rtv_desctiptor_size);
    gfx->device->CreateConstantBufferView(&cbv_desc, cbv_descreptor);
}

kr_pass_t
kuro_gfx_pass_from_swapchain(kr_gfx_t, kr_swapchain_t swapchain)
{
    kr_pass_t pass = (kr_pass_t)malloc(sizeof(_kr_pass_t));
    pass->swapchain = swapchain;
    return pass;
}

void
kuro_gfx_pass_free(kr_gfx_t, kr_pass_t pass)
{
    free(pass);
}

kr_commands_t
kuro_gfx_commands_create(kr_gfx_t gfx)
{
    HRESULT hr = {};

    kr_commands_t commands = (kr_commands_t)malloc(sizeof(_kr_commands_t));

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
kuro_gfx_commands_destroy(kr_gfx_t, kr_commands_t commands)
{
    commands->command_list->Release();
    commands->command_allocator->Release();
}

void
kuro_gfx_commands_begin(kr_gfx_t, kr_commands_t commands)
{
    HRESULT hr = commands->command_allocator->Reset();
    assert(SUCCEEDED(hr));

    hr = commands->command_list->Reset(commands->command_allocator, nullptr);
    assert(SUCCEEDED(hr));
}

void
kuro_gfx_commands_end(kr_gfx_t gfx, kr_commands_t commands)
{
    HRESULT hr = commands->command_list->Close();
    assert(SUCCEEDED(hr));

    ID3D12CommandList *cmd_lists[] = { commands->command_list };
    gfx->command_queue->ExecuteCommandLists(1, cmd_lists);
}


void
kuro_gfx_commands_buffer_copy(kr_commands_t commands, kr_buffer_t src_buffer, kr_buffer_t dst_buffer)
{
    // TODO[Waleed]: cpu access doesn't matter here, we should check the state instead
    assert(src_buffer->cpu_access == KURO_GFX_ACCESS_WRITE);
    assert(dst_buffer->cpu_access == KURO_GFX_ACCESS_NONE);

    D3D12_RESOURCE_BARRIER resource_barrier = {};
    resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resource_barrier.Transition.pResource = dst_buffer->buffer;
    resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
    commands->command_list->ResourceBarrier(1, &resource_barrier);

    commands->command_list->CopyBufferRegion(dst_buffer->buffer, 0, src_buffer->buffer, 0, src_buffer->size_in_bytes);

    resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
    commands->command_list->ResourceBarrier(1, &resource_barrier);
}

void
kuro_gfx_commands_set_pipeline(kr_commands_t commands, kr_pipeline_t pipeline)
{
    commands->command_list->SetPipelineState(pipeline->pipeline_state);
    commands->command_list->SetGraphicsRootSignature(pipeline->root_signature);

    ID3D12DescriptorHeap *descriptor_heaps[] = {pipeline->cbv_heap};
    commands->command_list->SetDescriptorHeaps(1, descriptor_heaps);
    commands->command_list->SetGraphicsRootDescriptorTable(0, pipeline->cbv_heap->GetGPUDescriptorHandleForHeapStart());
}

void
kuro_gfx_commands_pass_begin(kr_commands_t commands, kr_pass_t pass, kr_image_t depth_target)
{
    D3D12_RESOURCE_BARRIER resource_barrier = {};
    resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resource_barrier.Transition.pResource = pass->swapchain->buffers[pass->swapchain->swapchain->GetCurrentBackBufferIndex()];
    resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    commands->command_list->ResourceBarrier(1, &resource_barrier);

    commands->command_list->OMSetRenderTargets(
        1,
        &pass->swapchain->rtv_descriptor[pass->swapchain->swapchain->GetCurrentBackBufferIndex()],
        true,
        depth_target ? &depth_target->dsv_descriptor : nullptr);
}

void
kuro_gfx_commands_pass_end(kr_commands_t commands, kr_pass_t pass)
{
    D3D12_RESOURCE_BARRIER resource_barrier = {};
    resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resource_barrier.Transition.pResource = pass->swapchain->buffers[pass->swapchain->swapchain->GetCurrentBackBufferIndex()];
    resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    commands->command_list->ResourceBarrier(1, &resource_barrier);
}

void
kuro_gfx_commands_viewport(kr_commands_t commands, uint32_t width, uint32_t height)
{
    D3D12_VIEWPORT viewport = {};
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MaxDepth = 1;
    commands->command_list->RSSetViewports(1, &viewport);

    RECT scissor_rect = {};
    scissor_rect.right = width;
    scissor_rect.bottom = height;
    commands->command_list->RSSetScissorRects(1, &scissor_rect);
}

void
kuro_gfx_commands_clear_color(kr_commands_t commands, kr_pass_t pass, Kuro_Gfx_Color color)
{
    commands->command_list->ClearRenderTargetView(pass->swapchain->rtv_descriptor[pass->swapchain->swapchain->GetCurrentBackBufferIndex()], &color.r, 0, nullptr);
}

void
kuro_gfx_commands_clear_depth(kr_commands_t commands, kr_image_t depth_target, float value)
{
    commands->command_list->ClearDepthStencilView(depth_target->dsv_descriptor, D3D12_CLEAR_FLAG_DEPTH, value, 0, 0, nullptr);
}

void
kuro_gfx_commands_draw(kr_commands_t commands, Kuro_Gfx_Draw_Desc desc)
{
    commands->command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D12_VERTEX_BUFFER_VIEW vertex_buffer_views[KURO_CONSTANT_MAX_VERTEX_ATTRIPUTES] = {};
    for (uint32_t i = 0; i < KURO_CONSTANT_MAX_VERTEX_ATTRIPUTES; ++i)
    {
        kr_buffer_t vertex_buffer = desc.vertex_buffers[i].buffer;
        if (vertex_buffer == nullptr)
            continue;

        vertex_buffer_views[i].BufferLocation = vertex_buffer->buffer->GetGPUVirtualAddress();
        vertex_buffer_views[i].SizeInBytes = vertex_buffer->size_in_bytes;
        vertex_buffer_views[i].StrideInBytes = desc.vertex_buffers[i].stride;
    }
    commands->command_list->IASetVertexBuffers(0, KURO_CONSTANT_MAX_VERTEX_ATTRIPUTES, vertex_buffer_views);

    if (desc.index_buffer.buffer)
    {
        assert(desc.index_buffer.format == KURO_GFX_FORMAT_R16_UINT || desc.index_buffer.format == KURO_GFX_FORMAT_R32_UINT);

        D3D12_INDEX_BUFFER_VIEW index_buffer_view = {};
        index_buffer_view.BufferLocation = desc.index_buffer.buffer->buffer->GetGPUVirtualAddress();
        index_buffer_view.SizeInBytes = desc.index_buffer.buffer->size_in_bytes;
        index_buffer_view.Format = _kuro_gfx_format_to_dx(desc.index_buffer.format);
        commands->command_list->IASetIndexBuffer(&index_buffer_view);

        commands->command_list->DrawIndexedInstanced(desc.count, 1, 0, 0, 0);
    }
    else
    {
        commands->command_list->DrawInstanced(desc.count, 1, 0, 0);
    }
}

void
kuro_gfx_sync(kr_gfx_t gfx)
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