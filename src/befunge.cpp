#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <conio.h>

#include "utils/string_funcs.cpp"

// Code for befunge interpreter (nb h_xxx functions are helpers)
namespace befunge {
    // Type for stack processing
    typedef long num_t;

    // Helper to pop item of stack, or 0 if empty
    num_t h_pop(std::stack<num_t> *stack) {
        if (stack->empty()) return 0;
        num_t tmp = stack->top();
        stack->pop();
        return tmp;
    }

    // CHange movement vector
    void h_cmov(int* mrow, int* mcol, char c) {
        if (c == '<') {
            *mrow = 0;
            *mcol = -1;
        } else if (c == '>') {
            *mrow = 0;
            *mcol = 1;
        } else if (c == '^') {
            *mrow = -1;
            *mcol = 0;
        } else if (c == 'v') {
            *mrow = 1;
            *mcol = 0;
        } else {
            std::cout << "SYNTAX ERROR: Unknown movement operation: " << c << std::endl;
            exit(1);
        }
        // cout << "Move Vector -> " << *mrow << ":" << *mcol << endl;
    }

    void h_print(std::stack<num_t>* stack) {
        std::stack<num_t> dump = *stack;

        std::cout << "stack[" << stack->size() << "] : ";
        while (!dump.empty()) {
            std::cout << dump.top() << " ";
            dump.pop();
        }
    }

    void h_checkstack(std::stack<num_t>* stack, int min, int row, int col) {
        if (stack->size() < min) {
            std::cout << "STACK ERROR: stack too small; expected " << min << " items, got " << stack->size() << ".";
            std::cout << " (encountered at position " << row << ":" << col << ")" << std::endl;

            h_print(stack);
            exit(1);
        }
    }

