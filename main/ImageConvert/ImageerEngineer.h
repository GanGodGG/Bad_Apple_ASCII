#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include <mmsystem.h>
#include <future>
#pragma comment(lib, "winmm.lib")
using namespace std;
using namespace chrono_literals;
using namespace cv;
class ImageerEngineer
{
private:
	char getCharFromBrightness(float brightness, map<float, char> symbolDict);
public:
	void OpenImage(unsigned char* img, int height, int width, int nrChannel, map<float, char> symbolDict);
	
	void PlayViedo(const char* imgPath, map<float, char> symbolDict, int fps, string startName);


};

