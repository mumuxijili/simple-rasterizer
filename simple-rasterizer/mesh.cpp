#include "stdafx.h"
#include "mesh.h"

void Mesh::resizeVertices(int vertexCount)
{
	m_vMeshPoses.resize(vertexCount);
	m_vMeshNorms.resize(vertexCount);
	m_vMeshColors.resize(vertexCount);
}

Vertex Mesh::getVertex(int vertIndex)
{
	assert(vertIndex < (int)m_vMeshPoses.size());

	Vertex v;
	v.m_pos = m_vMeshPoses[vertIndex];
	v.m_normal = m_vMeshNorms[vertIndex];
	v.m_color = m_vMeshColors[vertIndex];
	return v;
}

void Mesh::setVertex(int vertIndex, const Vertex &v)
{
	assert(vertIndex < (int)m_vMeshPoses.size());

	m_vMeshPoses[vertIndex] = v.m_pos;
	m_vMeshNorms[vertIndex] = v.m_normal;
	m_vMeshColors[vertIndex] = v.m_color;
}

void Mesh::pushBackVertex(const Vertex &v)
{
	m_vMeshPoses.push_back(v.m_pos);
	m_vMeshNorms.push_back(v.m_normal);
	m_vMeshColors.push_back(v.m_color);
}
