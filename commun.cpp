#include <string>
#include <iostream>

#include "commun.hpp"

std::string removeSpace (std::string line)
{
    while (line.find (" ") != line.npos)
    {
        line.erase (line.find (" "), 1);
    }

    return line;
}

void printError (int errorCode, std::string errorMessage)
{
    std::cout << "error!" << std::endl
              << "  Error code:    " << errorCode    << std::endl
              << "  Error message: " << errorMessage << std::endl;
}

