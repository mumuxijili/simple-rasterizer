#pragma once

#include "mesh.h"
#include "camera.h"

// global vertex buffer
extern vector<Vertex> g_vVertexBuffer;
// global index buffer
extern vector<UINT> g_vIndexBuffer;
// global camera
extern Camera g_cam;


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

	int drawTriangle(Vertex v0, Vertex v1, Vertex v2);

	Mat4 model2WorldMatrix();
	Mat4 world2ModelMatrix();
	Mat4 world2ViewMatrix(Camera cam);
	//Mat4 ViewtoWorldMatrix();
	Mat4 perspectiveProjectionMatrix(Camera cam);
	//Mat4 OrthoProjectionMatrix();
	Mat4 perspective2ScreenMatrix();

	void renderPipeline();

private:
	// render mode(wireframe or solid)
	RenderMode m_renderMode;
	DWORD m_frameBuffer[g_winWidth * g_winHeight];
};