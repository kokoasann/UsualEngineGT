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
		ID3D11ShaderResourceView* mTextuer = nullptr;		//�摜
		CMatrix mWorld = CMatrix::Identity();							//���E�s��
		CVector4 mMulCol = CVector4::White();						//��Z�F
	};
}