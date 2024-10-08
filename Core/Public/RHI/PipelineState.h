﻿// copyright

#pragma once

#include <wrl/client.h>  // Include for Microsoft::WRL::ComPtr
#include <d3d11.h>       // Include for D3D11 types
#include <unordered_map> // Include for std::unordered_map
#include <vector>        // Include for std::vector
#include <memory>        // Include for std::unique_ptr

using Microsoft::WRL::ComPtr;

struct PipelineState
{
    // constexpr values for D3D11 limits
    static constexpr UINT MaxSRVs = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; // 128
    static constexpr UINT MaxSamplers = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;    // 16
    static constexpr UINT MaxConstantBuffers = D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; // 14
    static constexpr UINT MaxRenderTargets = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; // 8
    static constexpr UINT MaxUAVs = D3D11_PS_CS_UAV_REGISTER_COUNT; // 8
    static constexpr UINT MaxViewportsAndScissorRects = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE; // 16
    static constexpr UINT MaxVertexBuffers = D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; // 32 

    const TCHAR* Name = TEXT("(None)");

#pragma region(Input Assembler)

    // Input Layout
    ComPtr<ID3D11InputLayout> InputLayout;

    // Vertex Buffers
    std::unordered_map<UINT, ComPtr<ID3D11Buffer>> VertexBuffers;
    std::unordered_map<UINT, UINT> VertexBufferStrides;
    std::unordered_map<UINT, UINT> VertexBufferOffsets;

    ComPtr<ID3D11Buffer> IndexBuffer;
    DXGI_FORMAT IndexBufferFormat = DXGI_FORMAT_UNKNOWN;
    UINT IndexBufferOffset = 0;

    // Primitive Topology
    D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;

#pragma endregion

#pragma region(V H D G P C Shaders)

    // Shaders
    ComPtr<ID3D11VertexShader> VertexShader;
    ComPtr<ID3D11PixelShader> PixelShader;
    ComPtr<ID3D11GeometryShader> GeometryShader;
    ComPtr<ID3D11HullShader> HullShader;
    ComPtr<ID3D11DomainShader> DomainShader;
    ComPtr<ID3D11ComputeShader> ComputeShader;

    // Shader Constant Buffers
    std::unordered_map<UINT, ComPtr<ID3D11Buffer>> VertexConstantBuffers;
    std::unordered_map<UINT, ComPtr<ID3D11Buffer>> PixelConstantBuffers;
    std::unordered_map<UINT, ComPtr<ID3D11Buffer>> GeometryConstantBuffers;
    std::unordered_map<UINT, ComPtr<ID3D11Buffer>> HullConstantBuffers;
    std::unordered_map<UINT, ComPtr<ID3D11Buffer>> DomainConstantBuffers;
    std::unordered_map<UINT, ComPtr<ID3D11Buffer>> ComputeConstantBuffers;

    // Shader Resources (Textures)
    std::unordered_map<UINT, ComPtr<ID3D11ShaderResourceView>> VertexShaderResourceViews;
    std::unordered_map<UINT, ComPtr<ID3D11ShaderResourceView>> PixelShaderResourceViews;
    std::unordered_map<UINT, ComPtr<ID3D11ShaderResourceView>> GeometryShaderResourceViews;
    std::unordered_map<UINT, ComPtr<ID3D11ShaderResourceView>> HullShaderResourceViews;
    std::unordered_map<UINT, ComPtr<ID3D11ShaderResourceView>> DomainShaderResourceViews;
    std::unordered_map<UINT, ComPtr<ID3D11ShaderResourceView>> ComputeShaderResourceViews;

