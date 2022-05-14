#include <string>
#include <fstream>

// Given a filename, return contents as std::string
std::string get_file_contents(std::string fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    std::string fileStr;

    std::istreambuf_iterator<char> inputIt(file), emptyInputIt;
    std::back_insert_iterator<std::string> stringInsert(fileStr);

    copy(inputIt, emptyInputIt, stringInsert);

    return fileStr;
}

int put_file_contents(std::string file_name, std::string input) {
    std::ofstream file(file_name);
    file << input;
    file.close();
    return 0;
}