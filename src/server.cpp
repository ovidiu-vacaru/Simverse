#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")



int main() {
    WSADATA wsa {};

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){
        return 1;
    }

    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s, (SOCKADDR*)&addr, sizeof(addr));
    listen(s, SOMAXCONN);

    std::cout << "Server listening on port 8080...\n";



    SOCKET clientSocket = accept(s, NULL, NULL);

    if (clientSocket == INVALID_SOCKET){
        std::cout << "INVALID SOCKET";
    }




}