    // Shader Samplers
    std::unordered_map<UINT, ComPtr<ID3D11SamplerState>> VertexSamplerStates;
    std::unordered_map<UINT, ComPtr<ID3D11SamplerState>> HullSamplerStates;
    std::unordered_map<UINT, ComPtr<ID3D11SamplerState>> DomainSamplerStates;
    std::unordered_map<UINT, ComPtr<ID3D11SamplerState>> GeometrySamplerStates;
    std::unordered_map<UINT, ComPtr<ID3D11SamplerState>> PixelSamplerStates;
    std::unordered_map<UINT, ComPtr<ID3D11SamplerState>> ComputeSamplerStates;

#pragma endregion

#pragma region(Stream Output)

    // Stream Output Targets (optional)
    std::unordered_map<UINT, ComPtr<ID3D11Buffer>> StreamOutputBuffers;

#pragma endregion

#pragma region(Rasterizer)

    // Viewports
    std::vector<D3D11_VIEWPORT> Viewports;

    // Scissor Rectangles
    std::vector<D3D11_RECT> ScissorRects;

    // RS State
    ComPtr<ID3D11RasterizerState> RasterizerState;

#pragma endregion

#pragma region(Output Merger)

    UINT NumRenderTargets = MaxRenderTargets;
    ComPtr<ID3D11RenderTargetView> RenderTargetViews[MaxRenderTargets]; // todo: this reads and writes all 8 RTV pointers, but RTV count is the only thing that doesn't return a count. could use a map to store sparse list more effectively.
    ComPtr<ID3D11DepthStencilView> DepthStencilView;
    ComPtr<ID3D11DepthStencilState> DepthStencilState;
    ComPtr<ID3D11BlendState> BlendState;
    float BlendFactor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    UINT SampleMask = 0xFFFFFFFF;

#pragma endregion

#pragma region(Compute)

    // Unordered Access Views (for compute shader CS)
    std::unordered_map<UINT, ComPtr<ID3D11UnorderedAccessView>> UnorderedAccessViews;

#pragma endregion
};

class PipelineStateManager
{
public:

    PipelineStateManager(ComPtr<ID3D11DeviceContext> Context)
        : Context_(Context)
    {
    }

    void Initialize()
    {
        // Query the current state from the device context and store it internally
        QueryPipelineState(CurrentState_);
    }

    const PipelineState& GetCurrentState() const
    {
        return CurrentState_;
    }

    void SetVertexBuffersIfDifferent(const PipelineState& NewState);

    void SetIndexBufferIfDifferent(const PipelineState& NewState);

    void ApplyPipelineState(const PipelineState& NewState)
    {
        SetVertexBuffersIfDifferent(NewState);
        SetIndexBufferIfDifferent(NewState);
        SetShadersIfDifferent(NewState);
        SetInputLayoutIfDifferent(NewState);
        SetBlendStateIfDifferent(NewState);
        SetRasterizerStateIfDifferent(NewState);
        SetDepthStencilStateIfDifferent(NewState);
        SetViewportsIfDifferent(NewState);
        SetRenderTargetsIfDifferent(NewState);
        SetConstantBuffersIfDifferent(NewState);
        SetShaderResourcesIfDifferent(NewState);
        SetUnorderedAccessViewsIfDifferent(NewState);
        SetSamplersIfDifferent(NewState);
        SetPrimitiveTopologyIfDifferent(NewState);
        SetScissorRectsIfDifferent(NewState);

        CurrentState_ = NewState;
    }

private:

