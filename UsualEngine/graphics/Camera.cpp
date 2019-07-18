#include "PreCompile.h"
#include "Camera.h"

namespace UsualEngine
{
	//Camera g_camera3D;		//3D�J�����B

	Camera::Camera()
	{

	}

	void Camera::Update()
	{
		//�r���[�s����v�Z�B
		m_viewMatrix.MakeLookAt(
			m_position,
			m_target,
			m_up
		);
		
		//�v���W�F�N�V�����s����v�Z�B
		if (m_cameraMode == ECameraMode::ePerth)
		{
			//�ρ[���Ă���
			m_projMatrix.MakeProjectionMatrix(
				m_viewAngle,					//��p�B
				FRAME_BUFFER_W / FRAME_BUFFER_H,	//�A�X�y�N�g��B
				m_near,
				m_far
			);
		}
		else
		{
			//���s���e
			m_projMatrix.MakeOrthoProjectionMatrix(m_width, m_height, m_near, m_far);
		}


	}
}