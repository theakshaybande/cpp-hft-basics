#include <iostream>
#include <vector>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

void start_udp_server(int port) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket\n";
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket\n";
        return;
    }

    std::cout << "Listening for UDP packets on port " << port << "...\n";
    std::cout << "(Press Ctrl+C to stop)\n";

    // In a real HFT app, we would use non-blocking I/O or a busy-wait loop
    // For this demo, we'll just receive one packet and exit to prove it works.
    
    char buffer[1024];
    sockaddr_in client_addr;
#ifdef _WIN32
    int addr_len = sizeof(client_addr);
#else
    socklen_t addr_len = sizeof(client_addr);
#endif

    // Simulate receiving a packet (blocking call)
    // To test this, you'd need a sender. 
    // Since we can't easily run two interactive processes here, 
    // we will just set a timeout so it doesn't block forever if no data comes.
    
#ifdef _WIN32
    DWORD timeout = 3000; // 3 seconds
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
#else
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
#endif

    int n = recvfrom(sockfd, buffer, 1024, 0, (sockaddr*)&client_addr, &addr_len);
    if (n > 0) {
        buffer[n] = '\0';
        std::cout << "Received: " << buffer << "\n";
    } else {
        std::cout << "No packet received (timeout).\n";
    }

#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
}

int main() {
    start_udp_server(12345);
    return 0;
}