    void QueryPipelineState(PipelineState& state)
    {
        // Query VertexBuffers
        for (UINT i = 0; i < PipelineState::MaxVertexBuffers; ++i)
        {
            ComPtr<ID3D11Buffer> buffer;
            UINT stride = 0;
            UINT offset = 0;
            Context_->IAGetVertexBuffers(i, 1, buffer.GetAddressOf(), &stride, &offset);
            if (buffer) state.VertexBuffers[i] = buffer;
            if (stride) state.VertexBufferStrides[i] = stride;
            if (offset) state.VertexBufferOffsets[i] = offset;
        }

        // Query Index Buffer
        Context_->IAGetIndexBuffer(state.IndexBuffer.GetAddressOf(), &state.IndexBufferFormat, &state.IndexBufferOffset);

        // Query Shaders
        Context_->VSGetShader(state.VertexShader.GetAddressOf(), nullptr, nullptr);
        Context_->PSGetShader(state.PixelShader.GetAddressOf(), nullptr, nullptr);
        Context_->GSGetShader(state.GeometryShader.GetAddressOf(), nullptr, nullptr);
        Context_->HSGetShader(state.HullShader.GetAddressOf(), nullptr, nullptr);
        Context_->DSGetShader(state.DomainShader.GetAddressOf(), nullptr, nullptr);
        Context_->CSGetShader(state.ComputeShader.GetAddressOf(), nullptr, nullptr);

        // Query Input Layout
        Context_->IAGetInputLayout(state.InputLayout.GetAddressOf());

        // Query States
        Context_->OMGetBlendState(state.BlendState.GetAddressOf(), state.BlendFactor, &state.SampleMask);
        Context_->RSGetState(state.RasterizerState.GetAddressOf());
        Context_->OMGetDepthStencilState(state.DepthStencilState.GetAddressOf(), nullptr);

        // Query Viewports
        UINT numViewports = PipelineState::MaxViewportsAndScissorRects;
        D3D11_VIEWPORT Viewports[PipelineState::MaxViewportsAndScissorRects];
        Context_->RSGetViewports(&numViewports, Viewports);
        state.Viewports.resize(numViewports);
        for (UINT i = 0; i < numViewports; i++)
        {
            state.Viewports[i] = Viewports[i];
        }

        // Query Render Targets
        Context_->OMGetRenderTargets(state.NumRenderTargets, state.RenderTargetViews->GetAddressOf(), state.DepthStencilView.GetAddressOf());

        // Query Constant Buffers
        for (UINT i = 0; i < PipelineState::MaxConstantBuffers; ++i)
        {
            ComPtr<ID3D11Buffer> buffer;
            Context_->VSGetConstantBuffers(i, 1, buffer.GetAddressOf());
            if (buffer) state.VertexConstantBuffers[i] = buffer;

            Context_->PSGetConstantBuffers(i, 1, buffer.GetAddressOf());
            if (buffer) state.PixelConstantBuffers[i] = buffer;

            Context_->GSGetConstantBuffers(i, 1, buffer.GetAddressOf());
            if (buffer) state.GeometryConstantBuffers[i] = buffer;

            Context_->HSGetConstantBuffers(i, 1, buffer.GetAddressOf());
            if (buffer) state.HullConstantBuffers[i] = buffer;

            Context_->DSGetConstantBuffers(i, 1, buffer.GetAddressOf());
            if (buffer) state.DomainConstantBuffers[i] = buffer;

            Context_->CSGetConstantBuffers(i, 1, buffer.GetAddressOf());
            if (buffer) state.ComputeConstantBuffers[i] = buffer;
        }

        // Query Shader Resources
        for (UINT i = 0; i < PipelineState::MaxSRVs; ++i)
        {
            ComPtr<ID3D11ShaderResourceView> srv;
            Context_->VSGetShaderResources(i, 1, srv.GetAddressOf());
            if (srv) state.VertexShaderResourceViews[i] = srv;

            Context_->PSGetShaderResources(i, 1, srv.GetAddressOf());
            if (srv) state.PixelShaderResourceViews[i] = srv;

            Context_->GSGetShaderResources(i, 1, srv.GetAddressOf());
            if (srv) state.GeometryShaderResourceViews[i] = srv;

            Context_->HSGetShaderResources(i, 1, srv.GetAddressOf());
            if (srv) state.HullShaderResourceViews[i] = srv;

            Context_->DSGetShaderResources(i, 1, srv.GetAddressOf());
            if (srv) state.DomainShaderResourceViews[i] = srv;

            Context_->CSGetShaderResources(i, 1, srv.GetAddressOf());
            if (srv) state.ComputeShaderResourceViews[i] = srv;
        }

        // Query Unordered Access Views
        for (UINT i = 0; i < PipelineState::MaxUAVs; ++i)
        {
            ComPtr<ID3D11UnorderedAccessView> uav;
            Context_->CSGetUnorderedAccessViews(i, 1, uav.GetAddressOf());
            if (uav) state.UnorderedAccessViews[i] = uav;
        }

        // Query Samplers
        for (UINT i = 0; i < PipelineState::MaxSamplers; ++i)
        {
            ComPtr<ID3D11SamplerState> sampler;
            Context_->VSGetSamplers(i, 1, sampler.GetAddressOf());
            if (sampler) state.VertexSamplerStates[i] = sampler;

            Context_->PSGetSamplers(i, 1, sampler.GetAddressOf());
            if (sampler) state.PixelSamplerStates[i] = sampler;

            Context_->GSGetSamplers(i, 1, sampler.GetAddressOf());
            if (sampler) state.GeometrySamplerStates[i] = sampler;

            Context_->HSGetSamplers(i, 1, sampler.GetAddressOf());
            if (sampler) state.HullSamplerStates[i] = sampler;

            Context_->DSGetSamplers(i, 1, sampler.GetAddressOf());
            if (sampler) state.DomainSamplerStates[i] = sampler;

            Context_->CSGetSamplers(i, 1, sampler.GetAddressOf());
            if (sampler) state.ComputeSamplerStates[i] = sampler;
        }

        // Query Primitive Topology
        Context_->IAGetPrimitiveTopology(&state.PrimitiveTopology);

        // Query Scissor Rects
        UINT numRects = PipelineState::MaxViewportsAndScissorRects;
        D3D11_RECT Rects[PipelineState::MaxViewportsAndScissorRects];
        Context_->RSGetScissorRects(&numRects, Rects);
        state.ScissorRects.resize(numRects);
        for (UINT i = 0; i < numRects; i++)
        {
            state.ScissorRects[i] = Rects[i];
        }
    }

