#include <iostream>
#include "ImageConvert/ImageerEngineer.h"
using namespace std;


string symbols = " .'^_+*!%&$@";
map<float, char> symbolDict;
int main()
{
    for (int i = 0; i < symbols.length(); i++) {
        const char c = symbols[i];
        symbolDict[(float)((255.0f / symbols.length()) * i)] = c;
    }
    ImageerEngineer imgVid;
    //can be anything
    string name = "BadApple/badapple";
    //uncomment it for any video, but it must have mp4 and mo3
    //cin >> name;
    imgVid.PlayViedo((name + ".mp4").c_str(), symbolDict, 0, name);
}

