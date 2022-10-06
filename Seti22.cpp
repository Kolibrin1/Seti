#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

struct Person
{
    char name[25];
    int marks[4];
}A;

void main()
{
    setlocale(LC_ALL, "rus");
    const char* con = { "C:/Users/Admin/Desktop/Seti/conv.txt" };
    ofstream f_REQ;
    ifstream f_ANS;
    ofstream file0;
    fstream fC;
    long pred_size;
    int answer;

    fC.open(con);
    int temp;
    fC >> temp;
    fC.seekg(0, ios::beg);
    temp++;
    fC << temp << endl;;
    string stroka = to_string(temp);
    fC.close();
    string s2 = "C:/Users/Admin/Desktop/Seti/" + stroka + ".txt";
    const char* client = { s2.c_str() };
    while (true)
    {
        file0.open(client);
        f_ANS.open(client);
        f_ANS.seekg(0, ios::end);
        pred_size = f_ANS.tellg();
        cout << "Vvedite zapros: familiya, ocenki za sessiyu" << endl;
        cin >> A.name;
        for (int i = 0; i < 4; i++)
            cin >> A.marks[i];
        cout << A.name << endl;
        for (int i = 0; i < 4; i++)
            cout << A.marks[i];
        fC.open(con, ios::app | ios::in | ios::out);
        fC << A.name << endl;
        for (int i = 0; i < 4; i++)
            fC << A.marks[i] << endl;
        fC.close();


        while (pred_size == f_ANS.tellg())
        {
            Sleep(100);
            f_ANS.seekg(0, ios::end);
        }
        f_ANS.seekg(pred_size, ios::beg);
        f_ANS >> answer;
        pred_size = f_ANS.tellg();
        f_ANS.close();


        switch (answer)
        {
        case 0: {cout << "Sohranenie stipendii \n"; break; }
        case 1: {cout << "Povishennaya stipendiya \n"; break; }
        case 2: {cout << "vilet so stipendii \n"; break; }
        }

    }
    std::cout << "Hello World!\n";
}