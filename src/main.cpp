#include <iostream>
#include "CmdParser.h"

std::size_t parse_n(char const *str) {
    auto n = static_cast<std::size_t>(strtoll(str, nullptr, 0));
    return n;
}

int main(int argc, char const *argv[]) {
    auto n = (argc == 2) ? parse_n(argv[1]) : 0;

    if(n == 0) {
        std::cout << "Wrong argument." << std::endl;
        return 1;
    }

    CmdParser bulk(n);
    bulk.process();
    return 0;
}
