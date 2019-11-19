#include "htmlutils.h"
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <QDebug>

using namespace std;

HtmlUtils::HtmlUtils()
{

}

string HtmlUtils::formatRequest(string header, char splitDelimiter, string joinDelimiter)
{
    std::vector<std::string> v;

    split( header, v, splitDelimiter );

    v.erase(std::remove_if(v.begin(), v.end(),
                           mem_fun_ref(&std::string::empty)));

//    std::string delimiter = "\r\n";
    std::stringstream  s;

    // join
    std::copy(v.begin(),v.end(), std::ostream_iterator<std::string>(s, joinDelimiter.c_str()));

    return s.str();
}

size_t HtmlUtils::split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}
