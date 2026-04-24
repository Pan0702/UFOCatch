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

bool MyLib::IsSameFormat(const std::string& path, const std::string& format)
{
    const size_t lastDot = path.find_last_of(".");
    std::string ext;
    if (lastDot != std::string::npos)
    {
         ext = path.substr(lastDot + 1); //   
    }
    if (format == ext)
    {
        return true;
    }

    return false;
}
