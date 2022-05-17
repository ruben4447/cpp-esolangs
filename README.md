# About
A collection of interpreters for some esoteric languages
[Esolang Wiki](https://esolangs.org/wiki/Main_Page)

# Interpreters
- `brainfuck.exe`       Intrepret brainf*ck files
    - Command Line: `./brainfuck.exe <file>.bf` or `./brainfuck.exe --raw <brainfuck-code>`
- `brainfuck_sub.exe`   Transform brainf\*ck-inspired languages, such as alpuck, into brainf\*ck files
    - Command Line: `./brainfuck_sub.exe <format> <src_file> <output_file>`
        - Format: format of file (if alphuck, enter "alphuck")
        - src_file: Source file to transpile
        - output_file: file to dump generated brainfu*k code into, which is able to be run by `brainfuck.exe`
- `befune.exe`      Interpret Befunge files
    - Command Line: `./befunge.exe <file>`
- `cell.exe`      Interpret FALSE files
    - Command Line: `./cell.exe <file>`
- `FALSE.exe`      Interpret FALSE files
    - Command Line: `./FALSE.exe <file>`

# Supported Esolangs
- Brainf\*ck (`./brainfuck.exe`)
- Brainf\*ck substitutions (`./brainfuck_sub.exe`)
    - alphuck
    - binaryf*ck
    - blub
    - colonoscopy
    - ook
    - uwu
- Befunge (`./befunge.exe`)
- Cell (`./cell.exe`)
- FALSE (`./FALSE.ece`)

# Build
g++ <src_file> -o <out_file> -Os