    void SetShadersIfDifferent(const PipelineState& NewState)
    {
        if (NewState.VertexShader != CurrentState_.VertexShader)
        {
            Context_->VSSetShader(NewState.VertexShader.Get(), nullptr, 0);
        }
        if (NewState.PixelShader != CurrentState_.PixelShader)
        {
            Context_->PSSetShader(NewState.PixelShader.Get(), nullptr, 0);
        }
        if (NewState.GeometryShader != CurrentState_.GeometryShader)
        {
            Context_->GSSetShader(NewState.GeometryShader.Get(), nullptr, 0);
        }
        if (NewState.HullShader != CurrentState_.HullShader)
        {
            Context_->HSSetShader(NewState.HullShader.Get(), nullptr, 0);
        }
        if (NewState.DomainShader != CurrentState_.DomainShader)
        {
            Context_->DSSetShader(NewState.DomainShader.Get(), nullptr, 0);
        }
        if (NewState.ComputeShader != CurrentState_.ComputeShader)
        {
            Context_->CSSetShader(NewState.ComputeShader.Get(), nullptr, 0);
        }
    }

    void SetInputLayoutIfDifferent(const PipelineState& NewState)
    {
        if (NewState.InputLayout != CurrentState_.InputLayout)
        {
            Context_->IASetInputLayout(NewState.InputLayout.Get());
        }
    }

    void SetBlendStateIfDifferent(const PipelineState& NewState)
    {
        bool blendFactorChanged = false;
        for (int i = 0; i < 4; ++i)
        {
            if (NewState.BlendFactor[i] != CurrentState_.BlendFactor[i]) // todo: consider floating-point equality. though I'm thinking in this case i think binary compare is correct.   // NOLINT(clang-diagnostic-float-equal)
            {
                blendFactorChanged = true;
                break;
            }
        }

        if (NewState.BlendState != CurrentState_.BlendState || blendFactorChanged || NewState.SampleMask != CurrentState_.SampleMask)
        {
            Context_->OMSetBlendState(NewState.BlendState.Get(), NewState.BlendFactor, NewState.SampleMask);
        }
    }