    // Interpret befunge program
    int interpret(std::string program, bool pad_lines = true) {
        std::vector<std::string> lines;
        int line_count = string_split(program, &lines, '\n'); // Split into list of lines

        if (line_count == 0) {
            std::cout << "SYNTAX ERROR: Unexpected end of input at position 0:0" << std::endl;
            return 1;
        }

        // Generate new
        srand(time(NULL));

        // Pad all lines to same length (with spaces)
        if (pad_lines) {
            int max = lines[0].length();
            for (int i = 0; i < line_count; i++) {
                int len = lines[i].length();
                if (len > max) max = len;
            }
            for (int i = 0; i < line_count; i++) while (lines[i].length() < max) lines[i] += ' ';
        }

        int row = 0, col = 0;
        int mrow = 0, mcol = 1; // Movement (DEFAULT rtl, or '>')
        int wrap_limit = SHRT_MAX, wrap_times = 0;
        bool in_string = false;
        std::stack<num_t> stack;

        while (true) {
            // Check if OOB
            if (row < 0 || row >= lines.size() || col < 0 || col >= lines[row].length()) {
                // cout << "RUNTIME ERROR: Unexpected end of input at position " << row << ":" << col << " -> Expected movment operator or exit operator" << endl;   
                // Bounce back 180 degrees
                mrow *= -1;
                mcol *= -1;
                wrap_times++;

                if (wrap_times > wrap_limit) {
                    std::cout << "RUNTIME ERROR: Position wrap limit breached (" << wrap_limit << ") at position " << row << ":" << col << std::endl;
                    return 1;
                }
            } else {
                char c = lines[row][col];
                
                // Reset reflection if not direction control or invisible newline
                if (!in_string && c != '<' && c != '>' && c != '^' && c != 'v' && c != '\n' && c != '\r') {
                    wrap_times = 0;
                }

                // std::cout << "Token: " << c << " (str: " << in_string << ")" << std::endl;

                if (c == '"' || c == '\'') {
                    in_string = !in_string;
                } else if (in_string) {
                    stack.push(c);
                } else if (c >= '0' && c <= '9') {
                    int n = c - 48; // Convetr char num to integer
                    stack.push(n);
                } else if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                    // No-op
                } else if (c == '@') {
                    break; // Stop program execution
                } else if (c == '+') {
                    // Addition
                    num_t a = h_pop(&stack);
                    num_t b = h_pop(&stack);
                    stack.push(a + b);
                } else if (c == '-') {
                    // Subtraction
                    num_t a = h_pop(&stack);
                    num_t b = h_pop(&stack);
                    stack.push(b - a);
                } else if (c == '*') {
                    // Multiplication
                    num_t a = h_pop(&stack);
                    num_t b = h_pop(&stack);
                    stack.push(a * b);
                } else if (c == '/') {
                    // Integer Division
                    num_t a = h_pop(&stack);
                    num_t b = h_pop(&stack);
                    stack.push(b / a);
                } else if (c == '%') {
                    // Modulus
                    num_t a = h_pop(&stack);
                    num_t b = h_pop(&stack);
                    stack.push(b % a);
                } else if (c == '!') {
                    // Logical Not
                    num_t a = h_pop(&stack);
                    stack.push((a == 0) ? 1 : 0);
                } else if (c == '`') {
                    // Greater Than: b > a
                    num_t a = h_pop(&stack);
                    num_t b = h_pop(&stack);
                    stack.push((b > a) ? 1 : 0);
                } else if (c == '^' || c == '>' || c == '<' || c == 'v') {
                    // Move [up, right, left, down]
                    h_cmov(&mrow, &mcol, c);
                } else if (c == '?') {
                    // Move random direction
                    int n = rand() % 4; // Random number 0-4 (not inclusive)
                    char op = 'v';
                    if (n == 0) op = '<';
                    else if (n == 1) op = '>';
                    else if (n == 2) op = '^';
                    else op = 'v';

                    h_cmov(&mrow, &mcol, op);
                } else if (c == '_') {
                    // Pop a value; move right if value=0, left otherwise
                    num_t v = h_pop(&stack);
                    h_cmov(&mrow, &mcol, (v == 0) ? '>' : '<');
                } else if (c == '|') {
                    //	Pop a value; move down if value=0, up otherwise
                    num_t v = h_pop(&stack);
                    h_cmov(&mrow, &mcol, (v == 0L) ? 'v' : '^');
                } else if (c == ':') {
                    // Duplicate top value
                    if (stack.size() == 0) {
                        stack.push(0);
                    } else {
                        stack.push(stack.top());
                    }
                } else if (c == '\\') {
                    // Swap top stack values
                    num_t a = h_pop(&stack);
                    num_t b = h_pop(&stack);
                    stack.push(a);
                    stack.push(b);
                } else if (c == '$') {
                    // Remove value from top of stack
                    h_checkstack(&stack, 1, row, col);
                    stack.pop();
                } else if (c == '.') {
                    // Pop value, output as integer and space
                    h_checkstack(&stack, 1, row, col);
                    num_t v = stack.top(); stack.pop();
                    std::cout << v << " ";
                } else if (c == ',') {
                    // Output as ASCII
                    h_checkstack(&stack, 1, row, col);
                    int c = h_pop(&stack);
                    putchar(c);
                } else if (c == '#') {
                    // Skip next cell
                    row += mrow;
                    col += mcol;
                } else if (c == 'p') {
                    // A "put" call (a way to store a value for later use). Pop y, x, and v, then change the character at (x,y) in the program to the character with ASCII value v
                    // std::cout << "IMPLEMENTATION ERROR: put operator 'p' is not implemented" << std::endl;
                    h_checkstack(&stack, 3, row, col);
                    num_t y = h_pop(&stack);
                    num_t x = h_pop(&stack);
                    num_t v = h_pop(&stack);
                    std::cout << "Insert char at position " << x << "," << y << ": " << v << ": " << char(v) << std::endl;

                    if (y < 0 || y > line_count || x < 0 || x >= lines[y].length()) {
                        std::cout << "RUNTIME ERROR: position " << x << ", " << y << " is out of bounds" << std::endl;
                        return 1;
                    } else {
                        lines[x][y] = char(v);
                    }
                } else if (c == 'g') {
                    //	A "get" call (a way to retrieve data in storage). Pop y and x, then push ASCII value of the character at that position in the program
                    // std::cout << "IMPLEMENTATION ERROR: get operator 'g' is not implemented" << std::endl;
                    num_t y = h_pop(&stack);
                    num_t x = h_pop(&stack);
                    if (y < 0 || y > line_count || x < 0 || x >= lines[y].length()) {
                        // Out Of Bounds
                        stack.push(0);
                    } else {
                        char t = lines[x][y];
                        // std::string c(1, t);
                        // std::cout << "Push GET (" << x << ", " << y << "): " << c << std::endl;
                        stack.push(t);
                    }
                } else if (c == '&') {
                    // Ask for number and store it
                    int n = getch();
                    if (n >= '0' && n <= '9') {
                        stack.push(n - 48); // Convert char to int e.g. '9' to 9
                    } else {
                        std::cout << "TYPE ERROR: Expected input to be integer, got " << n << std::endl;
                        return 1;
                    }
                } else if (c == '~') {
                    // Ask for char and out ascii value
                    stack.push(getch());
                }
                
                else {
                    // Unknown?
                    std::cout << "SYNTAX ERROR: Unknown operator or token " << c << " (code: " << (int)c << ") at " << row << ":" << col << std::endl;
                    return 1;
                }
            }

            // Movement...
            if (mrow == 0 && mcol == 0) {
                std::cout << "RUNTIME ERROR: Movement vector is zero. (position " << row << ":" << col << ")" << std::endl;
                return 1;
            } else {
                row += mrow;
                col += mcol;
            }
        }

        return 0;
    }
};