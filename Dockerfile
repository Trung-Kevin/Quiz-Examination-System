FROM debian:bookworm-slim

RUN apt-get update && \
    apt-get install -y g++-mingw-w64-x86-64 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY src ./src
COPY data ./data

RUN x86_64-w64-mingw32-g++ -std=c++17 -mwindows src/*.cpp -o Quiz.exe