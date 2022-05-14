#include "src/FALSE.cpp"

#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

#include "src/utils/files.cpp"
#include "src/utils/time_ms.cpp"

int main(int argc, char* argv[]) {
    char* c = argv[1];
    if (c == 0) {
        std::cout << "Provide a filename";
        return 1;
    }
    std::string filename(c);
    const char* program;
    int len;
    if (filename == "--raw") {
        program = argv[2];
        len = strlen(program);
    } else {
        std::string data = get_file_contents(filename);
        program = data.c_str();
        len = data.length();
    }

    std::chrono::milliseconds start = get_time_ms();
    int code = FALSE::interpret(program, len);
    std::chrono::milliseconds elapsed = get_time_ms() - start;

    std::cout << std::endl
              << "--------------------------------------------------------"
              << std::endl;
    std::cout << "Execution Time: " << elapsed.count() << " ms" << std::endl;
    std::cout << "Exit Code: " << code << std::endl;

    return 0;
}