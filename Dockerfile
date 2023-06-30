# Start from a base image
FROM ubuntu:latest

# Install build tools and libraries
RUN apt-get update && apt-get install -y \
    build-essential \
    openssl \
    libssl-dev \
    git \
    wget \
    libgl1-mesa-dev

# Download and install cmake
WORKDIR /tmp
RUN wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1-Linux-x86_64.sh \
    && chmod +x cmake-3.25.1-Linux-x86_64.sh \
    && ./cmake-3.25.1-Linux-x86_64.sh --skip-license --prefix=/usr/local

# Set the working directory in the Docker container
WORKDIR /usr/src/app

# Copy your source code into the Docker container
COPY . .

# Build the C++ project
RUN mkdir build && cd build && cmake .. && make

# Run the C++ program
CMD ["./build/theencryptor"]
