#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
namespace UsualEngine
{
	class Camera
	{
	public:
		enum ECameraMode
		{
			ePerth,
			eOrth,
		};

		Camera();

		/*!
		 * @brief	�J�����s��A�v���W�F�N�V�����s��̍X�V�B
		 *@details
		 * ���ݐݒ肳��Ă���A�����_�A���_�A��p�Ȃǂ̏������Ƃ�
		 * �J�����s��ƃv���W�F�N�V�����s����X�V���܂��B
		 * ���̊֐����Ă΂Ȃ��ƁA�J�����͓����Ȃ��̂Œ��ӂ��K�v�ł��B
		 */
		void Update();
		/*!
		 * @brief	�J�����s����擾�B
		 */
		const CMatrix& GetViewMatrix() const
		{
			return m_viewMatrix;
		}
		/*!
		 * @brief	�v���W�F�N�V�����s����擾�B
		 */
		const CMatrix& GetProjectionMatrix() const
		{
			return m_projMatrix;
		}
		/*!
		 * @brief	���_���擾�B
		 */
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/*!
		 * @brief	�����_���擾�B
		 */
		const CVector3& GetTarget() const
		{
			return m_target;
		}
		/*!
		 * @brief	�J�����̏�������擾�B
		 */
		const CVector3& GetUp() const
		{
			return m_up;
		}
		/*!
		 * @brief	���_��ݒ�B
		 */
		void SetPosition(const CVector3& pos)
		{
			m_position = pos;
		}
		/*!
		 * @brief	�����_��ݒ�B
		 */
		void SetTarget(const CVector3& target)
		{
			m_target = target;
		}
		/*!
		 * @brief	�J�����̏������ݒ�B
		 */
		void SetUp(const CVector3& up)
		{
			m_up = up;
		}
		/*!
		 * @brief	�����ʂ�ݒ�B
		 */
		void SetFar(float f)
		{
			m_far = f;
		}
		/*!
		 * @brief	�ߕ��ʂ�ݒ�B
		 */
		void SetNear(float n)
		{
			m_near = n;
		}
		/*!
		 * @brief	��p��ݒ�B
		 */
		void SetViewAngle(float angle)
		{
			m_viewAngle = angle;
		}
		/// <summary>
		/// �r���[�A���O����Ԃ�
		/// </summary>
		/// <returns>�r���[�A���O��(float)</returns>
		float GetViewAngle() const
		{
			return m_viewAngle;
		}

		//�A�X���ݒ�
		void SetAspect(float as)
		{
			m_aspect = as;
		}
		//�A�X���Ԃ�
		float GetAspect() const
		{
			return m_aspect;
		}

		//�p�[�X�Ă��ԃ��[�h�ɂ���
		void CameraModePerth()
		{
			m_cameraMode = ECameraMode::ePerth;
		}
		//���s���e���[�h�ɂ���
		void CameraModeOrth()
		{
			m_cameraMode = ECameraMode::eOrth;
		}
		//���݂̃J�������[�h��Ԃ�
		ECameraMode GetCameraMode() const
		{
			return m_cameraMode;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const CVector3& GetForward() const
		{
			return m_forward;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const CVector3& GetRight() const
		{
			return m_right;
		}
		
		float GetFar() const
		{
			return m_far;
		}
		float GetNear() const
		{
			return m_near;
		}
	private:
		CMatrix	m_viewMatrix = CMatrix::Identity();		//�r���[�s��B
		CMatrix m_projMatrix = CMatrix::Identity();		//�v���W�F�N�V�����s��B
		CVector3 m_target = CVector3::Zero();			//�����_�B
		CVector3 m_position = CVector3::Zero();			//���_�B
		CVector3 m_up = CVector3::Up();					//������B
		CVector3 m_forward = CVector3::Front();		// �O�����B
		CVector3 m_right = CVector3::Right();			// �E�����B
		float m_viewAngle = CMath::DegToRad(60.0f);		//��p�B
		float m_aspect = 1.f;							//�A�X��
		float m_far = 1000.0f;							//�������ʂ܂ł̋����B
		float m_near = 1.0f;							//�ߕ��ʂ܂ł̋����B

		float m_width = 1280.0f;						//��
		float m_height = 720.0f;						//�c

		ECameraMode m_cameraMode = ECameraMode::ePerth;	//�J�������[�h
	};

	//extern Camera g_camera3D;		//!<3D�J�����B
}