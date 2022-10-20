#include <iostream>
#include "fstream"
#include "String"
using namespace std;

struct Buyer {
    char name[25]{};
    int expgoods[3]{};
    string client_name{};
};


void listen(const string& file, int start_pos) {
    int cursor = start_pos;
    int answer;
    while (true) {
        ifstream in(file, ios::binary);
        if (in) {
            in.seekg(cursor);
            in.read((char*)&answer, sizeof(answer));
            if (cursor <= in.tellg()) {
                cout << "Skidka v %: " << answer << "\n";
                cursor += sizeof(answer);
                break;
            }
            in.close();
        }
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    string client_name;
    cout << "Введите имя клиента:\n";
    getline(cin, client_name);
    while (true) {
        Buyer me{};
        cout << "Введите свое имя:\n";
        cin >> me.name;
        cout << "Введите свои 3 самых дорогих товара:\n";
        for (int& expgood : me.expgoods)
            cin >> expgood;
        me.client_name = client_name;
        ofstream out("../server.bin", ios::binary | ios::app);
        out.write((char*)&me, sizeof(me));
        out.close();

        ifstream in("../clients/" + client_name + ".bin", ios::binary);
        int position;
        if (in) {
            in.seekg(0, ios::end);
            position = in.tellg();
            in.close();
        }
        else {
            position = 0;
        }
        listen("../clients/" + client_name + ".bin", position);
    }
    return 0;
}
