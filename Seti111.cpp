#include <iostream>
#include "fstream"
#include <filesystem>

using namespace std;

struct Buyer {
    char name[25]{};
    int expgoods[3]{};
    string client_name{};
};
int answer;

int average_price(int expgoods[3]) {
    int avg_price = 0;
    for (auto i = 0; i < 3; i++) {
        avg_price += expgoods[i];
    }
    avg_price = avg_price / 3;
    return avg_price;
}

int get_answer(int expgoods[3]) {
    if (average_price(expgoods) >= 1500)
        return 15;
    else if (average_price(expgoods) >= 1000)
        return 10;
    else if (average_price(expgoods) >= 500)
        return 5;
    else
        return 0;
}

void listen(const string& file, int cursor) {
    Buyer student{};
    while (true) {
        ifstream in(file, ios::binary);
        in.seekg(cursor);
        in.read((char*)&student, sizeof(student));
        if (cursor < in.tellg()) {
            answer = get_answer(student.expgoods);
            cout << "Name: " << student.name << "\nSkidka v %: " << answer << "\n";
            ofstream out("../clients/" + student.client_name + ".bin", ios::binary | ios::app);
            out.write((char*)&answer, sizeof(answer));
            out.close();
            cursor += sizeof(student);
        }
        in.close();
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    cout << "Server is working..." << endl;
    string file = "../server.bin";
    ifstream in(file, ios::binary);
    in.seekg(0, ios::end);
    int position = in.tellg();
    in.close();
    listen(file, position);
    return 0;
}

