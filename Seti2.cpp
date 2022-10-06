#include <iostream>
#include <fstream>
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
    const char* nameR = "REQUEST.bin";
    const char* nameA = "ANSWER.bin";
    ofstream f_REQ;
    ifstream f_ANS;
    long pred_size;
    int answer;

    while (true)
    {
        cout << "Vvedite zapros: familiya, ocenki za sessiyu" << endl;
        cin >> A.name;
        for (int i = 0; i < 4; i++)
            cin >> A.marks[i];
        cout << A.name << endl;
        for (int i = 0; i < 4; i++)
            cout << A.marks[i];
        f_REQ.open(nameR, ios::app | ios::binary);
        f_REQ.write((char*)&A, sizeof(A));
        f_REQ.close();


        f_ANS.open(nameA, ios::binary);
        f_ANS.seekg(0, ios::end);
        pred_size = f_ANS.tellg();
        while (pred_size >= f_ANS.tellg())
        {
            Sleep(100);
            f_ANS.seekg(0, ios::end);
        }
        f_ANS.seekg(pred_size, ios::beg);
        f_ANS.read((char*)&answer, sizeof(answer));
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