    void SetRasterizerStateIfDifferent(const PipelineState& NewState)
    {
        if (NewState.RasterizerState != CurrentState_.RasterizerState)
        {
            Context_->RSSetState(NewState.RasterizerState.Get());
        }
    }

    void SetDepthStencilStateIfDifferent(const PipelineState& NewState)
    {
        if (NewState.DepthStencilState != CurrentState_.DepthStencilState)
        {
            Context_->OMSetDepthStencilState(NewState.DepthStencilState.Get(), 0);
        }
    }

    void SetViewportsIfDifferent(const PipelineState& NewState)
    {
        if (NewState.Viewports != CurrentState_.Viewports)
        {
            Context_->RSSetViewports(static_cast<UINT>(NewState.Viewports.size()), NewState.Viewports.data());
        }
    }

    void SetRenderTargetsIfDifferent(const PipelineState& NewState)
    {
        if (NewState.NumRenderTargets != CurrentState_.NumRenderTargets ||
            !std::equal(NewState.RenderTargetViews, NewState.RenderTargetViews + NewState.NumRenderTargets, CurrentState_.RenderTargetViews))
        {
            if (NewState.NumRenderTargets == 0)
            {
                Context_->OMSetRenderTargets(0, nullptr, NewState.DepthStencilView.Get());
            }
            else
            {
                Context_->OMSetRenderTargets(NewState.NumRenderTargets, NewState.RenderTargetViews->GetAddressOf(), NewState.DepthStencilView.Get());
            }
        }
    }

    void SetConstantBuffersIfDifferent(const PipelineState& NewState)
    {
        auto SetBuffers = [&](const auto& newBuffers, const auto& currentBuffers, auto setBufferFn)
        {
            for (const auto& [slot, buffer] : newBuffers)
            {
                auto it = currentBuffers.find(slot);
                if (it == currentBuffers.end() || it->second != buffer)
                {
                    setBufferFn(slot, 1, buffer.GetAddressOf());
                }
            }
        };

        SetBuffers(NewState.VertexConstantBuffers, CurrentState_.VertexConstantBuffers, [&](UINT slot, UINT Count, ID3D11Buffer* const* ppBuffer) { Context_->VSSetConstantBuffers(slot, Count, ppBuffer); });
        SetBuffers(NewState.HullConstantBuffers, CurrentState_.HullConstantBuffers, [&](UINT slot, UINT Count, ID3D11Buffer* const* ppBuffer) { Context_->HSSetConstantBuffers(slot, Count, ppBuffer); });
        SetBuffers(NewState.DomainConstantBuffers, CurrentState_.DomainConstantBuffers, [&](UINT slot, UINT Count, ID3D11Buffer* const* ppBuffer) { Context_->DSSetConstantBuffers(slot, Count, ppBuffer); });
        SetBuffers(NewState.GeometryConstantBuffers, CurrentState_.GeometryConstantBuffers, [&](UINT slot, UINT Count, ID3D11Buffer* const* ppBuffer) { Context_->GSSetConstantBuffers(slot, Count, ppBuffer); });
        SetBuffers(NewState.PixelConstantBuffers, CurrentState_.PixelConstantBuffers, [&](UINT slot, UINT Count, ID3D11Buffer* const* ppBuffer) { Context_->PSSetConstantBuffers(slot, Count, ppBuffer); });
        SetBuffers(NewState.ComputeConstantBuffers, CurrentState_.ComputeConstantBuffers, [&](UINT slot, UINT Count, ID3D11Buffer* const* ppBuffer) { Context_->CSSetConstantBuffers(slot, Count, ppBuffer); });
    }

