#include <iostream>
#include <cstdio>
#include <vector>
#include "proto/protocol.hpp"
#include "framer.hpp"
#include <cassert>
#include <iostream>

using namespace simverse::swp;


static std::vector<std::vector<uint8_t>> g_outbox;

static void handle_frame(const Frame& f){

    switch(f.id){
        case MsgId::CLIENT_HELLO: {
            g_outbox.push_back(pack(MsgId::SERVER_HELLO));
            break;
        }
        case MsgId::PING: {
            g_outbox.push_back(pack(MsgId::PING, f.payload));
            break;
        }
        default:
            std::cout << "No MATCHING MESSAGE ID " << static_cast<int>(f.id) << std::endl;
            break;
    }
}

int main(){

    
    const auto f1 = pack(MsgId::CLIENT_HELLO);
    const std::uint8_t data[] = {0xAA, 0xBB, 0xCC};
    const auto f2  = pack(MsgId::PING, std::span<const std::uint8_t>(data, 3));


    // TCP 

    std::vector<std::uint8_t> blob;
    blob.insert(blob.end(), f1.begin(), f1.end());
    blob.insert(blob.end(), f2.begin(), f2.end());

    Framer fr;



    const std::size_t half = blob.size() / 2;
    fr.feed({blob.data(), half});


    {
        Frame out{};
        while (true){
            auto s = fr.next(out);
            if ( s == Framer::Status::Ok) { handle_frame(out); continue;}
            if (s == Framer::Status::NeedMore) {std::cout << "Need more"; break; }
            else {std::cout <<("error parsing frame"); break;}
        }
    }

    // feeding the rest 

    fr.feed({blob.data() + half, blob.size() - half});

    {
        Frame out{};
        while (true){
            auto s = fr.next(out);
            if ( s == Framer::Status::Ok) { handle_frame(out); continue;}
            if (s == Framer::Status::NeedMore){ 
              std::cout << "Need more";
              break;
            }
            else { 
                std::cout <<("error parsing frame"); 
                break;
            }
    }

    for (const auto& buf : g_outbox) {
        const unsigned len = (unsigned(buf[0]) << 8) | unsigned(buf[1]);
        std::printf("OUT: bytes=%zu len=%u id=0x%02X\n", buf.size(), len, buf[2]);
    }
}
}
   
