#include "PreCompile.h"
#include "Sprite.h"

#include "SpriteDataManager.h"

namespace UsualEngine
{
	struct SimpleVertex
	{
		CVector4 pos;
		CVector2 tex;
	};
	void Sprite::Init(const wchar_t* path, CVector2 size)
	{
		std::wstring st = path;
		mTextuer = SpriteDataManager::Get()->Load(st);

		mSize = size;
		SimpleVertex topo[] =
		{
			{
				CVector4{-size.x,-size.y,0,1},
				CVector2{0,0}
			},
			{
				CVector4(size.x, -size.y, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f),
			},
			{
				CVector4(-size.x, size.y, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(size.x, size.y, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}
		};
		short index[] = { 0,1,2,3 };
		mPrimitive.Cteate(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, sizeof(SimpleVertex), topo, 4, IndexBuffer::e16bit, index);

		mCB.Create(nullptr, sizeof(CBData));

		//mVS.Load
	}
	void Sprite::Update(CVector3 pos, CQuaternion rot, CVector3 sca, CVector2 pivot)
	{
		//ピボットを考慮に入れた平行移動行列を作成。
		//ピボットは真ん中が0.0, 0.0、左上が-1.0f, -1.0、右下が1.0、1.0になるようにする。
		CVector2 localPivot = pivot;
		localPivot.x -= 0.5f;
		localPivot.y -= 0.5f;
		localPivot.x *= -2.0f;
		localPivot.y *= -2.0f;
		//画像のハーフサイズを求める。
		CVector2 halfSize = mSize;
		halfSize.x *= 0.5f;
		halfSize.y *= 0.5f;
		CMatrix mPivotTrans;

		mPivotTrans.MakeTranslation(
			{ halfSize.x * localPivot.x, halfSize.y * localPivot.y, 0.0f }
		);
		CMatrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(sca);
		mWorld = mPivotTrans * mScale;
		mWorld = mWorld * mRot;
		mWorld = mWorld * mTrans;
	}
}