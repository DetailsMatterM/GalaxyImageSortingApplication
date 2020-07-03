#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <strsafe.h>
#include <string>
#include "atlstr.h"
#include <io.h>
#include <direct.h>
#include <sstream>
#include <boost/filesystem.hpp>
#include <stdlib.h>
#include <direct.h>
#define GetCurrentDir _getcwd

#define MAX 60

using namespace std;
using namespace boost;

TCHAR* convertToTCHAR(char* cha)
{
    int length = strlen(cha);
    TCHAR* tmp = new TCHAR[length + 1];
    for (int i = 0; i < length + 1; i++)
    {
        tmp[i] = cha[i];
    }
    return tmp;
}

wstring s2ws(const std::string& s)
{
    int len;
    int sLength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), sLength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), sLength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

string stringAdjust(string input) {
    stringstream stream;
    for (int i = 0; i < input.length(); ++i) {
        if (input[i] == '\\') {
            stream << "\\\\";
        }
        else {
            stream << input[i];
        }
    }
    string output = stream.str();
    return output;
}

void makeDir(string path) {  
    wstring stemp = s2ws(path);
    LPCWSTR pathToUse = stemp.c_str();
    CreateDirectory(pathToUse, NULL);
}

bool exists(string p)
{
    if (filesystem::exists(p))
        return true;
    else
        return false;
}

void copyFile(string path, string newPath, char* charStr) {
    string filePathOld = path + "\\\\" + charStr;
    string filePathNew = newPath + "\\\\" + charStr;
    try {
        filesystem::copy_file(filePathOld, filePathNew);
    }
    catch (boost::filesystem::filesystem_error e) {
        cout << "File already copied" << endl;
    }
    
}

void copy(string path, char* charStr, string month) {
    path = stringAdjust(path);
    string newPath = path + "\\\\" + month;
    bool checker = exists(newPath);
    if (checker == false) {
        makeDir(newPath);
        copyFile(path, newPath, charStr);
    }
    else {
        copyFile(path, newPath, charStr);
    }
}

string getCurrentDir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    string currentWorkingDir(buff);
    return currentWorkingDir;
}

int main(int argc, char* argv[])
{
    WIN32_FIND_DATA FoundFileData;
    TCHAR szDir[MAX_PATH];
    size_t length_of_arg;
    
    string location = getCurrentDir();
    char* cstr = new char[location.length() + 1];
    strcpy(cstr, location.c_str());

    TCHAR* inputPath = convertToTCHAR(cstr);

    StringCchLength(inputPath, MAX_PATH, &length_of_arg);

    if (length_of_arg > (MAX_PATH - 3))
    {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }

    StringCchCopy(szDir, MAX_PATH, inputPath);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
    HANDLE hFind = FindFirstFile(szDir, &FoundFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return (-1);
    }  

    do {
        PTSTR filename = FoundFileData.cFileName;
        size_t size = wcstombs(NULL, filename, 0);
        char* charStr = new char[size + 1];
        wcstombs(charStr, filename, size + 1); //Access to filename works for example like CharStr[15]
        if (filename[4] == '1') {
            string path = location;
            path = stringAdjust(path);
            switch (filename[5]) {
                case '0': 
                    copy(path, charStr, "Oktober");
                    break;
                case '1':
                    copy(path, charStr, "November");
                    break;
                case '2':
                    copy(path, charStr, "Dezember");
                    break;
            }
        }
        else {
            string path = location;
            path = stringAdjust(path);
            switch (filename[5]) {
            case '1':
                copy(path, charStr, "Januar");
                break;
            case '2':
                copy(path, charStr, "Februar");
                break;
            case '3':
                copy(path, charStr, "März");
                break;
            case '4':
                copy(path, charStr, "April");
                break;
            case '5':
                copy(path, charStr, "Mai");
                break;
            case '6':
                copy(path, charStr, "Juni");
                break;
            case '7':
                copy(path, charStr, "Juli");
                break;
            case '8':
                copy(path, charStr, "August");
                break;
            case '9':
                copy(path, charStr, "September");
                break;
            }
        }
    } while (FindNextFile(hFind, &FoundFileData) != 0);
}