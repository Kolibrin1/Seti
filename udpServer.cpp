#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable: 4996)
using namespace std;
#define PORT 1234    // порт сервера
string answ = "empty";

//SERVER

struct goods
{
    string nazv;
    int price;
}A;

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char buff[1024];
    cout << "UDP DEMO ECHO-SERVER\n";
    // Winsocks lib _init_
    if (WSAStartup(0x202, (WSADATA*)&buff[0]))
    {
        cout << "WSAStartup error: " << WSAGetLastError();
        return -1;
    }
    // открытие сокета
    SOCKET Lsock;
    Lsock = socket(AF_INET, SOCK_DGRAM, 0);
    if (Lsock == INVALID_SOCKET) {
        cout << "SOCKET() ERROR: " << WSAGetLastError();
        WSACleanup();
        return -1;
    }
    // связывание сокета с локальным адресом 
    sockaddr_in Laddr;
    Laddr.sin_family = AF_INET;
    Laddr.sin_addr.s_addr = INADDR_ANY;
    Laddr.sin_port = htons(PORT);
    if (bind(Lsock, (sockaddr*)&Laddr, sizeof(Laddr)))
    {
        cout << "BIND ERROR:" << WSAGetLastError();
        closesocket(Lsock);
        WSACleanup();
        return -1;
    }
    // обработка пакетов, присланных клиентами
    while (1) {
        // Получение данных
        sockaddr_in Caddr;
        int Caddr_size = sizeof(Caddr);
        int bsize = recvfrom(Lsock, (char*)&A, sizeof(buff) - 1, 0,
            (sockaddr*)&Caddr, &Caddr_size);
        cout << A.nazv << " " << A.price << endl;

        if (bsize == SOCKET_ERROR)
            cout << "RECVFROM() ERROR:" << WSAGetLastError();
        // Определяем IP-адрес клиента и прочие атрибуты
        HOSTENT* hst;
        hst = gethostbyaddr((char*)&Caddr.sin_addr, 4, AF_INET);
        cout << "NEW DATAGRAM!\n" <<
            ((hst) ? hst->h_name : "Unknown host") << '\n' <<
            inet_ntoa(Caddr.sin_addr) << '\n' << ntohs(Caddr.sin_port) << '\n';
        buff[bsize] = '\0';              // добавление завершающего нуля

        // посылка датаграммы клиенту
        int discount = 0;
        if (A.price >= 3000)
            discount = 15;
        else if (A.price >= 2000 && A.price < 3000)
            discount = 10;
        else if (A.price >= 1000 && A.price < 2000)
            discount = 5;
        else
            discount = 0;

        answ = "Стоимость " + A.nazv + " с учетом скидки: " + to_string(A.price - discount * A.price / 100) + " рублей";

        sendto(Lsock, (char*)&answ[0], answ.size(), 0, (sockaddr*)&Caddr, sizeof(Caddr));
    }      return 0;
}