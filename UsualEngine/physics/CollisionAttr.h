/*!
 * @brief	コリジョン属性
 */
#pragma once

/*!
 * @brief	コリジョン属性の大分類。
 */
enum EnCollisionAttr{
	enCollisionAttr_Ground = 1,
	enCollisionAttr_Character = 2,
    enCollisionAttr_NonHitIK = 4,
	enCollisionAttr_User = 8
};