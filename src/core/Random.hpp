#pragma once

#include <random>

namespace Iris
{
namespace Random
{

class RandomNumberGenerator
{
  public:
    RandomNumberGenerator()
    {
        m_random_generator = std::mt19937(m_random_device());
    }
    template <typename T> T Uniform(T min, T max)
    {
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(m_random_generator);
    }
    template <typename T> T Normal(T mean, T std)
    {
        std::normal_distribution<T> distribution(mean, std);
        return distribution(m_random_generator);
    }

  private:
    std::random_device m_random_device;
    std::mt19937 m_random_generator;
};

static RandomNumberGenerator __g_random_number_generator = RandomNumberGenerator();
static inline RandomNumberGenerator &GetRandomNumberGenerator()
{
    return __g_random_number_generator;
}

template <typename T> T Uniform(T min, T max)
{
    return GetRandomNumberGenerator().Uniform<T>(min, max);
}

template <typename T> T Normal(T min, T max)
{
    return GetRandomNumberGenerator().Normal<T>(min, max);
}

} // namespace Random
} // namespace Iris
