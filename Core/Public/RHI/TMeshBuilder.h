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

    [[nodiscard]] const std::vector<uint16>& GetIndices() const;

private:

    TArray<VertexType> VertexList_;
    TArray<uint16> IndexList_;
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
    size_t PreviousVertexCount = VertexList_.size();
    
    // Append vertices
    VertexList_.insert(VertexList_.end(), Vertices.begin(), Vertices.end());

    // Append indices
    for (uint32 index : Indices)
    {
        uint16 NewVertexIndex = static_cast<uint16>(PreviousVertexCount + index);
        IndexList_.push_back(NewVertexIndex);
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
const std::vector<uint16>& TMeshBuilder<VertexType>::GetIndices() const
{
    return IndexList_;
}
