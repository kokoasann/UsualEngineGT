/*!
 * @brief	カプセルコライダー。
 */

#include "PreCompile.h"
#include "Physics/CapsuleCollider.h"


namespace UsualEngine
{
	/*!
		* @brief	デストラクタ。
		*/
	CapsuleCollider::~CapsuleCollider()
	{
		delete shape;
	}
}
