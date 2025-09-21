#ifndef FRAMER_H
#define FRAMER_H

#include <cstdint>
#include <vector>
#include <span>

#include "proto/protocol.hpp"


namespace simverse::swp {

struct Frame {
   MsgId id {};
   std::vector<std::uint8_t> payload;
};


class Framer {
public:

    // Police cap for (id+payload)
    static constexpr std::uint16_t MAX_LEN = 16 * 1024; 

    void feed(std::span<const std::uint8_t> bytes);

    enum class Status {Ok, NeedMore, Error};

    // Produce one frame into 'out'.
    // Returns:
    // Ok -> 'out' filled, internal buffer advanced.
    // Needmore -> not enough bytes yet.
    // Error -> malformed, buffer clear.

    Status next(Frame& out);

    std::size_t buffered_bytes() const noexcept { return buf_.size();};
    void clear();
    




private:
    std::vector<std::uint8_t> buf_;

};

}


#endif