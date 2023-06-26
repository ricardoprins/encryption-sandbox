#include "commands.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

namespace fs = std::filesystem;

// OpenSSLException class for error handling
class OpenSSLException : public std::exception {
public:
    explicit OpenSSLException(const char* message) : msg_(message) {}
    explicit OpenSSLException(std::string  message) : msg_(std::move(message)) {}
    ~OpenSSLException() noexcept override = default;

    [[nodiscard]] const char* what() const noexcept override {
        return msg_.c_str();
    }

protected:
    std::string msg_;
};

// function to get the file list from the current directory
std::vector<std::string> get_files() {
    std::vector<std::string> files;
    for (const auto &entry: fs::directory_iterator(".")) {
        if (entry.path().extension() == ".txt")
            files.push_back(entry.path().filename().string());
    }
    return files;
}

// function to get user input on which file to encrypt/decrypt
std::string get_file_choice() {
    auto files = get_files();
    if (files.empty()) {
        std::cout << "No text files found in the current directory.\n";
        return "";
    }
    int choice;
    for (int i = 0; i < files.size(); i++) {
        std::cout << i + 1 << ". " << files[i] << '\n';
    }
    std::cout << "Enter the number of your choice: ";
    std::cin >> choice;
    if (choice < 1 || choice > files.size()) {
        std::cout << "Invalid choice.\n";
        return "";
    }
    return files[choice - 1];
}

// File reading
std::string read_file(const std::string &filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        throw OpenSSLException("Failed to open file: " + filename);
    }
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    return buffer.str();
}

// Context initialization
EVP_CIPHER_CTX* init_context(const EVP_CIPHER* cipher, unsigned char* key, unsigned char* iv, int enc) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw OpenSSLException("Failed to create new cipher context.");
    if (1 != EVP_CipherInit_ex(ctx, cipher, nullptr, key, iv, enc)) {
        EVP_CIPHER_CTX_free(ctx);
        throw OpenSSLException("Failed to initialize cipher context.");
    }
    return ctx;
}

// Cipher Update
int update_cipher(EVP_CIPHER_CTX* ctx, std::vector<unsigned char>& out_text, const unsigned char* in_text, int in_len) {
    int len;
    if (1 != EVP_CipherUpdate(ctx, out_text.data(), &len, in_text, in_len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw OpenSSLException("Failed to update cipher.");
    }
    return len;
}

// Cipher Finalize
int finalize_cipher(EVP_CIPHER_CTX* ctx, std::vector<unsigned char>& out_text, int offset) {
    int len;
    if (1 != EVP_CipherFinal_ex(ctx, out_text.data() + offset, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw OpenSSLException("Failed to finalize cipher.");
    }
    return len;
}

// Base64 encoding
std::string encode_base64(const unsigned char* input, int length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, input, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);

    std::string output(bufferPtr->data, bufferPtr->length);
    BUF_MEM_free(bufferPtr);

    return output;
}

// Base64 decoding
std::vector<unsigned char> decode_base64(const std::string& input) {
    BIO *bio, *b64;
    std::vector<unsigned char> output(input.length());

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(input.c_str(), -1);
    bio = BIO_push(b64, bio);
    int outputLength = BIO_read(bio, output.data(), input.length());
    BIO_free_all(bio);

    if (outputLength <= 0) throw OpenSSLException("Failed to decode base64.");
    output.resize(outputLength);

    return output;
}

// Write to file
void write_file(const std::string& filename, const void* data, int length) {
    std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile) throw OpenSSLException("Failed to open file for writing: " + filename);
    outputFile.write(reinterpret_cast<const char*>(data), length);
}

// function to encrypt file
int cmd_encrypt(const std::string &filename, unsigned char *key, unsigned char *iv) {
    try {
        std::string tempFilename = filename + ".tmp";

        // Rename the original file to temporary file
        if (std::rename(filename.c_str(), tempFilename.c_str())) {
            throw OpenSSLException("File renaming failed");
        }

        // Read plaintext from file
        std::string plaintext = read_file(tempFilename);

        // Initialize the encryption operation
        EVP_CIPHER_CTX *ctx = init_context(EVP_aes_256_cbc(), key, iv, 1);

        // Provide the message to be encrypted, and obtain the encrypted output
        std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
        int len = update_cipher(ctx, ciphertext, reinterpret_cast<const unsigned char *>(plaintext.data()), plaintext.size());

        // Finalize the encryption
        len += finalize_cipher(ctx, ciphertext, len);

        // Convert the ciphertext to Base64 and write it to a new .txt file
        std::string base64Output = encode_base64(ciphertext.data(), len);
        write_file(filename, base64Output.c_str(), base64Output.size());

        // Delete the temporary file
        if (std::remove(tempFilename.c_str())) {
            throw OpenSSLException("Temporary file deletion failed");
        }

        // Clean up
        EVP_CIPHER_CTX_free(ctx);

        std::cout << "File " << filename << " encrypted successfully.\n";
        return 0;
    } catch (const OpenSSLException& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

// function to decrypt file
int cmd_decrypt(const std::string &filename, unsigned char *key, unsigned char *iv) {
    try {
        std::string tempFilename = filename + ".tmp";

        // Rename the original file to temporary file
        if (std::rename(filename.c_str(), tempFilename.c_str())) {
            throw OpenSSLException("File renaming failed");
        }

        // Read Base64 encoded text from file
        std::string base64Text = read_file(tempFilename);

        // Decode base64
        std::vector<unsigned char> base64Data = decode_base64(base64Text);

        // Initialize the decryption operation
        EVP_CIPHER_CTX *ctx = init_context(EVP_aes_256_cbc(), key, iv, 0);

        // Provide the message to be decrypted, and obtain the plaintext output
        std::vector<unsigned char> plaintext(base64Data.size());
        int len = update_cipher(ctx, plaintext, base64Data.data(), base64Data.size());

        // Finalize the decryption
        len += finalize_cipher(ctx, plaintext, len);

        // Write the decrypted data to a new .txt file
        write_file(filename, plaintext.data(), len);

        // Delete the temporary file
        if (std::remove(tempFilename.c_str())) {
            throw OpenSSLException("Temporary file deletion failed");
        }

        // Clean up
        EVP_CIPHER_CTX_free(ctx);

        std::cout << "File " << filename << " decrypted successfully.\n";
        return 0;
    } catch (const OpenSSLException& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

// function to read files in directory
void cmd_readfile(const std::string &filename) {
    try {
        if (filename.empty()) {
            return;
        }

        std::string content = read_file(filename);
        std::cout << content << '\n';
    } catch (const OpenSSLException& e) {
        std::cerr << e.what() << std::endl;
    }
}

// function to create four random .txt files to run the show!
void cmd_create_test_files() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 100);

    for (int i = 1; i <= 4; ++i) {
        std::string filename = "test0" + std::to_string(i) + ".txt";
        std::string content = "Random number: " + std::to_string(distribution(generator)) + '\n';
        write_file(filename, content.c_str(), content.size());
    }

    std::cout << "Test files created.\n";
}
