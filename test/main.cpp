//
// main.cpp
//

#include "framework.h"

namespace fw = ::framework;

int main(int argc, char* argv[], char* envp[])
{
    fw::logstream log("log.txt");

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

    return 0;
}

