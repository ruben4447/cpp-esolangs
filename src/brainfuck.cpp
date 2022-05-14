#include <conio.h>

#include <iostream>
#include <string>

#include "./utils/matching_brackets.cpp"

namespace brainfuck {
// Interpret BrainF**k program. Return exit code.
int interpret(std::string program) {
    const int cells = 10000;
    unsigned char data[cells] = {0};
    int data_ptr = (cells / 2) - 1;

    int ptr = 0, len = program.length();

    while (ptr > -1 && ptr < len) {
        switch (program[ptr]) {
            case '<':
                data_ptr--;
                break;
            case '>':
                data_ptr++;
                break;
            case '+':
                data[data_ptr]++;
                break;
            case '-':
                data[data_ptr]--;
                break;
            case '.':
                putchar(data[data_ptr]);
                break;
            case ',':
                data[data_ptr] = getch();
                break;
            case '[':
                // SKip if data < 1
                if (data[data_ptr] < 1)
                    ptr = get_matching_bracket(ptr, program);
                break;
            case ']':
                // Loop to start if data > 0
                if (data[data_ptr] > 0)
                    ptr = get_matching_bracket(ptr, program);
                break;
        }

        ptr++;
    }

    return 0;
}
};  // namespace brainfuck