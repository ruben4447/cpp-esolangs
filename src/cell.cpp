#include <conio.h>

#include <string>

#define CELLS 30000

namespace cell {
int interpret(std::string prog) {
    // Hi
    int cells[CELLS] = {0};
    int cp = 0, i = 0, l = prog.length();

    while (i < l) {
        char cmd = prog[i];
        std::string data;
        for (int j = i + 1; j < l && prog[j] != ' '; data += prog[j], ++j)
            ;
        i += 1 + data.length() + 1;  // Increment
        if (cmd == 'j') {            // JUMP
            cp = std::stoi(data);
        } else if (cmd == 'n') {  // NEXT
            ++cp;
        } else if (cmd == 'p') {  // PREVIOUS
            --cp;
        } else if (cmd == 'a') {  // ASSIGN
            cells[cp] = std::stoi(data);
        } else if (cmd == '+') {  // INC
            ++cells[cp];
        } else if (cmd == '-') {  // DEC
            --cells[cp];
        } else if (cmd == '.') {  // PRINT ASCII
            printf("%c", cells[cp]);
        } else if (cmd == ',') {  // INPUT ASCII
            cells[cp] = getch();
        } else if (cmd == '{') {
            if (cells[cp] == 0) {  // Move to }
                int open = 1, err = 0, j = i;
                while (err == 0 && open > 0) {
                    if (j >= l)
                        err = 1;
                    else {
                        if (prog[j] == '{')
                            ++open;
                        else if (prog[j] == '}')
                            --open;
                        ++j;
                    }
                };
                if (err == 1) {
                    printf("No matching } found for { at position %i\n",
                           i - 2 - data.length());
                    return 1;
                } else {
                    i = j - 1;
                }
            }
        } else if (cmd == '}') {
            if (cells[cp] != 0) {  // Move to }
                int closed = 1, instr = 0, err = 0, j = i - 3 - data.length();
                while (err == 0 && closed > 0) {
                    if (j < 0)
                        err = 1;
                    else {
                        if (prog[j] == '{')
                            --closed;
                        else if (prog[j] == '}')
                            ++closed;
                        --j;
                    }
                };
                if (err == 1) {
                    printf("No matching { found for } at position %i\n",
                           i - 2 - data.length());
                    return 1;
                } else {
                    i = j + 1;
                }
            }
        }
    }

    // Check cell pointer
    if (cp < 0 || cp >= CELLS) {
        printf("Cell pointer %i out of bounds (0-%i) \n", cp, CELLS);
        return 1;
    }
    return 0;
}
};  // namespace cell