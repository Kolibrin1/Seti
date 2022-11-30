#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
using namespace std;
#define PORT 1234

// CLIENT

struct goods
{
    string nazv;
    int price;
}B;

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Введите ip адрес сервера" << endl;
    string ip = "192.168.1.85";
    cin >> ip;
    const char* SERVERADDR = ip.c_str();
    char buff[10 * 1014];
    cout << "UDP Demo Client";

    // Winsocks lib _init_
    if (WSAStartup(0x202, (WSADATA*)&buff))
    {
        cout << "WSASTARTUP ERROR: " << WSAGetLastError() << "\n";
        return -1;
    }

    // открытие сокета
    SOCKET my_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (my_sock == INVALID_SOCKET) {
        cout << "SOCKET() ERROR: " << WSAGetLastError() << "\n";
        WSACleanup();
        return -1;
    }

    // СОЕДИНЕНИЕ
    HOSTENT* hst;
    sockaddr_in Daddr;
    Daddr.sin_family = AF_INET;
    Daddr.sin_port = htons(PORT);

    // определение IP-адреса узла
    if (inet_addr(SERVERADDR))
        Daddr.sin_addr.s_addr = inet_addr(SERVERADDR);
    else
        if (hst = gethostbyname(SERVERADDR))
            Daddr.sin_addr.s_addr = ((unsigned long**)
                hst->h_addr_list)[0][0];
        else {
            cout << "Unknown Host: " << WSAGetLastError() << "\n";
            closesocket(my_sock);
            WSACleanup();
            return -1;
        }

    // ОБМЕН ДАННЫХ
    while (1) {
        string Stroka;
        cout << "Продолжить? (y/n)  " << endl;
        cin >> Stroka;
        if (Stroka == "n") break;

        // ВВОД ДАННЫХ
        cout << "Введите название товара и его цену" << endl;
        cin >> B.nazv;
        cin >> B.price;
        getchar();

        // ОТПРАВКА СООБЩЕНИЙ СЕРВЕРУ
        sendto(my_sock, (char*)&B, sizeof(B), 0,
            (sockaddr*)&Daddr, sizeof(Daddr));

        // ПРИЕМ ОТВЕТА ОТ СЕРВЕРА
        sockaddr_in SRaddr;
        int SRaddr_size = sizeof(SRaddr);
        int n = recvfrom(my_sock, buff, sizeof(buff), 0,
            (sockaddr*)&SRaddr, &SRaddr_size);
        if (n == SOCKET_ERROR) {
            cout << "RECVFROM() ERROR:" << WSAGetLastError() << "\n";
            closesocket(my_sock);
            WSACleanup();  return -1;
        }
        buff[n] = '\0';
        cout << buff << "\n";
    }

    closesocket(my_sock);
    WSACleanup();
    return 0;
}