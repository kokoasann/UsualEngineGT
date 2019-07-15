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
	Sprite::Sprite()
	{
		mVS.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
		mPS.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);
	}
	void Sprite::Init(const wchar_t* path, CVector2 size)
	{
		std::wstring st = path;
		mTextuer = SpriteDataManager::Get()->Load(st);

		mSize = size;
		CVector2 hsize = size/2;
		SimpleVertex topo[] =
		{
			{
				CVector4(-hsize.x,-hsize.y,0.0f,1.0f),
				CVector2(0.0f,1.0f)
			},
			{
				CVector4(hsize.x, -hsize.y, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f)
			},
			{
				CVector4(-hsize.x, hsize.y, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(hsize.x, hsize.y, 0.0f, 1.0f),
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
	void Sprite::Draw(CMatrix viewMatrix, CMatrix projMatrix)
	{

		CBData cb;
		cb.WVP = mWorld;
		cb.WVP *= viewMatrix;
		cb.WVP *= projMatrix;
		cb.MulCol = mMulCol;

		ID3D11DeviceContext* dev = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		dev->UpdateSubresource(mCB.GetBody(), 0, NULL, &cb, 0, 0);
		dev->VSSetConstantBuffers(0, 1, &mCB.GetBody());
		dev->PSSetConstantBuffers(0, 1, &mCB.GetBody());
		dev->PSSetShaderResources(0, 1, &mTextuer);
		dev->IASetInputLayout(mVS.GetInputLayout());
		dev->VSSetShader((ID3D11VertexShader*)mVS.GetBody(), NULL, 0);
		dev->PSSetShader((ID3D11PixelShader*)mPS.GetBody(), NULL, 0);

		mPrimitive.Draw();
	}
}