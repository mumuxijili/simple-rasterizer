#pragma once
#include "math3d.h"
#include "mesh.h"
#include "camera.h"

enum CubeVertex
{
	flt, frt, flb, frb, blt, brt, blb, brb
};
// front-left-top, frt, flb, frb, blt, brt, blb, brb
extern Vertex g_cube[];

static const float EPSINON = 0.00001f;

enum RenderMode
{
	RenderMode_WIREFRAME,
	RenderMode_SOLID,
};

// ARGB format(a is 0)
DWORD RGBA(int r, int g, int b, int a);
// from Vec4 to DWORD
DWORD RGBA(Vec4 color);

// from DWORD to int[0, 255]
void fromRGB(DWORD rbg, int &r, int &g, int &b);

// fxxk
void fromRGBQUAD(DWORD rbg, int &r, int &g, int &b);
//inline DWORD toRGBQUAD(int r, int g, int b) { return RGB(b, g, r); }
#define toRGBQUAD(r,g,b)          ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))

float lerpFloat(float fStart, float fEnd, float t);
DWORD lerpColor(DWORD color1, DWORD color2, float t);
Vec4 lerpVector(Vec4 vStart, Vec4 vEnd, float t);

Vertex lerpVertex(Vertex vStart, Vertex vEnd, float t);

#define ColorBlack	toRGBQUAD(0, 0, 0)
#define ColorWhite	toRGBQUAD(255, 255, 255)
#define ColorRed	toRGBQUAD(255, 0, 0)
#define ColorGreen	toRGBQUAD(0, 255, 0)
#define ColorBlue	toRGBQUAD(0, 0, 255)

void swapBuffer(DWORD* frontBuffer, DWORD* backBuffer);

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

void drawFps(HDC hdc, LPCSTR str, int lineNum);

Mat4 model2WorldMatrix();
Mat4 model2WorldMatrix(Vec4 o, Vec4 x, Vec4 y, Vec4 z);
Mat4 world2ModelMatrix();
Mat4 world2ViewMatrix(Camera cam);
//Mat4 viewtoWorldMatrix();
Mat4 perspectiveProjectionMatrix(Camera cam);
//Mat4 orthoProjectionMatrix();
Mat4 perspective2ScreenMatrix();