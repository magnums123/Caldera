#pragma once

#include <Defines.hpp>
#include <cstdint>
#include <functional>
#include <limits>
#include <random>

namespace CAL
{

static std::random_device randomDevice{};
static std::uniform_int_distribution<uint64_t> uniform_dist(1, std::numeric_limits<uint64_t>::max());

class UUID
{
   public:
    UUID()
    {
        std::mt19937_64 randomEngine{ randomDevice() };
        id = uniform_dist(randomEngine);
    }
    ~UUID() = default;

    bool operator==(const UUID& other) const { return this->id == other.id; }
    bool operator!=(const UUID& other) const { return this->id != other.id; }

    // Strict weak ordering so UUID / MeshHandle can be used as std::map keys.
    bool operator<(const UUID& other) const { return this->id < other.id; }

    uint64_t operator()() const { return this->id; }

   private:
    uint64_t id{};
};

using MeshHandle = UUID;
}  // namespace CAL

// TODO: This piece of code is AI generated. Remove or understand deeply.
namespace std
{

// Hash specialization so UUID / MeshHandle can be used as std::unordered_map keys.
// Since MeshHandle is an alias for CAL::UUID, this covers both.
template <>
struct hash<CAL::UUID>
{
    size_t operator()(const CAL::UUID& uuid) const
    {
        // SplitMix64 finalizer: mixes the 64-bit id so keys stay evenly distributed
        // across buckets even if they share low bits (e.g. sequential ids).
        uint64_t x = uuid();
        x ^= x >> 30;
        x *= 0xbf58476d1ce4e5b9ULL;
        x ^= x >> 27;
        x *= 0x94d049bb133111ebULL;
        x ^= x >> 31;
        return static_cast<size_t>(x);
    }
};

}  // namespace std
