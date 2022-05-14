#include "src/DeathScript.cpp"

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
    std::string data = get_file_contents(filename);

    std::chrono::milliseconds start = get_time_ms();
    int code = DeathScript::interpret(data);
    std::chrono::milliseconds elapsed = get_time_ms() - start;

    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "Execution Time: " << elapsed.count() << " ms\n";
    std::cout << "Exit Code: " << code << "\n";

    return 0;
}