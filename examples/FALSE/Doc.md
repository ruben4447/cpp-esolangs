Memory consists of a large `int` stack

# Debug
Special characters created for debugging purposes

- `C`: view call stack
- `E`: Halt program
- `M`: view memory stack
- `P`: print currently loaded program
- `V`: view variable contents

# Literals
- *Integers* are pushed on to the stack e.g. `42`
- *Characters* the character code is pushed on to the stack e.g. `'a` pushes `97`
- *Strings* prints the string to STDOUT e.g. `"Hello"`

# Comments
Enclosed by `{ ... }`

# Arithmetic
- `+`: Pop values `a` and `b`. Push `b + a`.
- `-`: Pop values `a` and `b`. Push `b - a`.
- `*`: Pop values `a` and `b`. Push `b * a`.
- `/`: Pop values `a` and `b`. Push `b / a`.
- `_`: Pop value `a`. Push `-a`.

# Logical
- `&`: Pop values `a` and `b`. Push `b && a`.
- `|`: Pop values `a` and `b`. Push `b || a`.
- `~`: Pop value `a`. Push `!a`.
- `>`: Pop values `a` and `b`. Push `b > a`.
- `=`: Pop values `a` and `b`. Push `b == a`.

# Stack Operations
- `$`: Duplicate top item of stack
- `%`: Pop topmost item
- `\`: Swap top items - `a, b` -> `b, a`
- `@`: Rotate third item to top - `a, b, c` -> `b, c, a`
- `ø`: Pop value `a`. Push value in position `stack[sp - 1 - a]` to top of stack.
- `®`: Pop value `a`. **Move** value in position `stack[sp - a]` to top of stack.

# I/O
- `^`: Wait for character input, push character code to stack. (getch)
- `.`: Pop `a` and print as an integer (%i).
- `,`: Pop `a` and print as a character (%c).
- `ß`: Flush all streams.

# Variables
Variables are characters `a-z`

- `:` sets a variable e.g. `1a:` stores the value `1` in variable `a`.
- `;` gets a variable e.g. `a;` pushes the value of `a` to the stack.

# Lambdas
Lamdas are functions defined by square brackets `[...]` which contain code. When encoutered, they are pushed to the stack.

- `!`: Pop value `a` from the stack. Call lambda `a`.
- `?`: Pop values `a` and `b` from the stack. Call lambda `b` if `a` is truthy.
- `#`: While loop. Syntax: `[<cond>][<body>]#`. While `cond` is true, do `body`.