#include <iostream>
#include <string>

// Given position of bracket "[]", return position of macthing bracket
int get_matching_bracket(int pos, std::string program) {
    if (program[pos] != '[' && program[pos] != ']') {
        std::cout << "Unexpected token '" << program[pos] << "' at position "
                  << pos << ": cannot match token" << std::endl;
        exit(1);
    }
    int open_groups = 0, i = pos, len = program.length();
    int add = program[i] == '['
                  ? 1
                  : -1;  // Look forward if hnting closing, else backwards

    while (i > -1 && i < len) {
        if (program[i] == ']')
            open_groups--;
        else if (program[i] == '[')
            open_groups++;
        if (open_groups == 0) return i;
        i += add;  // Move to next token
    }

    std::cout << "No matching bracket found for '" << program[pos]
              << "' in position " << pos << std::endl;
    exit(1);
}