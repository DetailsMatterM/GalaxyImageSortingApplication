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
    /*string path = argv[1];
    cout << path << endl;
    path = stringAdjust(path);*/
    /*string newPath = newPath.append("\\\\Oktober");
    makeDir(newPath);
    bool checker = dirExists(newPath);
    cout << newPath << endl;*/

    

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
                string newPath = path + "\\\\Oktober";
                bool checker = exists(newPath);
                
                if (checker == false) {      
                    makeDir(newPath);
                }
                else {
                    //get filepath and then copy the file to the new location
                    string filePathOld = path + "\\\\" + charStr;
                    cout << filePathOld << endl;
                    string filePathNew = newPath + "\\\\" + charStr;
                    cout << filePathNew << endl;
                    filesystem::copy_file(filePathOld, filePathNew); //need better path to file
                }
            }
        }
        /*else {
            switch (filename[5]) {
            case 1:

            }
        }*/
        _tprintf(TEXT("  %s \n"), FoundFileData.cFileName);
    } while (FindNextFile(hFind, &FoundFileData) != 0);


}