#include <random>

static inline float uniform(float min, float max)
{
    static std::random_device random_device;
    static std::mt19937 random_generator(random_device());
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(random_generator);
}
