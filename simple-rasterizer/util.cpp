#include "stdafx.h"
#include "util.h"

//Vertex g_cube[8] = { Vertex(Vec4(100, 100, 100, 1), ColorRed), Vertex(Vec4(200, 100, 100, 1), ColorGreen), Vertex(Vec4(100, 200, 100, 1), ColorBlue), Vertex(Vec4(200, 200, 100, 1), ColorBlack),
//					Vertex(Vec4(100, 100, 0, 1), ColorRed), Vertex(Vec4(200, 100, 0, 1), ColorGreen), Vertex(Vec4(100, 200, 0, 1), ColorBlue), Vertex(Vec4(200, 200, 0, 1), ColorBlack) };
Vertex g_cube[8] = { Vertex(Vec4(-1, 1, 1, 1), ColorRed), Vertex(Vec4(1, 1, 1, 1), ColorGreen), Vertex(Vec4(1, -1, 1, 1), ColorBlue), Vertex(Vec4(-1, -1, 1, 1), ColorBlack),
					Vertex(Vec4(-1, 1, -1, 1), ColorRed), Vertex(Vec4(1, 1, -1, 1), ColorGreen), Vertex(Vec4(1, -1, -1, 1), ColorBlue), Vertex(Vec4(-1, -1, -1, 1), ColorBlack) };

DWORD RGBA(int r, int g, int b, int a)
{
	return ((b)+((g) << 8) + ((r) << 16) + ((a) << 24));
}

DWORD RGBA(Vec4 color)
{
	return (((int)color.z * 255) + (((int)color.y * 255) << 8) + (((int)color.x * 255) << 16) + ((0) << 24));
}

void fromRGB(DWORD rbg, int &r, int &g, int &b)
{
	r = GetRValue(rbg);
	g = GetGValue(rbg);
	b = GetBValue(rbg);
}

void fromRGBQUAD(DWORD rbg, int &r, int &g, int &b)
{
	b = GetRValue(rbg);
	g = GetGValue(rbg);
	r = GetBValue(rbg);
}

float lerpFloat(float fStart, float fEnd, float t)
{
	return fStart + (fEnd - fStart) * t;
}

DWORD lerpColor(DWORD color1, DWORD color2, float t)
{
	if (color1 == color2)
		return color1;

	int r1, g1, b1, r2, g2, b2;
	fromRGBQUAD(color1, r1, g1, b1);
	fromRGBQUAD(color2, r2, g2, b2);

	int r = (int)lerpFloat((float)r1, (float)r2, t);
	int g = (int)lerpFloat((float)g1, (float)g2, t);
	int b = (int)lerpFloat((float)b1, (float)b2, t);
	return toRGBQUAD(r, g, b);
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

Vertex lerpVertex(Vertex vStart, Vertex vEnd, float t)
{
	Vertex v;
	v.m_vertexPos = lerpVector(vStart.m_vertexPos, vEnd.m_vertexPos, t);
	v.m_SSCoord = lerpVector(vStart.m_SSCoord, vEnd.m_SSCoord, t);
	v.m_vertexNorm = lerpVector(vStart.m_vertexNorm, vEnd.m_vertexNorm, t);
	v.m_vertexColor = lerpColor(vStart.m_vertexColor, vEnd.m_vertexColor, t);

	return v;
}

void swapBuffer(DWORD* frontBuffer, DWORD* backBuffer)
{
	size_t bufferSize = g_winWidth * g_winHeight * sizeof(DWORD);
	memcpy_s(frontBuffer, bufferSize, backBuffer, bufferSize);
}

void drawFps(HDC hdc, LPCSTR str, int lineNum)
{
	//--draw text
	SetTextColor(hdc, RGB(255, 0, 0));
	SetBkMode(hdc, OPAQUE);
	TextOutA(hdc, 5, 17 * lineNum, str, (int)strlen(str));
}

// world matrix is identity matrix, which means model doesn't move
Mat4 model2WorldMatrix()
{
	Mat4 worldmatrix = Mat4(); // identity matrix
	return worldmatrix;
}

// https://zhuanlan.zhihu.com/p/35943426
Mat4 model2WorldMatrix(Vec4 o, Vec4 x, Vec4 y, Vec4 z)
{
	Mat4 worldmatrix = Mat4(); // identity matrix

	x.w = y.w = z.w = 0;
	worldmatrix.setCol(0, x);
	worldmatrix.setCol(1, y);
	worldmatrix.setCol(2, z);
	o.w = 1;
	worldmatrix.setCol(3, o);

	return worldmatrix;
}

Mat4 world2ModelMatrix()
{
	Mat4 inverseworldmatrix = Mat4();
	return inverseworldmatrix;
}

Mat4 world2ViewMatrix(Camera cam)
{
	Mat4 viewmatrix = Mat4();
	Vec4 v = Vec4(0, 1, 0, 0);
	Vec4 n = cam.direction();
	Vec4 u = v.cross(n);
	u.w = 0;
	u.normalize();
	v = n.cross(u);
	v.w = 0;
	v.normalize();
	viewmatrix.setColValue(0, u);
	viewmatrix.setColValue(1, v);
	viewmatrix.setColValue(2, n);
	float camx = -(cam.m_pos.dot(u));
	float camy = -(cam.m_pos.dot(v));
	float camz = -(cam.m_pos.dot(n));
	viewmatrix.setRow(3, Vec4(camx, camy, camz, 1));
	return viewmatrix;
}

Mat4 perspectiveProjectionMatrix(Camera cam)
{
	return Mat4();
}

// 透视除法 齐次除法
void perspectiveDivede(Vec4& v)
{
	v.x /= v.w;
	v.y /= v.w;
	v.z /= v.w;
	v.w = 1.0f;
}

// 转换到屏幕坐标
void transformScreen(Vec4& v)
{
	v.x = (v.x + 1.0f) * g_winWidth * 0.5f;
	v.y = (v.y + 1.0f) * g_winHeight * 0.5f;
}
