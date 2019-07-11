/*!
 * @brief	ベクトルクラス。
 */

#include "PreCompile.h"
#include "Vector.h"
#include "Matrix.h"



namespace UsualEngine
{
	/*!
	*@brief	行列からクォータニオンを作成。
	*/
	void CQuaternion::SetRotation(const CMatrix& m)
	{
		DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
	}

}