#ifndef HTMLUTILS_H
#define HTMLUTILS_H

#include <string>
#include <vector>

using namespace std;

class HtmlUtils
{
public:
    HtmlUtils();
    static string formatRequest(string, char, string);
    static string extractHost(string header);
    static string replaceInHeader(string, string, string);
private:
    static size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
};

#endif // HTMLUTILS_H
