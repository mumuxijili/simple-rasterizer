#pragma once

class Camera
{
public:
	Camera() {};
	~Camera() {};
	Camera(Vec4 pos, Vec4 dir, float fov, float fnear, float ffar)
	{
		m_Pos = pos;
		m_Dir = dir;
		m_Fov = fov;
		m_Near = fnear;
		m_Far = ffar;
	}
	Vec4 m_Pos; // camera position(world space)


	//      |v
	//      |
	//      |
	//  cam /------->target
	//     /         n
	//    /u
	Vec4 m_Dir; // look at direction
	Vec4 m_u;
	Vec4 m_v;
	Vec4 m_n;
	Vec4 m_Target; // target position(world space)

	float view_dist_h;
	float view_dist_v; // ˮƽ�Ӿ�ʹ�ֱ�Ӿ࣬��͸�ӱ任��ʹ��

	float m_Fov;
	float m_Near;
	float m_Far;
	float m_Aspect; // width/height

	float m_ViewportWidth; // screen/viewport size
	float m_ViewportHeight;

	Mat4 m_worldToView = Mat4();
};