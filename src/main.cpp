#include <iostream>
#include <cstdlib>

int main(int argc, char ** argv) {
    std::cout << "In main.cpp:\n"
        << "Calling fetch_data.py\n";

    std::system("python3 src/fetch_data.py AAPL");
}