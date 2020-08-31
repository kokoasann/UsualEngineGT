/*!
 * @brief	コリジョン属性
 */
#pragma once

namespace UsualEngine
{
	/*!
	 * @brief	コリジョン属性の大分類。
	 */
	enum EnCollisionAttr {
		enCollisionAttr_Ground = 1,
		enCollisionAttr_Character = 2,
		enCollisionAttr_NonHitIK = 4,
		enCOllisionAttr_IK = 8,
		enCollisionAttr_User = 16
	};
}