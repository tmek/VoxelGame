
#include "RHI/Mesh.h"

void Mesh::BindToDeviceContext(ID3D11DeviceContext* deviceContext) const
{
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &vertexStride, &offset);
    deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::Draw(ID3D11DeviceContext* deviceContext) const
{
    deviceContext->DrawIndexed(indexCount, 0, 0);
}

void Mesh::DrawSubMesh(ID3D11DeviceContext* deviceContext, UINT subMeshIndex) const
{
    const SubMesh& subMesh = SubMeshes[subMeshIndex];
    deviceContext->DrawIndexed(subMesh.indexCount, 0, subMesh.baseVertexLocation);
}
