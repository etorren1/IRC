#include "Server.hpp"
#include <algorithm>

bool    contains(std::vector<std::string> vect, std::string str)
{
    if (find(vect.begin(), vect.end(), str) != vect.end())
        return (true);
    return (false);
}

std::vector<std::string> split(std::string str, std::string delimiter)
{
    size_t pos = 0;
    std::string token;
    std::vector<std::string> res;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        res.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if (!str.empty())
        res.push_back(str);
    return (res);
}
