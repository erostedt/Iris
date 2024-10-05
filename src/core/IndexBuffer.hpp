#pragma once
#include <cstdint>
#include <vector>

#include "Buffer.hpp"

namespace Iris
{

class FixedIndexBuffer : Buffer
{
  public:
    FixedIndexBuffer(const std::vector<uint32_t> &&indices);
    virtual ~FixedIndexBuffer();

    FixedIndexBuffer(const FixedIndexBuffer &other) = delete;
    FixedIndexBuffer &operator=(const FixedIndexBuffer &other) = delete;

    FixedIndexBuffer(FixedIndexBuffer &&other) noexcept;
    FixedIndexBuffer &operator=(FixedIndexBuffer &&other) noexcept;

    void Bind() const override;
    void Unbind() const override;
    size_t Size() const;
    const std::vector<uint32_t> &GetIndices() const;

  private:
    std::vector<uint32_t> m_indices;
    uint32_t m_id;
};

}; // namespace Iris