    void SetShaderResourcesIfDifferent(const PipelineState& NewState)
    {
        auto SetResources = [&](const auto& newResources, const auto& currentResources, auto setResourceFn)
        {
            for (const auto& [slot, resource] : newResources)
            {
                auto it = currentResources.find(slot);
                if (it == currentResources.end() || it->second != resource)
                {
                    setResourceFn(slot, 1, resource.GetAddressOf());
                }
            }
        };

        SetResources(NewState.VertexShaderResourceViews, CurrentState_.VertexShaderResourceViews, [&](UINT slot, UINT Count, ID3D11ShaderResourceView* const* ppSRV) { Context_->VSSetShaderResources(slot, Count, ppSRV); });
        SetResources(NewState.HullShaderResourceViews, CurrentState_.HullShaderResourceViews, [&](UINT slot, UINT Count, ID3D11ShaderResourceView* const* ppSRV) { Context_->HSSetShaderResources(slot, Count, ppSRV); });
        SetResources(NewState.DomainShaderResourceViews, CurrentState_.DomainShaderResourceViews, [&](UINT slot, UINT Count, ID3D11ShaderResourceView* const* ppSRV) { Context_->DSSetShaderResources(slot, Count, ppSRV); });
        SetResources(NewState.GeometryShaderResourceViews, CurrentState_.GeometryShaderResourceViews, [&](UINT slot, UINT Count, ID3D11ShaderResourceView* const* ppSRV) { Context_->GSSetShaderResources(slot, Count, ppSRV); });
        SetResources(NewState.PixelShaderResourceViews, CurrentState_.PixelShaderResourceViews, [&](UINT slot, UINT Count, ID3D11ShaderResourceView* const* ppSRV) { Context_->PSSetShaderResources(slot, Count, ppSRV); });
        SetResources(NewState.ComputeShaderResourceViews, CurrentState_.ComputeShaderResourceViews, [&](UINT slot, UINT Count, ID3D11ShaderResourceView* const* ppSRV) { Context_->CSSetShaderResources(slot, Count, ppSRV); });
    }

    void SetUnorderedAccessViewsIfDifferent(const PipelineState& NewState)
    {
        for (UINT i = 0; i < PipelineState::MaxUAVs; ++i)
        {
            auto itNew = NewState.UnorderedAccessViews.find(i);
            auto itCurrent = CurrentState_.UnorderedAccessViews.find(i);
            if ((itNew != NewState.UnorderedAccessViews.end() && itCurrent != CurrentState_.UnorderedAccessViews.end() && itNew->second != itCurrent->second) ||
                (itNew != NewState.UnorderedAccessViews.end() && itCurrent == CurrentState_.UnorderedAccessViews.end()) ||
                (itNew == NewState.UnorderedAccessViews.end() && itCurrent != CurrentState_.UnorderedAccessViews.end()))
            {
                ID3D11UnorderedAccessView* uav = itNew != NewState.UnorderedAccessViews.end() ? itNew->second.Get() : nullptr;
                Context_->CSSetUnorderedAccessViews(i, 1, &uav, nullptr);
            }
        }
    }

