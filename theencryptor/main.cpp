#include "commands.h"
#include <iostream>

int main() {
    std::cout << R"(
This is The Encryptor. Please enter your desired option:

1. Encrypt a file
2. Decrypt a file
3. Read a file
4. Exit

)" << '\n';
    return 0;
    // TODO: finish menu
}