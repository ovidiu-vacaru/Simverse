#pragma once
#include <cstdint>
#include <vector>
#include <span>
#include <stdexcept>
#include <string_view>

namespace simverse::swp {

enum class MsgId : std::uint8_t {
    CLIENT_HELLO = 1,
    SERVER_HELLO = 2,
    PING         = 20
};

[[nodiscard]] inline constexpr std::string_view to_string(MsgId id) noexcept {
    switch (id) {
        case MsgId::CLIENT_HELLO: return "CLIENT_HELLO";
        case MsgId::SERVER_HELLO: return "SERVER_HELLO";
        case MsgId::PING:         return "PING";
    }
    return "UNKNOWN";
}

[[nodiscard]] inline std::vector<std::uint8_t>
pack(MsgId id, std::span<const std::uint8_t> payload = {}) {
    if (payload.size() > 0xFFFF - 1) {
        throw std::length_error("payload too large for u16 length");
    }

    const std::uint16_t len = static_cast<std::uint16_t>(1 + payload.size());

    std::vector<std::uint8_t> output;
    output.reserve(2 + len);

    output.push_back(static_cast<std::uint8_t>(len >> 8));
    output.push_back(static_cast<std::uint8_t>(len));
    output.push_back(static_cast<std::uint8_t>(id));
    output.insert(output.end(), payload.begin(), payload.end());
    return output;
}

} 
