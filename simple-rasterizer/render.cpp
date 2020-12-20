#include "stdafx.h"
#include "render.h"

vector<Vertex> g_vVertexBuffer;
vector<UINT> g_vIndexBuffer;
Camera g_cam;

void Render::initRender()
{
	m_renderMode = RenderMode::RM_WIREFRAME;
}

DWORD* Render::draw()
{
	//
	// something like: RenderScene();
	//
	SYSTEMTIME time = { 0 };
	GetLocalTime(&time);
	int blue = (int)((double)time.wMilliseconds / 1000 * 255);
	for (int i = 0; i < g_winHeight; i++)
		for (int j = 0; j < g_winWidth; j++)
		{
			int r, g, b;
			r = (int)(((double)i / g_winHeight) * 255);
			g = (int)(((double)j / g_winWidth) * 255);
			//dwFrameBuffer[i * WinWidth + j] = RGBA(r, g, blue);
			//dwFrameBuffer[i * WinWidth + j] = ColorBlack;
			m_frameBuffer[i * g_winWidth + j] = RGB(r, g, blue);
		}
	drawLine(0, 0, 0, 719, ColorBlack);
	drawLine(0, 0, 1279, 0, ColorBlack);
	drawLine(0, 719, 1279, 719, ColorBlack);
	drawLine(1279, 0, 1279, 719, ColorBlack);

	// Vertex should has screen space coordinate
	Vertex v0, v1, v2;
	v0.m_SSCoord = Vec4(100, 100, 100, 1);
	v1.m_SSCoord = Vec4(500, 100, 100, 1);
	v2.m_SSCoord = Vec4(100, 200, 100, 1);
	v0.m_vertexNorm = Vec4(0.0, 0.0, 1.0, 0.0);
	v1.m_vertexNorm = Vec4(0.0, 0.0, 1.0, 0.0);
	v2.m_vertexNorm = Vec4(0.0, 0.0, 1.0, 0.0);
	v0.m_vertexColor = ColorBLUE;
	v1.m_vertexColor = ColorGREEN;
	v2.m_vertexColor = ColorRED;
	drawTrianglePlaneBottom(v2, v1, v0);
	v0.m_SSCoord += Vec4(700, 0, 0, 0);
	v1.m_SSCoord += Vec4(500, 100, 0, 0);
	v2.m_SSCoord += Vec4(500, 0, 0, 0);
	drawTrianglePlaneTop(v2, v1, v0);
	v0.m_SSCoord += Vec4(-600, 200, 0, 0);
	v1.m_SSCoord += Vec4(-500, 0, 0, 0);
	v2.m_SSCoord += Vec4(-200, 300, 0, 0);
	drawSolidTriangle(v0, v1, v2);
	for (int i = 0; i < 10; i++)
	{
		v0.m_SSCoord += Vec4(25, 0, 0, 0);
		v1.m_SSCoord += Vec4(25, 0, 0, 0);
		v2.m_SSCoord += Vec4(25, 0, 0, 0);
		drawSolidTriangle(v0, v1, v2);
	}
	//m_render->DrawTriangle(v0, v1, v2);
	return m_frameBuffer;
}

int Render::drawPixel(int x, int y, DWORD color)
{
	// use screen space coordinate
	if (x < 0 || y < 0 || x >= g_winWidth || y >= g_winHeight) {
		return -1;
	}
	m_frameBuffer[y * g_winWidth + x] = color;
	return 0;
}

int Render::drawLine(int x0, int y0, int x1, int y1, DWORD color)
{
	// Bresenham algorithm
	int x, y, dx, dy, dx2, dy2, xstep, ystep, error, index;
	x = x0;
	y = y0;
	dx = x1 - x0;
	dy = y1 - y0;

	if (dx >= 0)	// left to right
		xstep = 1;	// x step 1
	else {			// right to left
		xstep = -1; // x step -1
		dx = -dx;	// abs x
	}

	if (dy >= 0)	// top to bottom
		ystep = 1;	// y step 1
	else {			// bottom to top
		ystep = -1; // y step -1
		dy = -dy;	// abs y
	}

	dx2 = dx << 1;	// 2 * dx
	dy2 = dy << 1;	// 2 * dy

	if (dx > dy) {	// line near x
		error = dy2 - dx;
		for (index = 0; index <= dx; ++index) {
			drawPixel(x, y, color);
			if (error >= 0) {
				error -= dx2;
				y += ystep;
			}
			error += dy2;
			x += xstep;
		}
	}
	else {			// line near y
		error = dx2 - dy;
		for (index = 0; index <= dy; ++index) {
			drawPixel(x, y, color);
			if (error >= 0) {
				error -= dy2;
				x += xstep;
			}
			error += dx2;
			y += ystep;
		}
	}

	return 0;
}

