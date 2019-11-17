#pragma once


namespace UsualEngine
{
	class SkinModelRender :public GameObject
	{
	public:
		/// <summary>
		/// IK��p���čs���ړ��̏����̊֐��^�B
		/// </summary>
		using MoveFunc = std::function<void(CVector3& pos)>;
		/// <summary>
		/// IK��p���čs����]�̏����̊֐��^�B
		/// </summary>
		using RotateFunc = std::function<void(CQuaternion& rot)>;

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update() override;
		/// <summary>
		/// �`�悷��
		/// </summary>
		void Render() override;
		
		/// <summary>
		/// �������֐��B
		/// </summary>
		/// <param name="path">���f���̃t�@�C���p�X</param>
		/// <param name="anims">�A�j���[�V�����N���b�v</param>
		/// <param name="animCount">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="axis">���f���̎�</param>
		void Init(const wchar_t* path, AnimationClip* anims = nullptr, int animCount = 0, EnFbxUpAxis axis = enFbxUpAxisZ);
		
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
		const CVector3 GetPos() const
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
		const CVector3 GetSca() const
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

		/// <summary>
		/// animation���������B
		/// </summary>
		/// <returns>�ێ�����animation��Ԃ�</returns>
		Animation& GetAnimation()
		{
			return m_animation;
		}

		/// <summary>
		/// IK�̐ݒ������B
		/// </summary>
		/// <param name="effector">�G�t�F�N�^�{�[��</param>
		/// <param name="end">�G���h�{�[��</param>
		/// <param name="radius">�R���C�_�[�̔��a</param>
		void SetingIK(Bone* effector, Bone* end, float radius)
		{
			m_animation.SetingIK(effector, end, radius, m_position);
		}

		/// <summary>
		/// IK��p���čs���ړ��̏����֐��̃Z�b�g
		/// </summary>
		/// <param name="mf">[](ue::CVector3& pos)->void</param>
		void SetMoveFunc(MoveFunc mf)
		{
			m_moveFunc = mf;
			m_isUseMoveFunc = true;
		}
		/// <summary>
		/// IK��p���čs����]�̏����֐��̃Z�b�g
		/// </summary>
		/// <param name="rf">[](ue::CQuaternion& rot)->void</param>
		void SetRotateFunc(RotateFunc rf)
		{
			m_rotateFunc = rf;
			m_isUseRotateFunc = true;
		}
	private:
		SkinModel m_skinModel;	//siknModel
		Animation m_animation;	//Animation

		CVector3 m_position = CVector3::Zero();					//�|�W�V����
		CQuaternion m_rotation = CQuaternion::Identity();	//��].
		CVector3 m_scale = CVector3::One();							//�X�P�[��

		bool m_isRenderingOK = false; //�����_�����O���Ă��悩�ƁH

		//��IK�p�Ƃ������Ă邯�ǁA�ׂɂ���Ȃ��Ƃ͂Ȃ��B
		MoveFunc m_moveFunc;					//IK�p�̈ړ��֐��B
		bool m_isUseMoveFunc=false;		//�ړ��֐����g�����H
		RotateFunc m_rotateFunc;				//IK�p�̉�]�֐��B
		bool m_isUseRotateFunc = false;		//��]�֐����g�����H
	};
}