#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET sendingSocket;
    sockaddr_in broadcastAddr;
    int broadcastEnable = 1;
    std::string message = "hello world";
    int port = 8080;

    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // 创建套接字
    sendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sendingSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // 启用广播选项
    if (setsockopt(sendingSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable)) < 0) {
        std::cerr << "Error in setting Broadcast option." << std::endl;
        closesocket(sendingSocket);
        WSACleanup();
        return 1;
    }

    // 设置广播地址
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(port);
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;

    while (true) {
        // 发送消息
        int result = sendto(sendingSocket, message.c_str(), message.size(), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
        if (result == SOCKET_ERROR) {
            std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
        }
        else {
            std::cout << "Message sent: " << message << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 清理
    closesocket(sendingSocket);
    WSACleanup();

    return 0;
}