void Render::drawScanLine(Vertex vLeft, Vertex vRight, int y)
{
	float leftX = vLeft.m_SSCoord.x;
	float rightX = vRight.m_SSCoord.x;
	int iLeftX = (int)(leftX + 0.5f);
	int iRightX = (int)(rightX + 0.5f);

	for (int x = iLeftX; x <= iRightX; x++)
	{
		if (x >= 0 && x < g_winWidth)
		{
			float dx = rightX - leftX;
			float t = 0.0f;
			if ((dx < -EPSINON) || (dx > EPSINON))
			{
				t = (x - leftX) / dx;
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////!!Optimize Me: lerp costs too much////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Vertex v;
			//v = lerpVertex(vLeft, vRight, t);
			DWORD color = lerpColor(vLeft.m_vertexColor, vRight.m_vertexColor, t);
			//DWORD color = ColorBlack;
			// depth test
			if (1)
			{
				drawPixel(x, y, color);
			}
		}
		if (x < 0 || x >= g_winWidth)
		{
			break;
		}
	}
}

// v0\------/v1
//    \    /
//     \  /
//    v2\/
void Render::drawTrianglePlaneTop(Vertex vTopLeft, Vertex vTopRight, Vertex vBottom)
{
	float topY = vTopLeft.m_SSCoord.y;
	float bottomY = vBottom.m_SSCoord.y;
	int iTopY = (int)(topY + 0.5);
	int iBottomY = (int)(bottomY + 0.5);

	// dx / dy, use as x step
	const double dDxDyLeft = (vTopLeft.m_SSCoord.x - vBottom.m_SSCoord.x) / (vTopLeft.m_SSCoord.y - vBottom.m_SSCoord.y);
	const double dDxDyRight = (vTopRight.m_SSCoord.x - vBottom.m_SSCoord.x) / (vTopRight.m_SSCoord.y - vBottom.m_SSCoord.y);

	for (int y = iBottomY; y <= iTopY; y++)
	{

		if (y >= 0 && y < g_winHeight)
		{
			// find left and right vertex
			Vertex vLeft, vRight;
			float t = (y - bottomY) / (topY - bottomY);
			vLeft = lerpVertex(vBottom, vTopLeft, t);
			vRight = lerpVertex(vBottom, vTopRight, t);

			//int xLeft = (int)(dDxDyLeft * (y - bottomY) + vBottom.m_SSCoord.x + 0.5);
			//int xRight = (int)(dDxDyRight * (y - bottomY) + vBottom.m_SSCoord.x + 0.5);
			//vLeft.m_SSCoord = Vec4(xLeft, y, 0, 0);
			//vRight.m_SSCoord = Vec4(xRight, y, 0, 0);

			drawScanLine(vLeft, vRight, y);
		}
		if (y < 0 || y >= g_winHeight)
		{
			break;
		}
	}
}

//      /\v0
//     /  \
//    /    \
// v2/______\v1
void Render::drawTrianglePlaneBottom(Vertex vTop, Vertex vBottomRight, Vertex vBottomLeft)
{
	float topY = vTop.m_SSCoord.y;
	float bottomY = vBottomLeft.m_SSCoord.y;
	int iTopY = (int)(topY + 0.5f);
	int iBottomY = (int)(bottomY + 0.5f);

	// dx / dy, use as x step
	const double dDxDyLeft = (vTop.m_SSCoord.x - vBottomLeft.m_SSCoord.x) / (vTop.m_SSCoord.y - vBottomLeft.m_SSCoord.y);
	const double dDxDyRight = (vTop.m_SSCoord.x - vBottomRight.m_SSCoord.x) / (vTop.m_SSCoord.y - vBottomRight.m_SSCoord.y);

	for (int y = iBottomY; y <= iTopY; y++)
	{

		if (y >= 0 && y < g_winHeight)
		{
			// find left and right vertex
			Vertex vLeft, vRight;
			float t = (y - bottomY) / (topY - bottomY);
			vLeft = lerpVertex(vBottomLeft, vTop, t);
			vRight = lerpVertex(vBottomRight, vTop, t);

			//int xLeft = (int)(dDxDyLeft * (y - bottomY) + vBottomLeft.m_SSCoord.x + 0.5);
			//int xRight = (int)(dDxDyRight * (y - bottomY) + vBottomRight.m_SSCoord.x + 0.5);
			//vLeft.m_SSCoord = Vec4(xLeft, y, 0, 0);
			//vRight.m_SSCoord = Vec4(xRight, y, 0, 0);

			drawScanLine(vLeft, vRight, y);
		}
		if (y < 0 || y >= g_winHeight)
		{
			break;
		}
	}
}

// normal triangle
void Render::drawSolidTriangle(Vertex v0, Vertex v1, Vertex v2)
{
	// if is planetop or planebottom
	if (v0.m_SSCoord.y == v1.m_SSCoord.y)
	{
		// planetop
		if (v0.m_SSCoord.y > v2.m_SSCoord.y)
		{
			if (v0.m_SSCoord.x < v1.m_SSCoord.x)
			{
				drawTrianglePlaneTop(v0, v1, v2);
			}
			else if (v0.m_SSCoord.x > v1.m_SSCoord.x)
			{
				drawTrianglePlaneTop(v1, v0, v2);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		// planebottom
		else if (v0.m_SSCoord.y < v2.m_SSCoord.y)
		{
			if (v0.m_SSCoord.x < v1.m_SSCoord.x)
			{
				drawTrianglePlaneBottom(v2, v1, v0);
			}
			else if (v0.m_SSCoord.x > v1.m_SSCoord.x)
			{
				drawTrianglePlaneBottom(v2, v0, v1);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		else
		{
			// v0 v1 v2 is a line
		}
	}
	else if (v0.m_SSCoord.y == v2.m_SSCoord.y)
	{
		// planetop
		if (v0.m_SSCoord.y > v1.m_SSCoord.y)
		{
			if (v0.m_SSCoord.x < v2.m_SSCoord.x)
			{
				drawTrianglePlaneTop(v0, v2, v1);
			}
			else if (v0.m_SSCoord.x > v2.m_SSCoord.x)
			{
				drawTrianglePlaneTop(v2, v0, v1);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		// planebottom
		else if (v0.m_SSCoord.y < v1.m_SSCoord.y)
		{
			if (v0.m_SSCoord.x < v2.m_SSCoord.x)
			{
				drawTrianglePlaneBottom(v1, v2, v0);
			}
			else if (v0.m_SSCoord.x > v2.m_SSCoord.x)
			{
				drawTrianglePlaneBottom(v1, v0, v2);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		else
		{
			// v0 v1 v2 is a line
		}
	}
	else if (v1.m_SSCoord.y == v2.m_SSCoord.y)
	{
		// planetop
		if (v1.m_SSCoord.y > v0.m_SSCoord.y)
		{
			if (v1.m_SSCoord.x < v2.m_SSCoord.x)
			{
				drawTrianglePlaneTop(v1, v2, v0);
			}
			else if (v1.m_SSCoord.x > v2.m_SSCoord.x)
			{
				drawTrianglePlaneTop(v2, v1, v0);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		// planebottom
		else if (v1.m_SSCoord.y < v0.m_SSCoord.y)
		{
			if (v1.m_SSCoord.x < v2.m_SSCoord.x)
			{
				drawTrianglePlaneBottom(v0, v2, v1);
			}
			else if (v1.m_SSCoord.x > v2.m_SSCoord.x)
			{
				drawTrianglePlaneBottom(v0, v1, v2);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		else
		{
			// v0 v1 v2 is a line
		}
	}
	// normal triangle
	else
	{
		Vertex vTop, vMiddle, vBottom;
		if (v0.m_SSCoord.y < v1.m_SSCoord.y && v0.m_SSCoord.y < v2.m_SSCoord.y)
		{
			vBottom = v0;
			if (v1.m_SSCoord.y < v2.m_SSCoord.y)
			{
				vMiddle = v1;
				vTop = v2;
			}
			else
			{
				vMiddle = v2;
				vTop = v1;
			}
		}
		else if (v1.m_SSCoord.y < v0.m_SSCoord.y && v1.m_SSCoord.y < v2.m_SSCoord.y)
		{
			vBottom = v1;
			if (v0.m_SSCoord.y < v2.m_SSCoord.y)
			{
				vMiddle = v0;
				vTop = v2;
			}
			else
			{
				vMiddle = v2;
				vTop = v0;
			}
		}
		else if (v2.m_SSCoord.y < v0.m_SSCoord.y && v2.m_SSCoord.y < v1.m_SSCoord.y)
		{
			vBottom = v2;
			if (v0.m_SSCoord.y < v1.m_SSCoord.y)
			{
				vMiddle = v0;
				vTop = v1;
			}
			else
			{
				vMiddle = v1;
				vTop = v0;
			}
		}
		else
		{
			// should not reach this branch
		}

		// find the new middle vertex
		float t = (vMiddle.m_SSCoord.y - vBottom.m_SSCoord.y) / (vTop.m_SSCoord.y - vBottom.m_SSCoord.y);
		Vertex newMiddle = lerpVertex(vBottom, vTop, t);
		if (vMiddle.m_SSCoord.x < newMiddle.m_SSCoord.x)
		{
			drawTrianglePlaneBottom(vTop, newMiddle, vMiddle);
			drawTrianglePlaneTop(vMiddle, newMiddle, vBottom);
		}
		else if (vMiddle.m_SSCoord.x > newMiddle.m_SSCoord.x)
		{
			drawTrianglePlaneBottom(vTop, vMiddle, newMiddle);
			drawTrianglePlaneTop(newMiddle, vMiddle, vBottom);
		}
		else
		{
			// is a line
		}
	}
}

void Render::drawFrameTriangle(Vertex v0, Vertex v1, Vertex v2)
{
	drawLine((int)v0.m_SSCoord.x, (int)v0.m_SSCoord.y, (int)v1.m_SSCoord.x, (int)v1.m_SSCoord.y, ColorBlack);
	drawLine((int)v1.m_SSCoord.x, (int)v1.m_SSCoord.y, (int)v2.m_SSCoord.x, (int)v2.m_SSCoord.y, ColorBlack);
	drawLine((int)v2.m_SSCoord.x, (int)v2.m_SSCoord.y, (int)v0.m_SSCoord.x, (int)v0.m_SSCoord.y, ColorBlack);
}

int Render::drawTriangle(Vertex v0, Vertex v1, Vertex v2)
{
	// Vertex should has screen space coordinate
	// now assume (int)Vertex.m_SSCoord.xy is the screen space coordinate
	if (m_renderMode == RenderMode::RM_SOLID)
	{

		drawSolidTriangle(v0, v1, v2);
		return 0;
	}
	else if (m_renderMode == RenderMode::RM_WIREFRAME)
	{

		drawFrameTriangle(v0, v1, v2);
		return 0;
	}
	else
	{
		return -1;
	}
	return 0;
}

// world matrix is identity matrix, which means model doesn't move
Mat4 Render::model2WorldMatrix()
{
	Mat4 worldmatrix = Mat4(); // identity matrix
	return worldmatrix;
}

Mat4 Render::world2ModelMatrix()
{
	Mat4 inverseworldmatrix = Mat4();
	return inverseworldmatrix;
}

Mat4 Render::world2ViewMatrix(Camera cam)
{
	Mat4 viewmatrix = Mat4();
	Vec4 v = Vec4(0, 1, 0, 0);
	Vec4 n = cam.m_Dir.normalize();
	Vec4 u = v.cross(n);
	u.w = 0;
	u.normalize();
	v = n.cross(u);
	v.w = 0;
	v.normalize();
	viewmatrix.setCol(0, u);
	viewmatrix.setCol(1, v);
	viewmatrix.setCol(2, n);
	float camx = -(cam.m_Pos.dot(u));
	float camy = -(cam.m_Pos.dot(v));
	float camz = -(cam.m_Pos.dot(n));
	viewmatrix.setRow(3, Vec4(camx, camy, camz, 1));
	return viewmatrix;
}

Mat4 Render::perspectiveProjectionMatrix(Camera cam)
{
	return Mat4();
}

void Render::renderPipeline()
{
	// 1. Model to World
	// 2. World to View
	// 3. NDC
	// 4. NDC Clipping
	// 5. Projection
	// 6. Screen Space
}