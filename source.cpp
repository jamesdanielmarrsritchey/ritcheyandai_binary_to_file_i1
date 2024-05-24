#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

std::string getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return std::string(*itr);
    }
    return "";
}

bool processFile(const std::string& source_file, const std::string& destination_file, const std::string& delimiter) {
    std::ifstream infile(source_file);
    std::ofstream outfile(destination_file);
    std::string line;
    while (std::getline(infile, line)) {
        if (delimiter.empty()) {
            for (size_t i = 0; i < line.size(); i += 8) {
                std::string byte = line.substr(i, 8);
                outfile << static_cast<char>(std::stoi(byte, nullptr, 2));
            }
        } else {
            size_t pos = 0;
            std::string token;
            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                outfile << static_cast<char>(std::stoi(token, nullptr, 2));
                line.erase(0, pos + delimiter.length());
            }
            outfile << static_cast<char>(std::stoi(line, nullptr, 2));
        }
    }
    outfile.close();
    infile.close();
    return true;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    std::string source_file = getCmdOption(argv, argv + argc, "--source_file");
    std::string destination_file = getCmdOption(argv, argv + argc, "--destination_file");
    std::string delimiter = getCmdOption(argv, argv + argc, "--delimiter");

    if (delimiter == "\\n") {
        delimiter = "\n"; // treat \n as line ending
    } else if (delimiter == "\\t") {
        delimiter = "\t"; // treat \t as tab
    }

    if (!source_file.empty() && !destination_file.empty()) {
        bool success = processFile(source_file, destination_file, delimiter);
        return success ? 0 : 1;
    } else {
        std::cout << "No source file or destination file was given." << std::endl;
        return 1;
    }
}