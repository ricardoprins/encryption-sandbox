#ifndef THEENCRYPTOR_COMMANDS_H
#define THEENCRYPTOR_COMMANDS_H

#include <vector>
#include <string>

int cmd_encrypt(const std::vector<std::string>& args);
int cmd_decrypt(const std::vector<std::string>& args);
int cmd_readfile(const std::vector<std::string>& args);

#endif //THEENCRYPTOR_COMMANDS_H