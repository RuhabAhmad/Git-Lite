#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include "FileHandling.h"
#include "CustomString.h"
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>
#include <stdio.h>
#include <set>
#include <iomanip>

using namespace std;

int getLatestVersion() {
    CustomString versionFile = "FilesToREAD\\versions.txt";
    ifstream file(versionFile);

    string line, lastLine;

    while (getline(file, line)) {
        // Ignore empty lines or lines with only whitespace
        if (!line.empty() && line.find_first_not_of(" \t") != CustomString::npos) {
            lastLine = line;
        }
    }

    file.close();

    if (lastLine.empty()) {
        return 0;
    }

    return stoi(lastLine);
}




CustomString fileName = "healthcare_dataset.txt";

class AddressLocation
{
public:
    CustomString filename;
    CustomString linenumber;
    AddressLocation() {};
    AddressLocation(CustomString f, CustomString l)
    {
        filename = f;
        linenumber = l;
    }
};

bool alphaCheck(const CustomString& str)
{
    for (int i = 0; i < str.size(); ++i)
    {
        char s = str[i];
        if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z'))
        {
            if (!(s >= '0' && s <= '9') || !(s == '.'))
                return true;
        }
    }
    return false;
}

bool DecimalCheck(const CustomString& str)
{
    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] == '.')
            return true;
    }
    return false;
}

int countChar(CustomString& str, char ch)
{
    int count = 0;
    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] == ch)
            ++count;
    }
    return count;
}

CustomString toLower(CustomString f)
{
    CustomString out = "";
    for (int i = 0; i < f.size(); i++)
    {
        if (isupper(f[i]))
            out += tolower(f[i]);

        else
            out += f[i];
    }
    return out;
}

typedef string CustomString;
using CustomString = string;

CustomString toUpper(CustomString f)
{
    CustomString out = "";
    for (int i = 0; i < f.size(); i++)
    {
        if (islower(f[i]))
            out += toupper(f[i]);

        else
            out += f[i];
    }
    return out;
}

CustomString DetermineDataType(const CustomString& str)
{
    if (alphaCheck(str))
        return "string";

    if (DecimalCheck(str))
        return "double";

    return "int";
}

int comparestring(CustomString key, CustomString val)
{
    if (key == val)
        return 0;

    if (DetermineDataType(key) == "string")
    {
        return strcmp(key.c_str(), val.c_str());
    }

    else if (DetermineDataType(key) == "int")
    {
        if (key.size() < val.size())
            return -1;

        else if (key.size() > val.size())
            return 1;

        for (int i = 0; i < key.size(); i++)
        {
            if (key[i] < val[i])
                return -1;

            if (key[i] > val[i])
                return 1;
        }
    }

    else
    {
        if (key.find('.') > val.find('.'))
            return 1;

        if (key.find('.') < val.find('.'))
            return -1;

        for (int i = 0; i < key.size(); i++)
        {
            if (key[i] < val[i])
                return -1;
            if (key[i] > val[i])
                return 1;
        }
    }
}

CustomVector<CustomString> split(const string& str, char delimiter) {
    CustomVector<string> result;
    stringstream sstream(str);
    string token;
    while (getline(sstream, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

string join(const CustomVector<string>& fields, char delimiter) {
    string result;
    for (int i = 0; i < fields.getSize(); i++) {
        if (i > 0) result += delimiter; // Add delimiter between fields
        result += fields[i];
    }
    return result;
}


void AllocateDataTypes(CustomVector<string>& DataType, CustomVector<string>& Fline)
{
    //CustomVector<string> out;
    for (int i = 0; i < Fline.getSize(); ++i)
    {
        string dataType = DetermineDataType(Fline[i]);
        DataType.push_back(dataType);
    }

    //return out;
}

int getFieldIndex(CustomVector<string>& fields, string fName)
{
    for (int i = 0; i < fields.getSize(); i++)
        if (toLower(fName) == toLower(fields[i]))
            return i;
    return -1;
}

void READLINE(fstream& file, CustomVector<string>& list)
{
    list.resize(0);
    string FirstLine, word = "";
    getline(file, FirstLine);
    stringstream Fline(FirstLine);

    while (getline(Fline, word, ','))
    {
        if (word.size() < 1)
            continue;
        if (countChar(word, '"') % 2 == 1)
        {
            string temp;
            getline(Fline, temp, '"');
            word += "," + temp + "\"";
        }
        list.push_back(word);
    }
}