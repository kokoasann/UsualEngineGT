#pragma once

namespace UsualEngine
{
	class Sprite
	{
	public:
		Sprite() {}
		~Sprite(){}

		void Init(const wchar_t* path, CVector2 size);

		void Update(CVector3 pos, CQuaternion rot, CVector3 sca, CVector2 pivot);

		void Draw(CMatrix viewMatrix, CMatrix projMatrix);

	private:
		struct CBData
		{
			CMatrix WVP = CMatrix::Identity();
			CVector4 MulCol = CVector4::White();
		};
		ID3D11ShaderResourceView* mTextuer = nullptr;		//画像
		Shader mVS;																//頂点シェーダ
		Shader mPS;																//ピクセルシェーダ

		Primitive mPrimitive;													//プリミティブ
		ConstantBuffer mCB;													//定数バッファ
		CMatrix mWorld = CMatrix::Identity();							//世界行列
		CVector4 mMulCol = CVector4::White();						//乗算色
		CVector2 mSize = { 0,0 };											//spriteのさいず
	};
}