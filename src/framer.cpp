#include "framer.hpp"



void simverse::swp::Framer::feed(std::span<const std::uint8_t> bytes){

    if (!bytes.empty())
        buf_.insert(buf_.end(), bytes.begin(), bytes.end());
}

void simverse::swp::Framer::clear(){
    buf_.clear();
    buf_.shrink_to_fit();
}

 simverse::swp::Framer::Status simverse::swp::Framer::next(Frame& out){
    if (buf_.size() < 3) return Status::NeedMore;

    // parse big-endian u16 of (id+payload)

    const std::uint16_t len = static_cast<std::uint16_t>(
        static_cast<std::uint16_t> (buf_[0] << 8) | 
        static_cast<std::uint16_t>(buf_[1])
    );

    if (len==0 || len > MAX_LEN) {
        clear();
        return Status::Error;
    }

    const std::size_t total = 2u + static_cast<std::size_t>(len);

    if (buf_.size() < total) return Status::NeedMore;

    const std::uint8_t id_byte = buf_[2];
    out.id = static_cast<MsgId> (id_byte);

    const std::size_t payload_len = static_cast<std::size_t>(len - 1u);
    out.payload.assign(buf_.begin() + 3, buf_.begin() + 3 + payload_len);

    buf_.erase(buf_.begin(), buf_.begin() + static_cast<std::ptrdiff_t>(total));

    return Status::Ok;

 }
