#include <cstdint>
#include <bitset>
#include <limits>

using Entity = std::uint32_t;
using ComponentID = std::uint8_t;



constexpr std::size_t kNumComponents = static_cast<std::size_t>(std::numeric_limits<ComponentID>::max() + 1);

using Signature = std::bitset<kNumComponents>;
