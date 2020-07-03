// S3ImageSortingApplication.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

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
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
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

bool dirExists(const string& dirName_in)
{
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
         
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        return true;
    }

    return false; 
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
    //cout << filePathOld << endl;
    string filePathNew = newPath + "\\\\" + charStr;
    //cout << filePathNew << endl;
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

int main(int argc, char* argv[])
{
    WIN32_FIND_DATA FoundFileData;
    TCHAR szDir[MAX_PATH];
    size_t length_of_arg;
    
    TCHAR* inputPath = convertToTCHAR(argv[1]);

    if (argc != 2)
    {
        _tprintf(TEXT("\nUsage: %s <directory name>\n"), inputPath);
        return (-1);
    }
    
    StringCchLength(inputPath, MAX_PATH, &length_of_arg);

    if (length_of_arg > (MAX_PATH - 3))
    {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }

    _tprintf(TEXT("\nTarget directory is %s\n\n"), inputPath);

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
            string path = argv[1];
            cout << path << endl;
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
            string path = argv[1];
            cout << path << endl;
            path = stringAdjust(path);
            string newPath;
            bool checker;
            switch (filename[5]) {
            case '1':
                newPath = path + "\\\\Januar";
                checker = exists(newPath);
                if (checker == false) {
                    makeDir(newPath);
                    copyFile(path, newPath, charStr);
                }
                else {
                    copyFile(path, newPath, charStr);
                }
                break;
            case '2':
                newPath = path + "\\\\Februar";
                checker = exists(newPath);
                if (checker == false) {
                    makeDir(newPath);
                    copyFile(path, newPath, charStr);
                }
                else {
                    copyFile(path, newPath, charStr);
                }
                break;

            }
        }
        _tprintf(TEXT("  %s \n"), FoundFileData.cFileName);
    } while (FindNextFile(hFind, &FoundFileData) != 0);


}