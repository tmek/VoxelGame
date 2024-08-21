

#include <RHI/MeshAssembler.h>

MeshAssembler::MeshAssembler(ID3D11Device* device) : device(device)
{
    
}

ComPtr<ID3D11Buffer> MeshAssembler::CreateIndexBuffer(const std::vector<UINT>& indices) {
    if (indices.empty()) {
        return nullptr;
    }

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = indices.data();

    ComPtr<ID3D11Buffer> buffer;
    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, buffer.GetAddressOf());
    assert(SUCCEEDED(hr));

    return buffer;
}
