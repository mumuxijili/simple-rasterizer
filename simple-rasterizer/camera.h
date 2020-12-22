#pragma once

class Camera
{
public:
	Camera() {};
	~Camera() {};

	Camera(Vec4 pos, Vec4 up, Vec4 dir, float fov, float aspect, float fnear, float ffar)
	{
		m_pos = pos;
		m_up = up;
		m_dir = dir;
		m_fov = fov;
		m_aspect = aspect;
		m_near = fnear;
		m_far = ffar;

		m_n = m_dir;
		m_u = m_up.cross(m_n).normalize();
		m_v = m_n.cross(m_u).normalize();
	}

	void setPosition(Vec4 pos) { m_pos = pos; }
	Vec4 position() { return m_pos; }

	void lookAt(Vec4 target)
	{
		m_target = target;
		m_dir = m_pos - m_target;
		m_dir.normalize();
	}
	Vec4 direction() { return m_dir; }

	void setPerspective(float fov, float aspect, float fnear, float ffar)
	{
		m_fov = fov;
		m_aspect = aspect;
		m_near = fnear;
		m_far = ffar;
	}

	void updateMatrix()
	{
		Vec4 u = m_u;
		u.w = -m_pos.dot(m_u);
		Vec4 v = m_v;
		v.w = -m_pos.dot(m_v);
		Vec4 n = m_n;
		n.w = -m_pos.dot(m_n);
		Vec4 w = Vec4(0.f, 0.f, 0.f, 1.f);
		// https://www.cnblogs.com/wbaoqing/p/5422974.html
		m_world2View = Mat4();
		m_world2View.setRow(0, u);
		m_world2View.setRow(1, v);
		m_world2View.setRow(2, n);
		m_world2View.setRow(3, w);

		// https://zhuanlan.zhihu.com/p/152280876
		// https://www.cnblogs.com/bluebean/p/5276111.html
		float f = 1.f / (float)tan(m_fov * 0.5f);

		m_view2Projection = Mat4(
			f / m_aspect, 0.f, 0.f, 0.f,
			0.f, f, 0.f, 0.f,
			0.f, 0.f, -(m_far + m_near) / (m_far - m_near), -2.f * m_near * m_far / (m_far - m_near),
			0.f, 0.f, -1.f, 0.f);

		m_world2Projection = m_view2Projection * m_world2View;
	}

	Vec4 m_pos; // camera position(world space)


	//      ^
	//      |v up
	//      |
	// n    |
	//  <---/- - - - target
	//     /
	//    /u
	Vec4 m_dir; // look at direction
	Vec4 m_up;
	Vec4 m_target; // target position(world space)

	Vec4 m_u;
	Vec4 m_v;
	Vec4 m_n;

	float view_dist_h;
	float view_dist_v; // 水平视距和垂直视距，在透视变换中使用

	float m_fov;
	float m_aspect; // width/height
	float m_near;
	float m_far;

	float m_viewportWidth; // screen/viewport size
	float m_viewportHeight;

	Mat4 m_world2View = Mat4();
	Mat4 m_view2Projection = Mat4();
	Mat4 m_world2Projection = Mat4();
};