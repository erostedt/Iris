#pragma once
namespace Iris
{

class Buffer
{
  public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual ~Buffer() = default;
};

} // namespace Iris
