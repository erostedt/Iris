#include <cstddef>
#include <functional>

template <typename Iterator> size_t CombineHashes(Iterator begin, Iterator end)
{
    using value_type = typename std::decay<decltype(*begin)>::type;
    size_t hash = 0;
    std::hash<value_type> hasher{};
    for (; begin != end; ++begin)
    {
        hash ^= hasher(*begin) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

struct VectorHasher
{
    template <typename T> size_t operator()(const std::vector<T> &vec) const
    {
        return CombineHashes(vec.cbegin(), vec.cend());
    }
};
