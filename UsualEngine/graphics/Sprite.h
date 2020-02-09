#pragma once

namespace UsualEngine
{
	class Sprite
	{
	public:
		Sprite();
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
		ID3D11ShaderResourceView* m_textuer = nullptr;		//�摜
		Shader m_vs;																//���_�V�F�[�_
		Shader m_ps;																//�s�N�Z���V�F�[�_

		Primitive m_primitive;													//�v���~�e�B�u
		ConstantBuffer m_constBuffer;													//�萔�o�b�t�@
		CMatrix mWorld = CMatrix::Identity();							//���E�s��
		CVector4 mMulCol = CVector4::White();						//��Z�F
		CVector2 mSize = { 0,0 };											//sprite�̂�����
	};
}