#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
//#pragma warning(disable:4996)

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512  

int main()
{
    int result, ListenSocket;
    char recvbuf[DEFAULT_BUFLEN];
    char sendbuf[] = "this is a test";
    int recvbuflen = DEFAULT_BUFLEN;
    u_short port = 8008;
    WSAData wsaData;
    //sockaddr_in server;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return 1;
    }

    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ListenSocket < 0)
        printf("ERROR opening socket");
    /*
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; //inet_addr("176.59.139.3");
    server.sin_port = htons(port);
    std::cout << server.sin_port << std::endl;*/

    struct addrinfo hints;
    struct addrinfo* servinfo;  // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_NUMERICHOST;
    getaddrinfo("127.0.0.1", "8008", &hints, &servinfo);

    //result = bind(ListenSocket, (SOCKADDR*)&server, sizeof(server));
    result = bind(ListenSocket, servinfo->ai_addr, servinfo->ai_addrlen);


    if (result == SOCKET_ERROR) {
        printf("bind failed with error %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    result = listen(ListenSocket, 1);

    if (result == SOCKET_ERROR) {
        printf("listen failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    
    SOCKET AcceptSocket;
    printf("Waiting for client to connect...\n");

    AcceptSocket = accept(ListenSocket, NULL, NULL);
    if (AcceptSocket == INVALID_SOCKET) {
        printf("accept failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
        printf("Client connected.\n");

    result = send(AcceptSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (result == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(AcceptSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", result);

    result = shutdown(AcceptSocket, SD_SEND);
    if (result == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(AcceptSocket);
        WSACleanup();
        return 1;
    }

    do {

        result = recv(AcceptSocket, recvbuf, recvbuflen, 0);
        if (result > 0)
            printf("Bytes received: %d\n", result);
        else if (result == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());

    } while (result > 0);

    closesocket(ListenSocket);

    freeaddrinfo(servinfo);
    WSACleanup();
    std::cout << "Hello World!\n";
    getchar();
}