    void SetSamplersIfDifferent(const PipelineState& NewState)
    {
        auto SetSamplers = [&](const auto& newSamplers, const auto& currentSamplers, auto setSamplerFn)
        {
            for (const auto& [slot, sampler] : newSamplers)
            {
                auto it = currentSamplers.find(slot);
                if (it == currentSamplers.end() || it->second != sampler)
                {
                    setSamplerFn(slot, 1, sampler.GetAddressOf());
                }
            }
        };

        SetSamplers(NewState.VertexSamplerStates, CurrentState_.VertexSamplerStates, [&](UINT Slot, UINT Count, ID3D11SamplerState* const* ppSampler) { Context_->VSSetSamplers(Slot, Count, ppSampler); });
        SetSamplers(NewState.HullSamplerStates, CurrentState_.HullSamplerStates, [&](UINT Slot, UINT Count, ID3D11SamplerState* const* ppSampler) { Context_->HSSetSamplers(Slot, Count, ppSampler); });
        SetSamplers(NewState.DomainSamplerStates, CurrentState_.DomainSamplerStates, [&](UINT Slot, UINT Count, ID3D11SamplerState* const* ppSampler) { Context_->DSSetSamplers(Slot, Count, ppSampler); });
        SetSamplers(NewState.GeometrySamplerStates, CurrentState_.GeometrySamplerStates, [&](UINT Slot, UINT Count, ID3D11SamplerState* const* ppSampler) { Context_->GSSetSamplers(Slot, Count, ppSampler); });
        SetSamplers(NewState.PixelSamplerStates, CurrentState_.PixelSamplerStates, [&](UINT Slot, UINT Count, ID3D11SamplerState* const* ppSampler) { Context_->PSSetSamplers(Slot, Count, ppSampler); });
        SetSamplers(NewState.ComputeSamplerStates, CurrentState_.ComputeSamplerStates, [&](UINT Slot, UINT Count, ID3D11SamplerState* const* ppSampler) { Context_->CSSetSamplers(Slot, Count, ppSampler); });
    }

    void SetPrimitiveTopologyIfDifferent(const PipelineState& NewState)
    {
        if (NewState.PrimitiveTopology != CurrentState_.PrimitiveTopology)
        {
            Context_->IASetPrimitiveTopology(NewState.PrimitiveTopology);
        }
    }

    void SetScissorRectsIfDifferent(const PipelineState& NewState)
    {
        if (NewState.ScissorRects != CurrentState_.ScissorRects)
        {
            Context_->RSSetScissorRects(static_cast<UINT>(NewState.ScissorRects.size()), NewState.ScissorRects.data());
        }
    }

    ComPtr<ID3D11DeviceContext> Context_;
    PipelineState CurrentState_;
};

inline void PipelineStateManager::SetVertexBuffersIfDifferent(const PipelineState& NewState)
{
    // Set VertexBuffers
    for (UINT i = 0; i < PipelineState::MaxVertexBuffers; ++i)
    {
        auto itNew = NewState.VertexBuffers.find(i);
        auto itCurrent = CurrentState_.VertexBuffers.find(i);
        auto itStride = NewState.VertexBufferStrides.find(i);
        auto itOffset = NewState.VertexBufferOffsets.find(i);
        auto itCurrentStride = CurrentState_.VertexBufferStrides.find(i);
        auto itCurrentOffset = CurrentState_.VertexBufferOffsets.find(i);
        

        bool bufferChanged = itNew != NewState.VertexBuffers.end() && itCurrent != CurrentState_.VertexBuffers.end() && itNew->second != itCurrent->second;
        bool strideChanged = itStride != NewState.VertexBufferStrides.end() && itCurrentStride != CurrentState_.VertexBufferStrides.end() && itStride->second != itCurrentStride->second;
        bool offsetChanged = itOffset != NewState.VertexBufferOffsets.end() && itCurrentOffset != CurrentState_.VertexBufferOffsets.end() && itOffset->second != itCurrentOffset->second;

        if(bufferChanged || strideChanged || offsetChanged)
        {
            Context_->IASetVertexBuffers(i, 1, itNew != NewState.VertexBuffers.end() ? itNew->second.GetAddressOf() : nullptr, itStride != NewState.VertexBufferStrides.end() ? &itStride->second : nullptr, itOffset != NewState.VertexBufferOffsets.end() ? &itOffset->second : nullptr);
        }
    }
}

inline void PipelineStateManager::SetIndexBufferIfDifferent(const PipelineState& NewState)
{
    if (NewState.IndexBuffer != CurrentState_.IndexBuffer || NewState.IndexBufferFormat != CurrentState_.IndexBufferFormat || NewState.IndexBufferOffset != CurrentState_.IndexBufferOffset)
    {
        Context_->IASetIndexBuffer(NewState.IndexBuffer.Get(), NewState.IndexBufferFormat, NewState.IndexBufferOffset);
    }
}
