#include "ImageerEngineer.h"
void ImageerEngineer::OpenImage(unsigned char* img, int height, int width, int nrChannel, map<float, char> symbolDict) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top;

    float scaleX = (float)width / consoleWidth;
    float scaleY = (float)height / consoleHeight;

    for (int y = 0; y < consoleHeight; y++) {
        //optimized cout
        string Line;
        for (int x = 0; x < consoleWidth; x++) {
            int srcX = (int)(x * scaleX);
            int srcY = (int)(y * scaleY);
            srcX = min(srcX, width - 1);
            srcY = min(srcY, height - 1);

            int index = (srcY * width + srcX) * nrChannel;
            //red green blue
            int r = img[index];
            int g = img[index + 1];
            int b = img[index + 2];

            float Y = .299f * r + .587f * g + .114f * b;
            //i tried to add some async, but line seems to be work better
            Line += getCharFromBrightness(Y, symbolDict);
        }
        cout << Line << endl;
    }
}
void PlayMP3(string fileName) {
    //command for console
    string command = "open \"" + fileName + ".mp3\" type mpegvideo alias bgm";
    //sending command
    mciSendStringA(command.c_str(), NULL, 0, NULL);
    mciSendStringA("play bgm", NULL, 0, NULL);
}

void ImageerEngineer::PlayViedo(const char* imgPath, map<float, char> symbolDict, int fps, string startName)
{
    //capturing video
    VideoCapture cap(imgPath);
    Mat frame;
    //getting console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //playing mp3
    PlayMP3(startName);
    if (!cap.isOpened()) {
        cout << "Error: Could not open video." << endl;
        return;
    }

    cout << "STARTED!" << endl;
    //getting ms for frame
    int delay = (fps > 0) ? (1000 / fps) : (1000 / cap.get(CAP_PROP_FPS));
    cout << delay << endl;

    while (true) {
        //hide cursor
        std::cout << "\033[?25l";

        cap >> frame;
        if (frame.empty()) break;
        //getting buffer for setting up cursor
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        SetConsoleCursorPosition(hConsole, { 0, 0 });
        //open image
        OpenImage(frame.data, frame.rows, frame.cols, frame.channels(), symbolDict);
        //wait for next frame
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
}

char ImageerEngineer::getCharFromBrightness(float brightness, map<float, char> symbolDict) {
    if (symbolDict.empty()) return ' ';

    auto it = symbolDict.lower_bound(brightness);
    //if ts is in start, this is in start
    if (it == symbolDict.begin()) {
        return it->second;
    }
    if (it == symbolDict.end()) {
        return prev(it)->second;
    }
    auto prevIt = prev(it);

    float diffCurrent = abs(it->first - brightness);
    float diffPrev = abs(prevIt->first - brightness);
    return ((diffCurrent < diffPrev) ? it->second : prevIt->second);
}