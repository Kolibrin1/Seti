/* для разрыва соединения клиент должен ввести строку “quit” */
#include <iostream>
#include <winsock2.h>  
#include <string>
#include <sstream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)
#pragma comment (lib,"Ws2_32.lib")
using namespace std;
u_short MY_PORT = 666;  // Порт, который слушает сервер         
// макрос для печати количества активных  пользователей 
#define PRINTNUSERS if (nclients)\
  cout << " user on-line " << nclients <<endl;\
  else cout << "No User on line\n";
/* прототип функции, обслуживающий  подключившихся клиентов  */
DWORD WINAPI ConToClient(LPVOID client_socket);
// глобальная переменная – количество  активных пользователей 
int nclients = 0;
struct BuyerData
{
    string sername;
    int maxprice[3];
    string skidka;
};
int main() {
    char buff[1024];    // Буфер для различных нужд
    cout << "TCP SERVER DEMO\n";
    // Шаг 1 - Инициализация Библиотеки Сокетов
    if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
    {
        cout << "Error WSAStartup \n" << WSAGetLastError();
        return -1;
    } 
// Шаг 2 - создание сокета
    SOCKET mysocket;
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {  
        cout << "Error socket \n" << WSAGetLastError();
        WSACleanup();  
        return -1;
    }
    // Шаг 3 связывание сокета с локальным адресом
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(MY_PORT);
    local_addr.sin_addr.s_addr = 0;
    // вызываем bind для связывания
    if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
    {     
        cout << "Error bind \n" << WSAGetLastError();
        closesocket(mysocket); 
        WSACleanup();
        return -1;
    }
    // Шаг 4 ожидание подключений
       
    if (listen(mysocket, 0x100))// размер очереди – 0x100
    {  
        cout << "Error listen: " << WSAGetLastError();
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }
    cout << "Waiting connections\n";
    // Шаг 5 извлекаем сообщение из очереди
    SOCKET client_socket;    // сокет для клиента
    sockaddr_in client_addr;    // адрес клиента
    // функции accept необходимо передать размер структуры
    int client_addr_size = sizeof(client_addr);
    // цикл извлечения запросов на подключение из  очереди
    while ((client_socket = accept(mysocket, (sockaddr*)
        &client_addr, &client_addr_size)))
    {
        nclients++;   // увеличиваем счетчик  клиентов
        HOSTENT* hst;  // пытаемся получить имя хоста
        hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);
        cout << "+new connect!\n";    // вывод сведений о клиенте
        if (hst) cout << hst->h_name;    else cout << "";
        cout << inet_ntoa(client_addr.sin_addr);
        PRINTNUSERS           cout << endl;
        DWORD thID; // Вызов нового потока для клиента
        CreateThread(NULL, NULL, ConToClient, &client_socket, NULL, &thID);
    }     return 0;
}
/* Эта функция создается в отдельном потоке и  обсуживает очередного подключившегося клиента независимо от остальных */
DWORD WINAPI ConToClient(LPVOID client_socket)
{
    SOCKET my_sock;   
    int len;
    my_sock = ((SOCKET*)client_socket)[0];
    char buff[1024];
    char sHELLO[] = "Hello, Buyer, send me your sername and 3 maxprice goods \r\n";
    send(my_sock, sHELLO, sizeof(sHELLO), 0);
    // отправляем клиенту приветствие 
     // цикл эхо: прием строки и  возвращение ее клиенту
    while (SOCKET_ERROR !=
        (len = recv(my_sock, (char*)&buff[0], 1024, 0)))
    {
        buff[len] = '\0';    
        cout << "getted:" << buff << endl;
        istringstream s(buff);
        BuyerData bD;
        s >> bD.sername;
        s >> bD.maxprice[0];
        s >> bD.maxprice[1];
        s >> bD.maxprice[2];


        double avg = (bD.maxprice[0] + bD.maxprice[1] + bD.maxprice[2])/3;
        int avg_const = 0;
        if (avg >= 3000 ) {
            bD.skidka = "20%";
            avg_const = 3000;
        }
        else if (avg >= 2000) {
            bD.skidka = "15%";
            avg_const = 2000;
        }
        else if (avg >= 1000) {
            bD.skidka = "10%";
            avg_const = 1000;
        }
        else{
            bD.skidka = "скидки нет";
        }
        string answ;
        if(avg >= 1000)
            answ = "Так как средняя цена товаров = " + to_string((int)avg) + " >= " + to_string((int)avg_const) + ", то скидка составит " + bD.skidka;
        else
            answ = "Так как средняя цена товаров = " + to_string((int)avg) + " < 1000, то " + bD.skidka;
        send(my_sock, answ.c_str(), answ.size(), 0);
    }
    // произошел выход из цикла, соединение c клиентом разорвано
    nclients--;     // уменьшаем счетчик активных клиентов
    cout << "-disconnect\n";
    PRINTNUSERS
        closesocket(my_sock);     
    return 0;
}
