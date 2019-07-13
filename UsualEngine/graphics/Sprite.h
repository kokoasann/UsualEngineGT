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
		ID3D11ShaderResourceView* mTextuer = nullptr;		//‰æ‘œ
		CMatrix mWorld = CMatrix::Identity();							//¢ŠEs—ñ
		CVector4 mMulCol = CVector4::White();						//æZF
	};
}