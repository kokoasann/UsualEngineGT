#pragma once


namespace UsualEngine
{
	class PostEffect;
	/// <summary>
	/// �|���ŉ_��`��B
	///	���C�}�[�`���O���g���ׂ�����Əd�������B
	/// </summary>
	class CloudPlane
	{
	public:
		CloudPlane();
		~CloudPlane();

		/// <summary>
		/// �������B
		/// </summary>
		void Init();
		/// <summary>
		/// �J���B
		/// </summary>
		void Release();

		/// <summary>
		/// �`��B
		/// </summary>
		void Render(PostEffect* pe);
	private:
		/// <summary>
		/// �萔�o�b�t�@�̍\���́B
		/// </summary>
		struct CBData
		{
			CVector3 campos;	//�J�����̈ʒu�B
			//CVector3 camdir;
			CVector3 mainLightDir;	//���C�����C�g�̌���
		};
		ConstantBuffer m_cBuffer;	//�萔�o�b�t�@
		CBData m_cbData;			//�萔�o�b�t�@�̃f�[�^
		ConstantBuffer m_vscBuffer;	//vs�p�̒萔�o�b�t�@
		Shader m_vs;				//���_�V�F�[�_
		Shader m_ps;				//�s�N�Z���V�F�[�_
		Primitive m_prim;	//�`�悷��B

		RenderTarget m_rt;

		CVector3 m_pos = {0.f,5000.f,0.f};					//�|�W�V����
		CVector3 m_scale = CVector3::One()*50000.f;				//�X�P�[��
		CQuaternion m_rot = CQuaternion::Identity();	//��]�B
	};
}