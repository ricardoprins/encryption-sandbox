cmake_minimum_required(VERSION 3.25)
project(theencryptor)

set(CMAKE_CXX_STANDARD 20)

add_executable(theencryptor theencryptor/main.cpp theencryptor/commands.cpp)
target_include_directories(theencryptor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/theencryptor)