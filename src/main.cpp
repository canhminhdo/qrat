//
// Created by CanhDo on 2024/08/29.
//

#include <iostream>
#include "getch.h"
#include "Person.h"

using namespace std;

int boostrap() {
    int c;
    qrat::Person p;
    cout << "Name: " << p.getName() << endl;
    cout << "Hello, world!" << endl;
    while ((c = qrat::getch()) != EOF) {
        cout << static_cast<char>(c);
    }
    return 0;
}
