#include <iostream>
#include <WinSock2.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    int err, sockfd;
    WSAData wsaData;
    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket");

    WSACleanup();
    std::cout << "Hello World!\n";
}