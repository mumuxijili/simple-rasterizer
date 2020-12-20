#pragma once

class Vertex
{
public:
	Vertex() {};
	Vertex(Vec4 pos, DWORD color) : m_vertexPos(pos), m_vertexColor(color) { m_SSCoord = m_vertexPos; } // WARNING model and world in the same coordinate system.
	~Vertex() {};

	Vec4 m_vertexPos;
	Vec4 m_vertexNorm;
	DWORD m_vertexColor;

	Vec4 m_SSCoord; // Screen Space Coordinate, with depth
};

struct Triangle
{
	Vertex v0, v1, v2;
};

class Mesh
{
public:
	Mesh() {};
	virtual ~Mesh() {};

	vector<Vec4> m_vMeshPoses;
	vector<Vec4> m_vMeshNorms;
	vector<DWORD> m_vMeshColors;
	vector<UINT> m_vMeshIndices;

	void resizeVertices(int vertexCount);
	void resizeIndices(int IDtriCount)
	{
		m_vMeshIndices.resize(IDtriCount);
	}
	void clearAll()
	{
		resizeVertices(0);
		resizeIndices(0);
	}
	Vertex getVertex(int vertIndex);
	void setVertex(int vertIndex, const Vertex &v);
	void pushBackVertex(const Vertex &v);
};