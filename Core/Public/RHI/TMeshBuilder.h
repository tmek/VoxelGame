// copyright 

#pragma once

#include <vector>
#include "HAL/Platform.h"

template <typename T>
using TArray = std::vector<T>;


template <typename VertexType>
class TMeshBuilder
{
public:

    TMeshBuilder() = default;

    void Clear();

    void Append(const std::vector<VertexType>& Vertices, const std::vector<uint32>& Indices);

    [[nodiscard]] uint32 GetVertexCount() const;

    [[nodiscard]] const std::vector<VertexType>& GetVertices() const;

    [[nodiscard]] uint32 GetIndexCount() const;

    [[nodiscard]] const std::vector<uint32>& GetIndices() const;

private:

    TArray<VertexType> VertexList_;
    TArray<uint32> IndexList_;
};


template <typename VertexType>
void TMeshBuilder<VertexType>::Clear()
{
    VertexList_.clear();
    IndexList_.clear();
}

 

template <typename VertexType>
void TMeshBuilder<VertexType>::Append(const std::vector<VertexType>& Vertices, const std::vector<uint32>& Indices)
{
    // Append vertices
    VertexList_.insert(VertexList_.end(), Vertices.begin(), Vertices.end());

    // Append indices
    uint32 IndexOffset = static_cast<uint32>(VertexList_.size());
    for (uint32 index : Indices)
    {
        IndexList_.push_back(IndexOffset + index);
    }
}

template <typename VertexType>
uint32 TMeshBuilder<VertexType>::GetVertexCount() const
{
    return static_cast<uint32>(VertexList_.size());
}

template <typename VertexType>
const std::vector<VertexType>& TMeshBuilder<VertexType>::GetVertices() const
{
    return VertexList_;
}

template <typename VertexType>
uint32 TMeshBuilder<VertexType>::GetIndexCount() const
{
    return static_cast<uint32>(IndexList_.size());
}

template <typename VertexType>
const std::vector<uint32>& TMeshBuilder<VertexType>::GetIndices() const
{
    return IndexList_;
}
