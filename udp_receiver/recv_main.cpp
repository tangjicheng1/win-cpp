#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET receivingSocket;
    sockaddr_in recvAddr;
    char recvBuffer[1024];
    int recvBufferLen = 1024;
    int port = 8080;

    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // 创建套接字
    receivingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (receivingSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // 绑定端口和地址
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(port);
    recvAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(receivingSocket, (sockaddr*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(receivingSocket);
        WSACleanup();
        return 1;
    }

    // 接收消息
    while (true) {
        sockaddr_in senderAddr;
        int senderAddrSize = sizeof(senderAddr);
        int bytesReceived = recvfrom(receivingSocket, recvBuffer, recvBufferLen, 0, (sockaddr*)&senderAddr, &senderAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
        }
        else {
            recvBuffer[bytesReceived] = '\0';
            std::cout << "Received message: " << recvBuffer << std::endl;
        }
    }

    // 清理
    closesocket(receivingSocket);
    WSACleanup();

    return 0;
}
