#pragma once
#include "math3d.h"

static const float EPSINON = 0.00001f;

enum RenderMode
{
	RM_WIREFRAME,
	RM_SOLID,
};

// ARGB format(a is 0)
DWORD RGBA(int r, int g, int b, int a);
// from Vec4 to DWORD
DWORD RGBA(Vec4 color);

// from DWORD to int[0, 255]
void toRGB(DWORD rbg, int &r, int &g, int &b);

float lerpFloat(float fStart, float fEnd, float t);
DWORD lerpColor(DWORD color1, DWORD color2, float t);
Vec4 lerpVector(Vec4 vStart, Vec4 vEnd, float t);

//static const DWORD ColorBlack	 = RGB(0, 0, 0);
//static const DWORD ColorWhite = RGB(255, 255, 255);
//static const DWORD ColorRED = RGB(255, 0, 0);
//static const DWORD ColorGREEN = RGB(0, 255, 0);
//static const DWORD ColorBLUE = RGB(0, 0, 255);

#define ColorBlack	RGB(0, 0, 0)
#define ColorWhite	RGB(255, 255, 255)
#define ColorRED	RGB(255, 0, 0)
#define ColorGREEN	RGB(0, 255, 0)
#define ColorBLUE	RGB(0, 0, 255)

//std::string numberToStr(int n) {//int to string
//	char numbuf[128];
//	sprintf_s(numbuf, "%i", n);//there is no itoa in xcode
//	std::string numStr = numbuf;
//	return numStr;
//}

//inline bool isFloatZero(float f)
//{
//	if ((f > -EPSINON) || (f < EPSINON)) return true;
//	else return false;
//}

void drawLabel(HDC hdc, LPCSTR str, int lineNum);