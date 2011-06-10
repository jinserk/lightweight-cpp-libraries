//
// main.cpp
//

#include <iostream>
#include <iomanip>

#include "framework.h"

using namespace std;
using namespace framework;

void test_logstream(void);
void test_array(void);
void test_buffer(void);

int main(int argc, char* argv[], char* envp[])
{
    //test_logstream();
    //test_array();
    test_buffer();

    return 0;
}

void test_logstream(void)
{
    logstream log("log.txt");

    log << "both!\n";

    log.cout_off();
    log << "file only!\n";

    log.cout_on();
    log.fout_off();
    log << "crt only!\n";

    log.cout_off();
    log << "both off\n";

    log.cout_on();
    log.fout_on();
    log << "both on\n";

    log.printf("test %d %s %f\n", 10, "string", 1.);
    log << endl;
}

void test_array(void)
{
    array<int, 2> A(4, 3);
    array<int, 2> B;

    A = 1, 2, 3,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12;

    try {
        B[0][0] = -1;
    } catch (array_exception e) {
        SHOW(e);
    }

    B = A;

    A.resize(2, 4);
    A = 0, 1, 2, 3, 4, 5, 6, 7;

    cout << setw(2) << setfill('0') << 9 << endl;
    cout << setw(2) << setfill('0') << B << endl;
    cout << A[1] << endl;
    cout << B.at(2) << endl;

    try {
        A[4][2] = 99;
    } catch (array_exception e) {
        SHOW(e);
    }

    array<int, 1> C;
    C.set_size(3);
    C = 1, 2, 3, 4;

    array<int, 1> D = C;
    cout << D << endl;
}

void test_buffer(void)
{
    buffer<int> A;
    
    try {
        A.push(10);
    } catch (array_exception e) {
        SHOW(e);
    }

    A.set_size(10);

    for (int i = 0; i < 20; ++i) {
        A.push(i);
        cout << setw(2) << setfill('0') << A << endl;
    }
    for (int i = 0; i < 11; ++i) {
        try {
            int x = A.pop();
            cout << setw(2) << setfill('0') << x << '\t' << A << endl;
        } catch (array_exception e) {
            SHOW(e);
        }
    }
}
