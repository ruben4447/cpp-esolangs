#include <iostream>
#include <string>
#include <map>

#include "./utils/is_char.cpp"

namespace brainfuck {
    // Create x-to-brainfuck map
    int brainfuck_to_x_map(std::map<std::string, char>* map, std::string a, std::string b, std::string c, std::string d, std::string e, std::string f, std::string g, std::string h) {
        map->clear();
        map->insert({ a, '>' });
        map->insert({ b, '<' });
        map->insert({ c, '+' });
        map->insert({ d, '-' });
        map->insert({ e, '.' });
        map->insert({ f, ',' });
        map->insert({ g, '[' });
        map->insert({ h, ']' });
        return 0;
    }

    // TO brainfuck. Takes a char map with arbitrary values for replacement.
    int to_brainfuck_arbitrary(std::string input, std::string *output, std::map<std::string, char>* sub_map, bool space_seperated, bool remove_whitespace, int start_pos)
    {
        if (remove_whitespace) {
            std::string old_input = input;
            input = "";
            for (int i = 0; i < old_input.length(); i++) if (!is_char_whitespace(old_input[i])) input += old_input[i];
        }

        int len = input.length();
        int i = start_pos;

        while (i < len) {
            if (space_seperated && is_char_whitespace(input[i])) {
                i++;
                continue;
            }

            bool found = false;
            for (auto item : *sub_map) {
                int sub_len = item.first.length();
                // std::cout << "Length: " << sub_len << "; Test: " << i + sub_len << " <= " << len << std::endl;
                if (i + sub_len <= len) {
                    // Substring
                    std::string substring = input.substr(i, sub_len);
                    // std::cout << "Match '" << substring << "' to '" << item.first << "' -> " << (substring == item.first) << std::endl;
                    if (substring == item.first) {
                        // Match!
                        *output += item.second;
                        found = true;

                        i += sub_len;
                        break;
                    }
                }
            }

            if (!found) {
                std::string c(1, input[i]);
                *output = "ERROR: unknown token \'" + c + "\' at position " + std::to_string(i) + "\n";
                *output += input.substr(0, i + 1) + " <-- HERE";
                return 1;
            }
        }

        return 0;
    }
};

// Namespace which contains conversion thingies
namespace brainfuck_sub_tables {
    std::map<std::string, char> ook, alphuck, binaryfuck, blub, colonoscopy, detailedfuck, uwu;
    bool is_init = false;

    void init() {
        is_init = true;

        brainfuck::brainfuck_to_x_map(&ook, "Ook. Ook?", "Ook? Ook.", "Ook. Ook.", "Ook! Ook!", "Ook! Ook.", "Ook. Ook!", "Ook! Ook?", "Ook? Ook!");
        brainfuck::brainfuck_to_x_map(&alphuck, "a", "c", "e", "i", "j", "o", "p", "s");
        brainfuck::brainfuck_to_x_map(&binaryfuck, "010", "011", "000", "001", "100", "101", "110", "111");
        brainfuck::brainfuck_to_x_map(&blub, "Blub. Blub?", "Blub? Blub.", "Blub. Blub.", "Blub! Blub!", "Blub! Blub.", "Blub. Blub!", "Blub! Blub?", "Blub? Blub!");
        brainfuck::brainfuck_to_x_map(&colonoscopy, ";};", ";{;", ";;};", ";;{;", ";;;};", ";;;{;", "{{;", "}};");
        brainfuck::brainfuck_to_x_map(&detailedfuck, "MOVE THE MEMORY POINTER ONE CELL TO THE RIGHT", "MOVE THE MEMORY POINTER ONE CELL TO THE LEFT", "INCREMENT THE CELL UNDER THE MEMORY POINTER BY ONE", "DECREMENT THE CELL UNDER THE MEMORY POINTER BY ONE", "REPLACE THE CELL UNDER THE MEMORY POINTER'S VALUE WITH THE ASCII CHARACTER CODE OF USER INPUT", "PRINT THE CELL UNDER THE MEMORY POINTER'S VALUE AS AN ASCII CHARACTER", "IF THE CELL UNDER THE MEMORY POINTER'S VALUE IS ZERO INSTEAD OF READING THE NEXT COMMAND IN THE PROGRAM JUMP TO THE CORRESPONDING COMMAND EQUIVALENT TO THE ] COMMAND IN BRAINFUCK", "IF THE CELL UNDER THE MEMORY POINTER'S VALUE IS NOT ZERO INSTEAD OF READING THE NEXT COMMAND IN THE PROGRAM JUMP TO THE CORRESPONDING COMMAND EQUIVALENT TO THE [ COMMAND IN BRAINFUCK");
        brainfuck::brainfuck_to_x_map(&uwu, "OwO", "°w°", "UwU", "QwQ", "@w@", ">w<", "~w~", "¯w¯");
    }
};

namespace brainfuck {
    // Convert to brainfuck given a from_format
    int to_brainfuck(std::string input, std::string *output, std::string format)
    {
        std::map<std::string, char> table;
        int start_pos = 0;
        bool space_seperated = true, remove_spaces = false;
        if (format == "ook")
        {
            table = brainfuck_sub_tables::ook;
        }
        else if (format == "blub")
        {
            table = brainfuck_sub_tables::blub;
        }
        else if (format == "alphuck")
        {
            table = brainfuck_sub_tables::alphuck;
            remove_spaces = true;
        }
        else if (format == "binaryfuck")
        {
            table = brainfuck_sub_tables::binaryfuck;
            remove_spaces = true;
            start_pos = 1;
            // Must lead with a ONE
            if (input[0] != '1')
            {
                std::string c(1, input[0]);
                *output = "SYNTAX ERROR: invalid binaryfuck program: expected leading 1, got " + c;
                return 1;
            }
        }
        else if (format == "colonoscopy")
        {
            table = brainfuck_sub_tables::colonoscopy;
            remove_spaces = true;
        }
        else if (format == "detailedfuck")
        {
            table = brainfuck_sub_tables::detailedfuck;
            space_seperated = true;
        }
        else if (format == "uwu")
        {
            table = brainfuck_sub_tables::uwu;
            space_seperated = true;
        }
        else
        {
            *output = "ERROR: Unknown format: '" + format + "'";
            return 1;
        }

        return to_brainfuck_arbitrary(input, output, &table, space_seperated, remove_spaces, start_pos);
    }
};