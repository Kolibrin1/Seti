#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

struct Person
{
    char name[25];
    int marks[4];
}B;

int answer;
long size_pred;

int main()
{
    ifstream fR;
    ofstream fA;
    setlocale(LC_ALL, "rus");
    const char* nameR = { "REQUEST.bin" };
    const char* nameA = { "ANSWER.bin" };

    cout << "Server is working" << endl;
    fR.open(nameR, ios::binary);
    fR.seekg(0, ios::end);
    size_pred = fR.tellg();
    fR.close();
    while (true)
    {
        fR.open(nameR, ios::binary);
        fR.seekg(0, ios::end);
        while (size_pred >= fR.tellg())
        {
            Sleep(100);
            fR.seekg(0, ios::end);
        }
        fR.seekg(size_pred, ios::beg);
        fR.read((char*)&B, sizeof(B));
        size_pred = fR.tellg();
        fR.close();
        if (B.marks[0] < 4 || B.marks[1] < 4 || B.marks[2] < 4 || B.marks[3] < 4)
            answer = 2;
        else
            if (B.marks[0] == 5 && B.marks[1] == 5 && B.marks[2] == 5 && B.marks[3] == 5)
                answer = 1;
            else
                answer = 0;
        fA.open(nameA, ios::binary | ios::app);
        fA.write((char*)&answer, sizeof(answer));
        fA.close();
    }
}
