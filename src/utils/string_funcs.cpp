#include <string>
#include <vector>

// Split string by delimetter into vector of strings (inefficiently)
int string_split(std::string program, std::vector<std::string> *lines, char delimiter)
{
    std::string currString = "";

    for (int i = 0; i < program.length(); i++)
    {
        if (program[i] == delimiter)
        {
            lines->push_back(currString);
            currString = "";
        }
        else
        {
            currString += program[i];
        }
    }
    if (currString != "")
        lines->push_back(currString);
    return lines->size();
}