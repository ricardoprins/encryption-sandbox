#include "commands.h"
#include <iostream>
#include <openssl/rand.h>

constexpr int KEY_LENGTH = 256 / 8;  // AES-256
constexpr int IV_LENGTH = 128 / 8;   // AES block size

void generate_random_bytes(unsigned char *buffer, int length) {
    if (RAND_bytes(buffer, length) != 1) {
        std::cerr << "Error generating random bytes.\n";
        exit(1);
    }
}

int main() {
    int choice;
    unsigned char key[KEY_LENGTH];
    unsigned char iv[IV_LENGTH];
    generate_random_bytes(key, KEY_LENGTH);
    generate_random_bytes(iv, IV_LENGTH);

    while (true) {
        std::cout << R"(
This is The Encryptor. Please enter your desired option:

1. Encrypt a file
2. Decrypt a file
3. Read a file
4. Create test files
5. Exit

)" << '\n';

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid option.\n";
            continue;
        }

        std::string filename;
        if (choice == 1) {
            std::cout << "Which file do you want to encrypt?\n";
            filename = get_file_choice();
            if (!filename.empty()) {
                cmd_encrypt(filename, key, iv);
            } else {
                // Error handling
                std::cout << "No file selected or file not found. Please try again.\n";
            }
        } else if (choice == 2) {
            std::cout << "Which file do you want to decrypt?\n";
            filename = get_file_choice();
            if (!filename.empty()) {
                cmd_decrypt(filename, key, iv);
            } else {
                // Error handling
                std::cout << "No file selected or file not found. Please try again.\n";
            }
        } else if (choice == 3) {
            std::cout << "Which file do you want to read?\n";
            filename = get_file_choice();
            if (!filename.empty()) {
                cmd_readfile(filename);
            } else {
                // Error handling
                std::cout << "No file selected or file not found. Please try again.\n";
            }
        } else if (choice == 4) {
            cmd_create_test_files();
        } else if (choice == 5) {
            std::cout << "Thanks for using The Encryptor!\n";
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
