#pragma once

#include <Defines.hpp>
#include <cstdint>
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
    uint64_t operator()() const { return this->id; }

   private:
    uint64_t id{};
};
}  // namespace CAL
