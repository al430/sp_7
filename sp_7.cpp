// sp_7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#define DEBUG_MODE

using namespace std;

struct order {
    int order_number;
    string client_name;
    string product_name;
    int cost;
    string status;
};

vector<order> orders;

void load_from_file(string file_name) {
    ifstream input_file(file_name);
    if (!input_file.is_open()) {
        throw runtime_error("file_not_open");
    }

    string line;
    int count = 0;

    while (getline(input_file, line)) {
        if (line.empty()) continue;

        int semicolon_count = 0;
        for (char ch : line) {
            if (ch == ';') semicolon_count++;
        }
        if (semicolon_count != 4) {
            throw runtime_error("bad_line_format");
        }

        order current_order;
        string field;
        int field_index = 0;
        size_t start = 0;
        size_t end = line.find(';');

        while (end != string::npos) {
            field = line.substr(start, end - start);
            if (field_index == 0) {
                try {
                    current_order.order_number = stoi(field);
                }
                catch (...) {
                    throw runtime_error("bad_number_format");
                }
            }
            else if (field_index == 1) {
                current_order.client_name = field;
            }
            else if (field_index == 2) {
                current_order.product_name = field;
            }
            else if (field_index == 3) {
                try {
                    current_order.cost = stoi(field);
                }
                catch (...) {
                    throw runtime_error("bad_number_format");
                }
                if (current_order.cost < 0) {
                    throw runtime_error("negative_cost");
                }
            }
            field_index++;
            start = end + 1;
            end = line.find(';', start);
        }

        field = line.substr(start);
        if (field_index == 4) {
            current_order.status = field;
        }
        else {
            throw runtime_error("not_enough_fields");
        }

        orders.push_back(current_order);
        count++;
    }

    input_file.close();

#ifdef DEBUG_MODE
    cout << "loaded_records: " << count << endl;
#endif
}

void save_to_file(string file_name) {
    ofstream output_file(file_name);
    if (!output_file.is_open()) {
        throw runtime_error("file_not_open");
    }

    for (size_t i = 0; i < orders.size(); i++) {
        output_file << orders[i].order_number << ";"
            << orders[i].client_name << ";"
            << orders[i].product_name << ";"
            << orders[i].cost << ";"
            << orders[i].status << endl;
    }

    output_file.close();

#ifdef DEBUG_MODE
    cout << "saved_records: " << orders.size() << endl;
#endif
}

void search_by_number(int number) {
    bool found = false;
    for (size_t i = 0; i < orders.size(); i++) {
        if (orders[i].order_number == number) {
            cout << orders[i].order_number << " "
                << orders[i].client_name << " "
                << orders[i].product_name << " "
                << orders[i].cost << " "
                << orders[i].status << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "not_found" << endl;
    }
}

void search_by_client(string client) {
    bool found = false;
    for (size_t i = 0; i < orders.size(); i++) {
        if (orders[i].client_name.find(client) != string::npos) {
            cout << orders[i].order_number << " "
                << orders[i].client_name << " "
                << orders[i].product_name << " "
                << orders[i].cost << " "
                << orders[i].status << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "not_found" << endl;
    }
}

void show_not_issued() {
    bool found = false;
    for (size_t i = 0; i < orders.size(); i++) {
        if (orders[i].status != "issued") {
            cout << orders[i].order_number << " "
                << orders[i].client_name << " "
                << orders[i].product_name << " "
                << orders[i].cost << " "
                << orders[i].status << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "no_orders" << endl;
    }
}

void issue_order(int number) {
    for (size_t i = 0; i < orders.size(); i++) {
        if (orders[i].order_number == number) {
            if (orders[i].status == "issued") {
                throw runtime_error("already_issued");
            }
            orders[i].status = "issued";
            cout << "order_issued" << endl;
            return;
        }
    }
    throw runtime_error("order_not_found");
}

void total_issued_cost() {
    int total = 0;
    for (size_t i = 0; i < orders.size(); i++) {
        if (orders[i].status == "issued") {
            total += orders[i].cost;
        }
    }
    cout << "total_cost: " << total << endl;
}

int main() {
    string file_name = "orders.txt";

    try {
        load_from_file(file_name);
    }
    catch (const exception& e) {
        cout << "error: " << e.what() << endl;
        return 1;
    }

    int choice;
    do {
        cout << "1 - search_by_number" << endl;
        cout << "2 - search_by_client" << endl;
        cout << "3 - show_not_issued" << endl;
        cout << "4 - issue_order" << endl;
        cout << "5 - total_issued_cost" << endl;
        cout << "6 - save_and_exit" << endl;
        cout << "choice: ";
        cin >> choice;

        try {
            if (choice == 1) {
                int number;
                cout << "enter_number: ";
                cin >> number;
                search_by_number(number);
            }
            else if (choice == 2) {
                string client;
                cout << "enter_client: ";
                cin >> client;
                search_by_client(client);
            }
            else if (choice == 3) {
                show_not_issued();
            }
            else if (choice == 4) {
                int number;
                cout << "enter_number: ";
                cin >> number;
                issue_order(number);
            }
            else if (choice == 5) {
                total_issued_cost();
            }
            else if (choice == 6) {
                save_to_file(file_name);
                cout << "saved" << endl;
            }
            else if (choice != 0) {
                cout << "invalid_choice" << endl;
            }
        }
        catch (const exception e) {
            cout << "error: " << e.what() << endl;
        }
    } while (choice != 6);

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
