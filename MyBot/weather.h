#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>


// Get the weather Key
std::string get_weather_key()
{
    ifstream file("secret/weather");
    std::string key;

    if (file.is_open())
    {
        getline(file, key);
        file.close();
    }
    else
        std::cerr << "Unable to open the 'key' file.\nPlease check the file is exist." << std::endl;

    return key;
}

#pragma comment(lib, "Ws2_32.lib")

std::string httpGet(const std::string& host, const std::string& path, const std::string& query) {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    std::string response;

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return "";
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // 서버 주소 정보 가져오기
    if (getaddrinfo(host.c_str(), "80", &hints, &result) != 0) {
        std::cerr << "getaddrinfo failed." << std::endl;
        WSACleanup();
        return "";
    }

    // 소켓 생성 및 연결
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
            WSACleanup();
            return "";
        }

        if (connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (sock == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server!" << std::endl;
        WSACleanup();
        return "";
    }

    // GET 요청 문자열 구성
    std::string request = "GET " + path + "?" + query + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Connection: close\r\n\r\n";

    // 서버에 요청 전송
    if (send(sock, request.c_str(), (int)request.length(), 0) == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return "";
    }

    // 서버로부터 응답 수신
    char buffer[512];
    int bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        response += buffer;
    }

    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }

    // 소켓 종료
    closesocket(sock);
    WSACleanup();

    return response;
}

void show_data()
{
    std::string host = "apis.data.go.kr";
    std::string path = "/1360000/VilageFcstInfoService_2.0/getVilageFcst";
    std::string serviceKey = get_weather_key();
    std::string query = "serviceKey=" + serviceKey + "&pageNo=1&numOfRows=1000&dataType=XML&base_date=20210628&base_time=0600&nx=55&ny=127";

    std::string response = httpGet(host, path, query);

    std::cout << response << std::endl;
}
