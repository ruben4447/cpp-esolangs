#include <conio.h>

#include "./utils/extract_n.cpp"
#include "./utils/matching_brackets.cpp"

#define MEM_SIZE 10000
#define CAL_SIZE 1000
#define WHL_SIZE 500

namespace FALSE {
/** Check for stack overflow */
int _sovfl(const int ptr) {
    if (ptr > MEM_SIZE) {
        printf("Stack Overflow: stack size limit reached {%i} \r\n", MEM_SIZE);
        return 1;
    }
    return 0;
}

/** Check for call stack overflow */
int _covfl(const int ptr) {
    if (ptr > CAL_SIZE) {
        printf("Stack Overflow: call stack size limit reached {%i} \r\n",
               CAL_SIZE);
        return 1;
    }
    return 0;
}

/** Check for while stack overflow */
int _wovfl(const int ptr) {
    if (ptr > WHL_SIZE) {
        printf("Stack Overflow: while stack size limit reached {%i} \r\n",
               WHL_SIZE);
        return 1;
    }
    return 0;
}

/** Check for stack underflow */
int _unfl(const int ptr) {
    if (ptr < 0) {
        printf("Stack Underflow \r\n");
        return 1;
    }
    return 0;
}

void pmem(int* mem, int start, int end) {
    if (start < 0) start = 0;
    while (start < end) {
        printf("%i", mem[start]);
        if (start + 1 < end) printf(", ");
        start++;
    }
    printf("\n");
}

// Interpret a FALSE program. Return exit code.
int interpret(const char* prog, int len) {
    int pidx = 0;                       // Index in prog
    int stack[MEM_SIZE] = {0}, sp = 0;  // Data stack
    int vars[26];  // Map variable symbols [a-z] to there respective value
    int cstack[CAL_SIZE] = {0}, csp = 0;  // Call stack
    int wstack[WHL_SIZE] = {0},
        wsp =
            0;  // While stack. [# pos; 1 = cond, 2 = body; cond pos; body pos]
    bool skip = false;  // Skip character
    int ret = 0;        // Return code. If == 0, break from loop

    int ic = 0;
    while (pidx > -1 && pidx < len) {
        char c = prog[pidx];
        ++ic;
        // printf("pidx=%i, sp=%i, csp=%i, wsp=%i \n", pidx, sp, csp, wsp);

        if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
            pidx++;
        } else if (c == '{') {  // CLOSE COMMENT
            skip = true;
            pidx++;
        } else if (c == '}') {  // OPEN COMMENT
            skip = false;
            pidx++;
        } else if (skip)
            pidx++;
        else if (c == 'M') {  // CUSTOM debug memory
            printf("[MEM] ");
            pmem(stack, 0, sp);
            pidx++;
        } else if (c == 'C') {  // CUSTOM debug call stack
            printf("[CAL] ");
            pmem(cstack, 0, csp);
            pidx++;
        } else if (c == 'V') {  // CUSTOM debug vars
            printf("[VARS] ");
            for (int j = 0; j < 26; ++j) {
                printf("%c=%i", j + 97, vars[j]);
                if (j < 25) printf(", ");
            }
            printf("\n");
            pidx++;
        } else if (c == 'P') {  // CUSTOM debug print program
            printf("[PROGRAM{%i}] ", len);
            int i = 0;
            while (i < len) {
                printf("%i", prog[i]);
                if (i + 1 < len) printf(", ");
                i++;
            }
            printf("\n");
            pidx++;
        } else if (c == 'E') {  // CUSTOM debug exit
            printf("INTERRUPT \n");
            ret = 2;
        } else if (c == '\'') {  // INSERT CHAR CODE
            stack[sp++] = prog[pidx + 1];
            ret = _sovfl(sp);
        } else if (c >= '0' && c <= '9') {  // INTEGER
            int n;
            if (prog[pidx + 1] >= '0' && prog[pidx + 1] <= '9') {
                n = extract_int(prog, len, pidx);
            } else {
                n = c - 48;
                pidx++;
            }
            // ADD: INTEGER n
            stack[sp++] = n;
            ret = _sovfl(sp);
        } else if (c == '"') {  // STRING
            pidx++;
            while (pidx < len && prog[pidx] != '"') {
                if (prog[pidx] == '\\') pidx++;  // Escape next character
                printf("%c", prog[pidx++]);
            }
            pidx++;
            // int slen = 0, i = ++pidx;
            // while (i < len && prog[i] != '"') {
            //     if (prog[i] == '\\') i++;  // Escape next character
            //     slen++;
            //     i++;
            // }
            // char* str = (char*)malloc(sizeof(char) * (slen + 1));
            // str[slen] = 0;  // Null-escape
            // i = 0;
            // while (i < slen) {
            //     if (prog[pidx] == '\\') pidx++;  // Escape next character
            //     str[i] = prog[pidx];
            //     pidx++;
            //     i++;
            // }
            // printf("%s", str);
            // free(str);
            // pidx++;
        } else if (c >= 'a' && c <= 'z') {  // VARIABLE: SYMBOL
            stack[sp++] = c;                // Just push character code
            pidx++;
        } else if (c == ':') {  // VARIABLE: STORE
            int s = stack[--sp], d = stack[--sp];
            if (s < 'a' || s > 'z') {
                printf("Expected symbol a-z, got %c \n", s);
                ret = 1;
            } else {
                vars[s - 97] = d;
            }
            pidx++;
        } else if (c == ';') {  // VARIABLE: GET
            int s = stack[--sp];
            if (s < 'a' || s > 'z') {
                printf("Expected symbol a-z, got %c \n", s);
                ret = 1;
            } else {
                stack[sp++] = vars[s - 97];
            }
            pidx++;
        } else if (c == '[') {  // LAMBDA: DEFINE
            stack[sp++] = ++pidx;
            int open = 1, instr = 0;
            while (ret == 0 && open > 0) {
                if (pidx >= len)
                    ret = 1;
                else {
                    if (instr) {
                        if (prog[pidx] == '\\')
                            ++pidx;
                        else if (prog[pidx] == '"')
                            instr = 0;
                    } else if (prog[pidx] == '"')
                        instr = 1;
                    else if (prog[pidx] == '[')
                        ++open;
                    else if (prog[pidx] == ']')
                        --open;
                    ++pidx;
                }
            }
        } else if (c == ']') {  // LAMBDA: CLOSE
            ret = _unfl(csp);
            if (ret == 0) {
                pidx = cstack[--csp];  // Restore old pointer
            }
        } else if (c == '!') {  // LAMBDA: APPLY
            int p = stack[--sp];
            cstack[csp++] = ++pidx;  // Save current pointer
            pidx = p;                // Move to lambda
            ret = _covfl(csp);
        } else if (c == '?') {  // LAMBDA: CONDITIONAL APPLY
            int p = stack[--sp], b = stack[--sp];
            ++pidx;
            if (b) {
                cstack[csp++] = pidx;  // Save current pointer
                pidx = p;              // Move to lambda
                ret = _covfl(csp);
            }
        } else if (c == '#') {                          // LAMBDA: WHILE
            if (wsp == 0 || wstack[wsp - 4] != pidx) {  // Setup while cache
                wstack[wsp++] = pidx;                   // # position
                wstack[wsp++] = 1;                      // Do Condition
                int b = stack[--sp], c = stack[--sp];
                wstack[wsp++] = c;  // Cond pos
                wstack[wsp++] = b;  // Body pos
            }
            if (wstack[wsp - 3] == 1) {  // Condition
                wstack[wsp - 3] = 2;
                cstack[csp++] = pidx;    // Return to '#'
                pidx = wstack[wsp - 2];  // Goto condition
                ret = _covfl(csp);
            } else {
                int n = stack[--sp];
                if (n) {  // Run body
                    wstack[wsp - 3] = 1;
                    cstack[csp++] = pidx;    // Return to '#'
                    pidx = wstack[wsp - 1];  // Goto body
                    ret = _covfl(csp);
                } else {
                    pidx++;
                    wsp -= 4;
                }
            }
        } else if (c == '+') {  // ARITHMETIC: ADD
            int a = stack[--sp], b = stack[--sp];
            stack[sp++] = b + a;
            pidx++;
        } else if (c == '-') {  // ARITHMETIC: SUBTRACT
            int a = stack[--sp], b = stack[--sp];
            stack[sp++] = b - a;
            ret = _sovfl(sp);
            pidx++;
        } else if (c == '*') {  // ARITHMETIC: MULTIPLY
            int a = stack[--sp], b = stack[--sp];
            stack[sp++] = b * a;
            pidx++;
        } else if (c == '/') {  // ARITHMETIC: DIVIDE
            int a = stack[--sp], b = stack[--sp];
            stack[sp++] = b / a;
            pidx++;
        } else if (c == '_') {  // ARITHMETIC: NEGATE
            int a = stack[--sp];
            stack[sp++] = -a;
            pidx++;
        } else if (c == '&') {  // LOGIC: AND
            int a = stack[--sp], b = stack[--sp];
            stack[sp++] = b && a;
            pidx++;
        } else if (c == '|') {  // LOGIC: OR
            int a = stack[--sp], b = stack[--sp];
            stack[sp++] = b || a;
            pidx++;
        } else if (c == '~') {  // LOGIC: NOT
            int a = stack[--sp];
            stack[sp++] = !a;
            pidx++;
        } else if (c == '>') {  // LOGIC: GREATER THAN
            int a = stack[--sp], b = stack[--sp];
            stack[sp++] = b > a;
            pidx++;
        } else if (c == '=') {  // LOGIC: EQUAL
            int a = stack[--sp], b = stack[--sp];
            stack[sp++] = b == a;
            pidx++;
        } else if (c == '$') {  // STACK OP: DUPLICATE
            stack[sp] = stack[sp - 1];
            sp++;
            ret = _sovfl(sp);
            pidx++;
        } else if (c == '%') {  // STACK OP: POP
            sp--;
            ret = _unfl(sp);
            pidx++;
        } else if (c == '\\') {  // STACK OP: SWAP
            int a = stack[--sp], b = stack[--sp];
            stack[sp++] = a;
            stack[sp++] = b;
            pidx++;
        } else if (c == '@') {  // STACK OP: ROTATE
            int a = stack[--sp], b = stack[--sp], c = stack[--sp];
            stack[sp++] = b;
            stack[sp++] = a;
            stack[sp++] = c;
            pidx++;
        } else if (prog[pidx] == -61 &&
                   prog[pidx + 1] == -72) {  // STACK OP: PICK 'ø'
            int i = stack[--sp], n = stack[sp - 1 - i];
            stack[sp++] = n;
            pidx += 2;
        } else if (prog[pidx] == -62 &&
                   prog[pidx + 1] == -82) {  // STACK OP: ROTATE '®'
            int i = stack[--sp], n = stack[sp - i];
            int j = sp - i;
            while (j < sp) {
                stack[j] = stack[j + 1];
                j++;
            }
            sp--;
            stack[sp++] = n;
            pidx += 2;
        } else if (c == '^') {  // IO: INPUT
            int c = getch();
            stack[sp++] = c;
            pidx++;
        } else if (c == '.') {  // IO: OUTPUT AS INT
            int n = stack[--sp];
            printf("%i", n);
            ret = _unfl(sp);
            pidx++;
        } else if (c == ',') {  // IO: OUTPUT AS CHAR
            int n = stack[--sp];
            printf("%c", n);
            ret = _unfl(sp);
            pidx++;
        } else if (prog[pidx] == -61 &&
                   prog[pidx + 1] == -97) {  // IO: FLUSH 'ß'
            fflush(nullptr);
            pidx += 2;
        }

        else {
            printf("Unknown character %c (%#05x) at %i \n", c, c, pidx);
            ret = 1;
        }

        if (ret != 0) break;
    }

    if (ret == 1) printf("Error at index %i \n", pidx);
    return ret;
}
};  // namespace FALSE