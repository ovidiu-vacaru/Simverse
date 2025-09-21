#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <thread>
#include <vector>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")



int main() {
    WSADATA wsa {};

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){
        std::printf("WSAStartup failed %d\n", WSAGetLastError());
        return 1;
    }

    SOCKET s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s, (sockaddr*)&addr, sizeof(addr));
    listen(s, SOMAXCONN);

    std::cout << "Server listening on port 8080...\n";

}

