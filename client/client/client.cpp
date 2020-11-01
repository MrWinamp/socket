#include <iostream>
#include <WinSock2.h>
#include <stdio.h>
#include <ws2tcpip.h>

#pragma warning(disable:4996)

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512

int main()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    char sendbuf[] = "Client: sending data test";
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    std::string port = "8008";
    std::string host = "127.0.0.1";
    if (result != NO_ERROR) {
        printf("WSAStartup function failed with error: %d\n", result);
        return 1;
    }
    
    SOCKET ConnectSocket = INVALID_SOCKET;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("socket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    /*
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("176.59.131.101");
    clientService.sin_port = htons(8008);*/

    //inet_pton(AF_INET, "176.59.131.101", &clientService.sin_addr);
    struct addrinfo hints;
    struct addrinfo* servinfo;  // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo); //192.168.43.100
    //176.59.131.101
    result = connect(ConnectSocket, servinfo->ai_addr, servinfo->ai_addrlen);

    //result = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
    if (result == SOCKET_ERROR) {
        printf("connect function failed with error: %ld\n", WSAGetLastError());
        result = closesocket(ConnectSocket);
        if (result == SOCKET_ERROR)
            printf("closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    //Делаем общение с сервером

    result = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (result == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", result);

    result = shutdown(ConnectSocket, SD_SEND);
    if (result == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    do {

        result = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (result > 0)
            printf("Bytes received: %d\n", result);
        else if (result == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());

    } while (result > 0);

    result = closesocket(ConnectSocket);
    if (result == SOCKET_ERROR) {
        printf("closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    freeaddrinfo(servinfo);
    WSACleanup();
    std::cout << "Hello World!\n";
    getchar();
}