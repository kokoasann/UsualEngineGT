/*!
 * @brief	�x�N�g���N���X�B
 */

#include "PreCompile.h"
#include "Vector.h"
#include "Matrix.h"



namespace UsualEngine
{
	/*!
	*@brief	�s�񂩂�N�H�[�^�j�I�����쐬�B
	*/
	void CQuaternion::SetRotation(const CMatrix& m)
	{
		DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
	}

}