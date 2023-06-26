cmake_minimum_required(VERSION 3.25)
project(theencryptor)

set(CMAKE_CXX_STANDARD 20)

set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl@3)
set(OPENSSL_LIBRARIES ${OPENSSL_ROOT_DIR}/lib)
set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)

find_package(OpenSSL REQUIRED)

add_executable(theencryptor theencryptor/main.cpp theencryptor/commands.cpp)
target_include_directories(theencryptor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(theencryptor PRIVATE OpenSSL::SSL OpenSSL::Crypto)
