/*!
 * @brief	コライダー。
 */

#pragma once

class btCollisionShape;
namespace UsualEngine
{
	/*!
	 * @brief	コライダーのインターフェースクラス。
	 */
	class ICollider {
	public:
		virtual btCollisionShape* GetBody() const = 0;
	};
}