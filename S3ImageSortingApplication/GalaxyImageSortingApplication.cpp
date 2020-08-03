#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <strsafe.h>
#include <string>
#include <io.h>
#include <sstream>
#include <boost/filesystem.hpp>
#include <direct.h>
#include <conio.h>
#include <boost/locale/info.hpp>

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
    wstring r(buf);
    delete[] buf;
    return r;
}

string stringAdjust(string input) {
    stringstream stream;
    //To make the path usable, all "\" have to be replaced with "\\"
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

bool exists(string p) //checks if a certain file on a certain path already exists
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
    path = stringAdjust(path); //make the path useable
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

string getCurrentDir() { //this function allows to retrieve the current working directory, in which the executable is located
    char buff[FILENAME_MAX]; 
    GetCurrentDir(buff, FILENAME_MAX);
    string currentWorkingDir(buff);
    return currentWorkingDir;
}

int main()
{
    WIN32_FIND_DATA FoundFileData;
    TCHAR szDir[MAX_PATH];
    size_t lengthOfArg;
    
    string location = getCurrentDir();
    char* cstr = new char[location.length() + 1];
    strcpy(cstr, location.c_str());

    TCHAR* inputPath = convertToTCHAR(cstr);

    StringCchLength(inputPath, MAX_PATH, &lengthOfArg);

    StringCchCopy(szDir, MAX_PATH, inputPath);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
    HANDLE hFind = FindFirstFile(szDir, &FoundFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return (-1);
    }  
    PTSTR filename = FoundFileData.cFileName;
    cout << "Welcome to GalaxyImageSorting" << endl << "Please answer some questions to determine the best configuration for you" << endl;
    cout << "Do you want your images to be sorted in directories named in German or English? (g/e)";
    char lang;
    cin >> lang;
    
    if (lang != 'e' && lang != 'g') {
        cout << "You have entered an invalid identifier" << endl;
        return -1;
    }

    cout << lang << endl;
    const char* months[12] = { "Januar", "Februar", "M\x84rz", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };
    if (lang == 'e') {
        const char* monthsE[12] = { "January", "February", "March", "April", "Mai", "June", "July", "August", "September", "October", "November", "December" };
        memcpy(months, monthsE, sizeof(months));
    }
    
    cout << "Please look at the name of the first file  in your list" << endl << "Count (starting at 1) at which position the first number of the month is placed and enter it" << endl;
    int x = 0;
    cin >> x;
    if (cin.fail()) {
        cout << "You have not entered a valid digit" << endl;
        return -1;
    }

    x = x - 1;

    if (x == -1) {
        cout << "0 is not a valid entry" << endl;
        return -1;
    }

    do {
        PTSTR filename = FoundFileData.cFileName;       
        size_t size = wcstombs(NULL, filename, 0);
        char* charStr = new char[size + 1];
        wcstombs(charStr, filename, size + 1); //Access to filename works for example like CharStr[15]
        if (filename[x] == '1') {
            string path = location;
            path = stringAdjust(path);
            switch (filename[x+1]) {
                case '0': 
                    copy(path, charStr, months[9]);
                    break;
                case '1':
                    copy(path, charStr, months[10]);
                    break;
                case '2':
                    copy(path, charStr, months[11]);
                    break;
            }
        }
        else {
            string path = location;
            path = stringAdjust(path);
            switch (filename[5]) {
            case '1':
                copy(path, charStr, months[0]);
                break;
            case '2':
                copy(path, charStr, months[1]);
                break;
            case '3':
                copy(path, charStr, months[2]);
                break;
            case '4':
                copy(path, charStr, months[3]);
                break;
            case '5':
                copy(path, charStr, months[4]);
                break;
            case '6':
                copy(path, charStr, months[5]);
                break;
            case '7':
                copy(path, charStr, months[6]);
                break;
            case '8':
                copy(path, charStr, months[7]);
                break;
            case '9':
                copy(path, charStr, months[8]);
                break;
            }
        }
    } while (FindNextFile(hFind, &FoundFileData) != 0);
    cout << "We have sorted your files for you!" << endl;
    cout << "Press any key to close this window and end the program" << endl;
    getch();

    return 0;
}