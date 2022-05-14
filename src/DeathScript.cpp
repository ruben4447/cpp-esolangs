#include <Windows.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "utils/string_funcs.cpp"

namespace DeathScript {
int interpret(const std::string prog) {
    std::srand(std::time(0));

    int i = 0, l = prog.length();
    std::string A, B, C;
    while (i < l) {
        if (prog[i] == '\n' || prog[i] == '\r')
            i++;
        else if (prog.substr(i, 6) == "output") {
            std::string out = get_to_EOL(prog, i + 7);
            if (out == "\\n" || out == "\\r") {
                std::cout << "\n";
            } else {
                std::cout << out;
            }
            i += 7 + out.length() + 1;
        } else if (prog.substr(i, 6) == "outvar") {
            i += 7;
            if (prog[i] == 'A')
                std::cout << A;
            else if (prog[i] == 'B')
                std::cout << B;
            else if (prog[i] == 'C')
                std::cout << C;
            i += 2;
        } else if (prog.substr(i, 5) == "input") {
            i += 6;
            std::string in;
            std::cin >> in;
            if (prog[i] == 'A')
                A = in;
            else if (prog[i] == 'B')
                B = in;
            else if (prog[i] == 'C')
                C = in;
            i += 2;
        } else if (prog.substr(i, 3) == "set") {
            i += 4;
            char v = prog[i];
            i += 2;
            std::string val;
            if (prog.substr(i, 6) == "random") {
                int n = std::rand();
                val = std::to_string(n);
            } else {
                val = get_to_EOL(prog, i);
            }
            if (v == 'A')
                A = val;
            else if (v == 'B')
                B = val;
            else if (v == 'C')
                C = val;
            i += val.length() + 1;
        } else if (prog.substr(i, 5) == "clear") {
            system("cls");
            i += 6;
        } else if (prog.substr(i, 6) == "switch") {
            i += 7;
            char v = prog[i];
            i += 2;
            if (v == 'B') {
                std::string tmp = A;
                A = B;
                B = tmp;
            } else if (v == 'C') {
                std::string tmp = A;
                A = C;
                C = tmp;
            }
        } else if (prog.substr(i, 4) == "math") {
            i += 5;
            std::string act = prog.substr(i, 3);
            i += 4;
            if (act == "add")
                A = std::to_string(std::stof(B) + std::stof(C));
            else if (act == "sub")
                A = std::to_string(std::stof(B) - std::stof(C));
            else if (act == "mul")
                A = std::to_string(std::stof(B) * std::stof(C));
            else if (act == "div")
                A = std::to_string(std::stof(B) / std::stof(C));
            else if (act == "mod")
                A = std::to_string(std::stol(B) % std::stol(C));
        } else {
            printf("Unknown character %c at position %i \n", prog[i], i);
            return 1;
        }
    }
    return 0;
}
}  // namespace DeathScript