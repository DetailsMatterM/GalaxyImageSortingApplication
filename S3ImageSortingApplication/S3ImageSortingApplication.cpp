// S3ImageSortingApplication.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <strsafe.h>
#include <string>
#include "atlstr.h"

#define MAX 60

using namespace std;

TCHAR* convertToTCHAR(char* cha)
{
    int aa = strlen(cha);
    TCHAR* tmp = new TCHAR[aa + 1];
    for (int i = 0; i < aa + 1; i++)
    {
        tmp[i] = cha[i];
    }
    return tmp;
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
        if (FoundFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            _tprintf(TEXT("  %s   <DIR>\n"), FoundFileData.cFileName);
        else
            _tprintf(TEXT("  %s \n"), FoundFileData.cFileName);
    } while (FindNextFile(hFind, &FoundFileData) != 0);


}