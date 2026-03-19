#include "MyLib.h"

std::string MyLib::ChangePathToName(const std::string& path)
{
    const size_t lastSlash = path.find_last_of("\\/");
    const size_t lastDot = path.find_last_of(".");
    
    const size_t start = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
    std::string name;
    if (lastDot != std::string::npos && lastDot > start)
    {
        name = path.substr(start, lastDot - start);
    }
    else
    {
        name = path.substr(start);
    }
    return name;
}
