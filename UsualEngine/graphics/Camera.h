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
		 * @brief	カメラ行列、プロジェクション行列の更新。
		 *@details
		 * 現在設定されている、注視点、視点、画角などの情報をもとに
		 * カメラ行列とプロジェクション行列を更新します。
		 * この関数を呼ばないと、カメラは動かないので注意が必要です。
		 */
		void Update();
		/*!
		 * @brief	カメラ行列を取得。
		 */
		CMatrix GetViewMatrix()
		{
			return m_viewMatrix;
		}
		/*!
		 * @brief	プロジェクション行列を取得。
		 */
		CMatrix GetProjectionMatrix()
		{
			return m_projMatrix;
		}
		/*!
		 * @brief	視点を取得。
		 */
		CVector3 GetPosition()
		{
			return m_position;
		}
		/*!
		 * @brief	注視点を取得。
		 */
		CVector3 GetTarget()
		{
			return m_target;
		}
		/*!
		 * @brief	カメラの上方向を取得。
		 */
		CVector3 GetUp()
		{
			return m_up;
		}
		/*!
		 * @brief	視点を設定。
		 */
		void SetPosition(CVector3 pos)
		{
			m_position = pos;
		}
		/*!
		 * @brief	注視点を設定。
		 */
		void SetTarget(CVector3 target)
		{
			m_target = target;
		}
		/*!
		 * @brief	カメラの上方向を設定。
		 */
		void SetUp(CVector3 up)
		{
			m_up = up;
		}
		/*!
		 * @brief	遠平面を設定。
		 */
		void SetFar(float f)
		{
			m_far = f;
		}
		/*!
		 * @brief	近平面を設定。
		 */
		void SetNear(float n)
		{
			m_near = n;
		}
		/*!
		 * @brief	画角を設定。
		 */
		void SetViewAngle(float angle)
		{
			m_viewAngle = angle;
		}
		/// <summary>
		/// ビューアングルを返す
		/// </summary>
		/// <returns>ビューアングル(float)</returns>
		float GetViewAngle()
		{
			return m_viewAngle;
		}

		//アス比を設定
		void SetAspect(float as)
		{
			m_aspect = as;
		}
		//アス比を返す
		float GetAspect()
		{
			return m_aspect;
		}

		//パースてぃぶモードにする
		void CameraModePerth()
		{
			m_cameraMode = ECameraMode::ePerth;
		}
		//平行投影モードにする
		void CameraModeOrth()
		{
			m_cameraMode = ECameraMode::eOrth;
		}
		//現在のカメラモードを返す
		ECameraMode GetCameraMode()
		{
			return m_cameraMode;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		CVector3 GetForward()
		{
			return m_forward;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		CVector3 GetRight()
		{
			return m_right;
		}
		
	private:
		CMatrix	m_viewMatrix = CMatrix::Identity();		//ビュー行列。
		CMatrix m_projMatrix = CMatrix::Identity();		//プロジェクション行列。
		CVector3 m_target = CVector3::Zero();			//注視点。
		CVector3 m_position = CVector3::Zero();			//視点。
		CVector3 m_up = CVector3::Up();					//上方向。
		CVector3 m_forward = CVector3::Front();		// 前方向。
		CVector3 m_right = CVector3::Right();			// 右方向。
		float m_viewAngle = CMath::DegToRad(60.0f);		//画角。
		float m_aspect = 1.f;							//アス比
		float m_far = 10000.0f;							//遠い平面までの距離。
		float m_near = 1.0f;							//近平面までの距離。

		float m_width = 1280.0f;						//横
		float m_height = 720.0f;						//縦

		ECameraMode m_cameraMode = ECameraMode::ePerth;	//カメラモード
	};

	//extern Camera g_camera3D;		//!<3Dカメラ。
}