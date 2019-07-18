#include "PreCompile.h"
#include "Camera.h"

namespace UsualEngine
{
	//Camera g_camera3D;		//3Dカメラ。

	Camera::Camera()
	{

	}

	void Camera::Update()
	{
		//ビュー行列を計算。
		m_viewMatrix.MakeLookAt(
			m_position,
			m_target,
			m_up
		);
		
		//プロジェクション行列を計算。
		if (m_cameraMode == ECameraMode::ePerth)
		{
			//ぱーすてぃぶ
			m_projMatrix.MakeProjectionMatrix(
				m_viewAngle,					//画角。
				FRAME_BUFFER_W / FRAME_BUFFER_H,	//アスペクト比。
				m_near,
				m_far
			);
		}
		else
		{
			//平行投影
			m_projMatrix.MakeOrthoProjectionMatrix(m_width, m_height, m_near, m_far);
		}


	}
}