#pragma once


namespace UsualEngine
{
	class SkinModelDummy :public GameObject
	{
	public:

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update() override;

		/// <summary>
		/// �������֐��B
		/// </summary>
		/// <param name="path">���f���̃t�@�C���p�X</param>
		/// <param name="anims">�A�j���[�V�����N���b�v</param>
		/// <param name="animCount">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="axis">���f���̎�</param>
		void Init(const wchar_t* path, EnFbxUpAxis axis = enFbxUpAxisZ);

		/// <summary>
		/// �|�W�V�������Z�b�g����
		/// </summary>
		/// <param name="pos"></param>
		void SetPos(const CVector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// �|�W�V������Ԃ�
		/// </summary>
		/// <returns></returns>
		const CVector3& GetPos() const
		{
			return m_position;
		}
		/// <summary>
		/// ��]���Z�b�g����
		/// </summary>
		/// <param name="rot"></param>
		void SetRot(const CQuaternion& rot)
		{
			m_rotation = rot;
		}
		/// <summary>
		/// ��]��Ԃ�
		/// </summary>
		/// <returns></returns>
		const CQuaternion& GetRot() const
		{
			return m_rotation;
		}
		/// <summary>
		/// �X�P�[�����Z�b�g����
		/// </summary>
		/// <param name="sca"></param>
		void SetSca(const CVector3& sca)
		{
			m_scale = sca;
		}
		/// <summary>
		/// �X�P�[����Ԃ�
		/// </summary>
		/// <returns></returns>
		const CVector3& GetSca() const
		{
			return m_scale;
		}

		/// <summary>
		/// �V���h�E���o�����ǂ���
		/// </summary>
		/// <param name="b">�o���ꍇtrue������</param>
		void SetIsShadowCaster(bool b)
		{
			m_skinModel.SetIsShadowCaster(b);
		}
		/// <summary>
		/// �V���h�E�o���H
		/// </summary>
		/// <returns>�o���ꍇtrue���Ԃ��Ă���</returns>
		bool IsShadowCaster()
		{
			return m_skinModel.IsShadowCaster();
		}
		/// <summary>
		/// �V���h�E���󂯎�邩�ǂ���
		/// </summary>
		/// <param name="b">�󂯎��ꍇtrue������</param>
		void SetIsShadowReciever(bool b)
		{
			m_skinModel.SetIsShadowReciever(b);
		}
		/// <summary>
		/// �V���h�E���󂯎��H
		/// </summary>
		/// <returns>�󂯎��ꍇtrue���Ԃ��Ă���</returns>
		bool IsShadowReciever()
		{
			return m_skinModel.IsShadowReciever();
		}

		/// <summary>
		/// �X�L�����f����Ԃ�
		/// </summary>
		/// <returns></returns>
		SkinModel& GetSkinModel()
		{
			return m_skinModel;
		}
	private:
		SkinModel m_skinModel;	//siknModel

		CVector3 m_position = CVector3::Zero();					//�|�W�V����
		CQuaternion m_rotation = CQuaternion::Identity();		//��].
		CVector3 m_scale = CVector3::One();						//�X�P�[��
	};
}