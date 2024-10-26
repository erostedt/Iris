#pragma once

#include <cstdint>
#include <vector>

#include "Buffer.hpp"

namespace Iris
{

class IndexBuffer : Buffer
{
  public:
    IndexBuffer(const std::vector<uint32_t> &&indices);
    virtual ~IndexBuffer();

    IndexBuffer(const IndexBuffer &other) = delete;
    IndexBuffer &operator=(const IndexBuffer &other) = delete;

    IndexBuffer(IndexBuffer &&other) noexcept;
    IndexBuffer &operator=(IndexBuffer &&other) noexcept;

    void Bind() const override;
    void Unbind() const override;
    size_t Size() const;

  private:
    std::vector<uint32_t> m_indices;
    uint32_t m_id;
};

}; // namespace Iris
