#ifndef THEENCRYPTOR_COMMANDS_H
#define THEENCRYPTOR_COMMANDS_H

#include <vector>
#include <string>

int cmd_encrypt(const std::string& filename, unsigned char *key, unsigned char *iv);
int cmd_decrypt(const std::string& filename, unsigned char *key, unsigned char *iv);
std::string get_file_choice();
void cmd_readfile(const std::string& filename);
void cmd_create_test_files();
std::vector<std::string> get_files();

#endif //THEENCRYPTOR_COMMANDS_H