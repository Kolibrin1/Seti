#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

struct Person
{
    char name[25];
    int marks[4];
}B;

int answer;
long poz_pred;

int main()
{
    ifstream fR;
    ofstream fA;
    fstream fC;
    setlocale(LC_ALL, "rus");
    const char* nameR = { "C:/Users/Admin/Desktop/Seti/REQUEST.txt" };
    const char* nameA = { "C:/Users/Admin/Desktop/Seti/ANSWER.txt" };
    const char* con = { "C:/Users/Admin/Desktop/Seti/conv.txt" };



    cout << "Server is working" << endl;
    fC.open(con, ios::in | ios::out);

    fC << 0 << endl;
    fC.close();
    while (true)
    {
        fC.open(con);
        fC.seekg(0, ios::end);
        poz_pred = fC.tellg();
        while (poz_pred == fC.tellg())
        {
            Sleep(100);
            fC.seekg(0, ios::end);
        }
        fC.seekg(0, ios::beg);
        int temp;
        fC >> temp;

        fC << temp << endl;
        string stroka = to_string(temp);
        string s2 = "C:/Users/Admin/Desktop/Seti/" + stroka + ".txt";
        const char* client = { s2.c_str() };

        fA.open(client, ios::app);

        fC.seekg(poz_pred, ios::beg);
        fC >> B.name >> B.marks[0] >> B.marks[1] >> B.marks[2] >> B.marks[3];
        poz_pred = fC.tellg();

        if (B.marks[0] < 4 || B.marks[1] < 4 || B.marks[2] < 4 || B.marks[3] < 4)
            answer = 2;
        else
            if (B.marks[0] == 5 && B.marks[1] == 5 && B.marks[2] == 5 && B.marks[3] == 5)
                answer = 1;
            else
                answer = 0;
        fC.seekg(0, ios::beg);

        fC.close();

        fA << answer;
        fA << endl;
        fA.close();
    }
}