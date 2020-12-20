#include "stdafx.h"
#include "util.h"

DWORD RGBA(int r, int g, int b, int a)
{
	return ((b)+((g) << 8) + ((r) << 16) + ((a) << 24));
}

DWORD RGBA(Vec4 color)
{
	return (((int)color.z * 255) + (((int)color.y * 255) << 8) + (((int)color.x * 255) << 16) + ((0) << 24));
}

void toRGB(DWORD rbg, int &r, int &g, int &b)
{
	r = rbg >> 16 & 0xff;
	g = rbg >> 8 & 0xff;
	b = rbg & 0xff;
}

float lerpFloat(float fStart, float fEnd, float t)
{
	return fStart + (fEnd - fStart) * t;
}

DWORD lerpColor(DWORD color1, DWORD color2, float t)
{
	int r1, g1, b1, r2, g2, b2;
	toRGB(color1, r1, g1, b1);
	toRGB(color2, r2, g2, b2);

	int r = (int)lerpFloat((float)r1, (float)r2, t);
	int g = (int)lerpFloat((float)g1, (float)g2, t);
	int b = (int)lerpFloat((float)b1, (float)b2, t);
	return RGB(r, g, b);
}

Vec4 lerpVector(Vec4 vStart, Vec4 vEnd, float t)
{
	float x1, y1, z1, w1, x2, y2, z2, w2;
	x1 = vStart.x;
	y1 = vStart.y;
	z1 = vStart.z;
	w1 = vStart.w;
	x2 = vEnd.x;
	y2 = vEnd.y;
	z2 = vEnd.z;
	w2 = vEnd.w;
	float x = lerpFloat(x1, x2, t);
	float y = lerpFloat(y1, y2, t);
	float z = lerpFloat(z1, z2, t);
	float w = lerpFloat(w1, w2, t);
	return Vec4(x, y, z, w);
}

void drawLabel(HDC hdc, LPCSTR str, int lineNum)
{
	//--draw text
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	//char fpsstr[32];
	//sprintf_s(fpsstr, "FPS: %d", fps);
	TextOutA(hdc, 0, 17 * lineNum, str, (int)strlen(str));
}