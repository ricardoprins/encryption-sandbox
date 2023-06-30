# The Encryptor

## Introduction
Welcome to The Encryptor! This application is a simple, yet powerful tool that leverages the OpenSSL library to provide secure encryption and decryption services. It's designed to be cross-platform, running on Mac, Windows, and Linux. Written in C++, it incorporates best coding practices, ensuring its maintainability and scalability.

## Features
- Encrypt or decrypt files in your local directory using AES (Advanced Encryption Standard) algorithm.
- Generate random files for encryption/decryption testing.
- Exception handling with custom OpenSSLException.
- Interactive menu for ease of operation.

## Installation

### Prerequisites
Ensure you have installed the following dependencies on your system:
- C++ compiler (with C++20 support)
- CMake (minimum version 3.25)
- OpenSSL library

### Building the project
1. Clone this repository to your local machine.

    ```bash
    git clone https://github.com/ricardoprins/encryption-sandbox.git
    ```

2. Navigate into the project directory.

    ```bash
    cd encryption-sandbox
    ```

3. Create a new directory for the build, and navigate into it.

    ```bash
    mkdir build && cd build
    ```

4. Configure the project with CMake.

    ```bash
    cmake ..
    ```

5. Build the project.

    ```bash
    cmake --build .
    ```

### Docker-based Setup

If you have Docker installed on your machine, you can build and run the program without needing to manually install any of the prerequisites.

1. Clone this repository to your local machine.

    ```bash
    git clone https://github.com/ricardoprins/encryption-sandbox.git
    ```

2. Navigate into the project directory.

    ```bash
    cd encryption-sandbox
    ```

3. Build the Docker image.

    ```bash
    docker build -t theencryptor .
    ```

4. Run the Docker image.

    ```bash
    docker run -it theencryptor
    ```

## Usage

After successful compilation, you can run the program by executing the created binary.

```bash
./theencryptor
```

The program will present an interactive menu allowing you to choose from encrypting a file, decrypting a file, viewing the contents of a file, and generating random test files.

For the Docker setup, the Docker container starts the program automatically when it runs.

## Contribute

Contributions are welcome! If you find any bugs or wish to add new features, feel free to create a pull request.

## License

This project is open source, under the [MIT License](LICENSE).

## Disclaimer

This application is meant to be a demonstration of how OpenSSL can be used for encryption and decryption in C++. It should not be used for securing sensitive data or in any security-critical contexts.
