#pragma once
#include <vector>

template <typename VertexType>
class MeshBuilder {
public:
    MeshBuilder();

    void Clear();
    void Append(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices);

    const std::vector<VertexType>& GetVertices() const { return vertexList; }
    const std::vector<uint32_t>& GetIndices() const { return indexList; }

    uint32_t GetVertexCount() const { return static_cast<uint32_t>(vertexList.size()); }
    uint32_t GetIndexCount() const { return static_cast<uint32_t>(indexList.size()); }
    
private:
    std::vector<VertexType> vertexList;
    std::vector<uint32_t> indexList;
};

template <typename VertexType>
MeshBuilder<VertexType>::MeshBuilder() {}

template <typename VertexType>
void MeshBuilder<VertexType>::Clear() {
    vertexList.clear();
    indexList.clear();
}

template <typename VertexType>
void MeshBuilder<VertexType>::Append(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices) {
    uint32_t indexOffset = static_cast<uint32_t>(vertexList.size());
    vertexList.insert(vertexList.end(), vertices.begin(), vertices.end());
    for (uint32_t index : indices) {
        indexList.push_back(indexOffset + index);
    }
}
