#include "stdafx.h"
#include "render.h"

vector<Vertex> g_vVertexBuffer;
vector<UINT> g_vIndexBuffer;
Camera g_camera;

void Render::initRender()
{
	for (int i = 0; i < g_winWidth * g_winHeight; i++)
		m_zBuffer[i] = -FLT_MAX;
	m_renderMode = RenderMode::RenderMode_SOLID;
	Vec4 cameraPos = Vec4(5.f, 5.f, 5.f, 1.f);
	Vec4 up = Vec4(0, 1, 0, 0);
	//Vec4 target = Vec4(g_winWidth / 2, g_winHeight / 2, 0);
	Vec4 target = Vec4(0, 0, 0, 1.f);
	g_camera = Camera(cameraPos, up, (cameraPos - target).normalize(), 0, 0, 0);
	g_camera.setPerspective((float)M_PI * 0.25f, (float)g_winWidth / g_winHeight, 1.f, -10.f);
	g_camera.updateMatrix();
	for (int i = 0; i < 8; i++)
	{
		g_cube[i].m_SSCoord = g_camera.m_world2Projection.mulVec(g_cube[i].m_vertexPos);
		perspectiveDivede(g_cube[i].m_SSCoord);
		transformScreen(g_cube[i].m_SSCoord);
	}
}

DWORD* Render::draw()
{
	//////////////////////////////////////////////////// set background ////////////////////////////////////////////////////
	memset(m_frameBuffer, ColorWhite, g_winWidth * g_winHeight * sizeof(DWORD));
	for (int i = 0; i < g_winWidth * g_winHeight; i++)
		m_zBuffer[i] = -FLT_MAX;
	//SYSTEMTIME time = { 0 };
	//GetLocalTime(&time);
	//int blue = (int)((double)time.wMilliseconds / 1000 * 255);
	//for (int i = 0; i < g_winHeight; i++)
	//{
	//	for (int j = 0; j < g_winWidth; j++)
	//	{
	//		int r, g;
	//		r = (int)(((double)i / g_winHeight) * 255);
	//		g = (int)(((double)j / g_winWidth) * 255);
	//		m_frameBuffer[i * g_winWidth + j] = toRGBQUAD(r, g, blue);
	//	}
	//}

	//////////////////////////////////////////////////// draw lines ////////////////////////////////////////////////////
	//drawLine(0, 0, 719, 719, ColorBlack);

	// x-axis
	drawLine(0, 10, 1279, 10, ColorRed);
	drawLine(1269, 0, 1279, 10, ColorRed);
	drawLine(1269, 20, 1279, 10, ColorRed);
	// y-axis
	drawLine(10, 0, 10, 719, ColorBlue);
	drawLine(0, 709, 10, 719, ColorBlue);
	drawLine(20, 709, 10, 719, ColorBlue);

	////////////////////////////////////////////////////// draw triangles ////////////////////////////////////////////////////
	//// Vertex should has screen space coordinate
	//Vertex v0, v1, v2;
	//v0.m_SSCoord = Vec4(100, 100, 100, 1);
	//v1.m_SSCoord = Vec4(500, 100, 100, 1);
	//v2.m_SSCoord = Vec4(100, 200, 100, 1);
	////v0.m_vertexNorm = Vec4(0.0, 0.0, 1.0, 0.0);
	////v1.m_vertexNorm = Vec4(0.0, 0.0, 1.0, 0.0);
	////v2.m_vertexNorm = Vec4(0.0, 0.0, 1.0, 0.0);
	//v0.m_vertexColor = ColorBlue;
	//v1.m_vertexColor = ColorGreen;
	//v2.m_vertexColor = ColorRed;
	//drawTrianglePlaneBottom(v2, v1, v0);
	//v0.m_SSCoord += Vec4(700, 0, 0, 0);
	//v1.m_SSCoord += Vec4(500, 100, 0, 0);
	//v2.m_SSCoord += Vec4(500, 0, 0, 0);
	//drawTrianglePlaneTop(v2, v1, v0);
	//v0.m_SSCoord += Vec4(-600, 200, 0, 0);
	//v1.m_SSCoord += Vec4(-500, 0, 0, 0);
	//v2.m_SSCoord += Vec4(-200, 300, 0, 0);
	//drawFrameTriangle(v0, v1, v2);
	//v0.m_SSCoord += Vec4(300, 0, 0, 0);
	//v1.m_SSCoord += Vec4(300, 0, 0, 0);
	//v2.m_SSCoord += Vec4(300, 0, 0, 0);
	//drawSolidTriangle(v0, v1, v2);
	////for (int i = 0; i < 10; i++)
	////{
	////	v0.m_SSCoord += Vec4(25, 0, 0, 0);
	////	v1.m_SSCoord += Vec4(25, 0, 0, 0);
	////	v2.m_SSCoord += Vec4(25, 0, 0, 0);
	////	drawSolidTriangle(v0, v1, v2);
	////}

	//////////////////////////////////////////////////// draw cube ////////////////////////////////////////////////////
	drawRect(g_cube[flt], g_cube[frt], g_cube[frb], g_cube[flb]); // front
	drawRect(g_cube[blt], g_cube[flt], g_cube[flb], g_cube[blb]); // left
	drawRect(g_cube[frt], g_cube[brt], g_cube[brb], g_cube[frb]); // right
	drawRect(g_cube[blt], g_cube[brt], g_cube[frt], g_cube[flt]); // top
	drawRect(g_cube[blb], g_cube[brb], g_cube[frb], g_cube[flb]); // bottom
	drawRect(g_cube[blt], g_cube[brt], g_cube[brb], g_cube[blb]); // back

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
			// depth test
			float z = lerpFloat(vLeft.m_SSCoord.z, vRight.m_SSCoord.z, t);
			if (z > m_zBuffer[y * g_winWidth + x])
			{
				//Vertex v;
				//v = lerpVertex(vLeft, vRight, t);
				DWORD color = lerpColor(vLeft.m_vertexColor, vRight.m_vertexColor, t);
				//DWORD color = ColorBlack;
				drawPixel(x, y, color);
				m_zBuffer[y * g_winWidth + x] = z;
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

void Render::drawTriangle(Vertex v0, Vertex v1, Vertex v2)
{
	// Vertex should has screen space coordinate
	// now assume (int)Vertex.m_SSCoord.xy is the screen space coordinate
	if (m_renderMode == RenderMode_SOLID)
	{
		drawSolidTriangle(v0, v1, v2);
	}
	else if (m_renderMode == RenderMode_WIREFRAME)
	{
		drawFrameTriangle(v0, v1, v2);
	}
}

void Render::drawTriangle(Triangle triangle)
{
	drawTriangle(triangle.v0, triangle.v1, triangle.v2);
}

void Render::drawRect(Vertex lt, Vertex rt, Vertex rb, Vertex lb)
{
	drawTriangle(lt, rt, rb);
	drawTriangle(lt, rb, lb);
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