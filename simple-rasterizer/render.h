#pragma once

#include "mesh.h"
#include "camera.h"

// global vertex buffer
extern vector<Vertex> g_vVertexBuffer;
// global index buffer
extern vector<UINT> g_vIndexBuffer;
// global camera
extern Camera g_camera;


class Render
{
public:
	Render() {};
	~Render() {};

	void initRender();

	DWORD* draw();

	void setRenderMode(RenderMode rd) { m_renderMode = rd; }
	RenderMode renderMode() { return m_renderMode; }
	int drawPixel(int x, int y, DWORD color);
	// Line Rasterize, use screen space coordinate
	int drawLine(int x0, int y0, int x1, int y1, DWORD color);
	void drawScanLine(Vertex vLeft, Vertex vRight, int y);
	void drawTrianglePlaneTop(Vertex vTopLeft, Vertex vTopRight, Vertex vBottom);
	void drawTrianglePlaneBottom(Vertex vTop, Vertex vBottomRight, Vertex vBottomLeft);
	void drawSolidTriangle(Vertex v0, Vertex v1, Vertex v2);
	void drawFrameTriangle(Vertex v0, Vertex v1, Vertex v2);

	void drawTriangle(Vertex v0, Vertex v1, Vertex v2);
	void drawTriangle(Triangle triangle);

	void drawRect(Vertex lt, Vertex rt, Vertex rb, Vertex lb);

	void renderPipeline();

private:
	RenderMode m_renderMode;
	DWORD m_frameBuffer[g_winWidth * g_winHeight];
	float m_zBuffer[g_winWidth * g_winHeight];
};