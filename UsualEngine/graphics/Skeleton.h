/*!
 *@brief	�X�P���g��
 */
#pragma once

namespace UsualEngine
{
	/*!
	*@brief	�{�[���B
	*/
	class Bone {
	public:
		/*!
		*@brief	�R���X�g���N�^�B
		*@param[in]	boneName		�{�[���̖��O�B
		*@param[in]	bindPose		�o�C���h�|�[�Y�̍s��B
		*@param[in] invBindPose		�o�C���h�|�[�Y�̋t�s��B
		*@param[in]	parentId		�e�̃{�[���ԍ��B
		*@param[in] boneId			�{�[���ԍ��B
		*/
		Bone(
			const wchar_t* boneName,
			const CMatrix& bindPose,
			const CMatrix& invBindPose,
			int parentId,
			int boneId
		) :
			m_boneName(boneName),
			m_bindPose(bindPose),
			m_invBindPose(invBindPose),
			m_parentId(parentId),
			m_worldMatrix(bindPose),
			m_boneId(boneId)
		{
		}
		/*!
		 *@brief	���[�J���s���ݒ�
		 */
		void SetLocalMatrix(const CMatrix& m)
		{
			m_localMatrix = m;
		}
		/*!
		 *@brief	���[�J���s����擾�B
		 */
		const CMatrix& GetLocalMatrix() const
		{
			return m_localMatrix;
		}
		/*!
		*@brief	���[���h�s���ݒ�B
		*/
		void SetWorldMatrix(const CMatrix& m)
		{
			m_worldMatrix = m;
		}
		/*!
		 *@brief	���[���h�s����擾�B
		 */
		const CMatrix& GetWorldMatrix() const
		{
			return m_worldMatrix;
		}
		/*!
		*@brief	�o�C���h�|�[�Y�̍s����擾�B
		*/
		const CMatrix& GetBindPoseMatrix() const
		{
			return m_bindPose;
		}
		/*!
		 *@brief	�o�C���h�|�[�Y�̋t�s����擾�B
		 */
		const CMatrix& GetInvBindPoseMatrix() const
		{
			return m_invBindPose;
		}
		/*!
		 *@brief	�e�̃{�[���s����擾�B
		 */
		int GetParentId() const
		{
			return m_parentId;
		}
		/*!
		 *@brief	�{�[���ԍ��̎擾�B
		 */
		int GetNo() const
		{
			return m_boneId;
		}

		/*!
		*@brief	�q����ǉ��B
		*/
		void AddChild(Bone* bone)
		{
			m_children.push_back(bone);
			bone->SetParent(this);
		}
		/*!
		*@brief	�q�����擾�B
		*/
		const std::vector<Bone*>& GetChildren() const
		{
			return m_children;
		}

		void SetParent(Bone* chil)
		{
			m_parent = chil;
		}
		Bone* GetParent() const
		{
			return m_parent;
		}

		/*!
		 *@brief	���O�̎擾�B
		 */
		const wchar_t* GetName() const
		{
			return m_boneName.c_str();
		}

		bool IsONGround() const
		{
			return m_isONGround;
		}
		void SetIsONGround(bool b)
		{
			m_isONGround = b;
		}


		void SetMove(CVector3 v)
		{
			m_move = v;
		}
		CVector3 GetMove() const
		{
			return m_move;
		}
		void SetAnimMove(CVector3 v)
		{
			m_animMove = v;
		}
		CVector3 GetAnimMove() const
		{
			return m_animMove;
		}

		void SetBaseWorldMatrix(const CMatrix& mat)
		{
			m_oldWorldMatrix = m_baseWorldMatrix;
			m_baseWorldMatrix = mat;
			
		}
		const CMatrix& GetBaseWorldMatrix() const
		{
			return m_baseWorldMatrix;
		}

		void SetOldWorldMatrix(const CMatrix& mat)
		{
			m_oldWorldMatrix = mat;
		}
		const CMatrix& GetOldWorldMatrix() const
		{
			return m_oldWorldMatrix;
		}

		void SetMomentum(const CVector3& v)
		{
			m_momentum = v;
		}
		const CVector3& GetMomentum() const
		{
			return m_momentum;
		}

		/*!
		*@brief	���̃{�[���̃��[���h��Ԃł̈ʒu�Ɖ�]�ƃX�P�[�����v�Z����B
		*@param[out]	trans		���s�ړ��ʂ̊i�[��B
		*@param[out]	rot			��]�ʂ̊i�[��B
		*@param[out]	scale		�g�嗦�̊i�[��B
		*/
		void CalcWorldTRS(CVector3& trans, CQuaternion& rot, CVector3& scale);

	private:

		std::wstring	m_boneName;
		int				m_parentId = -1;	//!<�e�̃{�[���ԍ��B
		int				m_boneId = -1;		//!<�{�[���ԍ��B
		CMatrix			m_bindPose;		//!<�o�C���h�|�[�Y�B
		CMatrix			m_invBindPose;	//!<�o�C���h�|�[�Y�̋t�s��B
		CMatrix			m_localMatrix = CMatrix::Identity();	//!<���[�J���s��B
		CMatrix			m_worldMatrix = CMatrix::Identity();	//!<���[���h�s��B
		CVector3		m_positoin = CVector3::Zero();			//!<���̃{�[���̃��[���h��Ԃł̈ʒu�B�Ō��CalcWorldTRS�����s�����Ƃ��̌��ʂ��i�[����Ă���B
		CVector3		m_scale = CVector3::One();				//!<���̃{�[���̊g�嗦�B�Ō��CalcWorldTRS�����s�����Ƃ��̌��ʂ��i�[����Ă���B
		CQuaternion		m_rotation = CQuaternion::Identity();	//!<���̃{�[���̉�]�B�Ō��CalcWorldTRS�����s�����Ƃ��̌��ʂ��i�[����Ă���B
		std::vector<Bone*>	m_children;		//!<�q���B
		Bone* m_parent = nullptr;			//�e�{�[��

		CMatrix			m_baseWorldMatrix = CMatrix::Identity();
		CMatrix			m_oldWorldMatrix = CMatrix::Identity();

		CVector3 m_animMove = CVector3::Zero();
		CVector3 m_move = CVector3::Zero();			//�{�[���̈ړ��x�N�g��
		CVector3 m_momentum = CVector3::Zero();		//�����B
		bool m_isONGround = false;
	};
	/*!
	 *@brief	�X�P���g���B
	 */
	class Skeleton {
	public:
		using OnPostProcessSkeletonUpdate = std::function<void()>;

		Skeleton();
		~Skeleton();

		void Release();

		/*!
		 *@brief	�{�[���̃��[�J���s���ݒ�B
		 *@param[in]	boneNo		�{�[���ԍ�
		 *@param[in]	m			�s��B
		 */
		void SetBoneLocalMatrix(int boneNo, const CMatrix& m)
		{
			if (boneNo > (int)m_bones.size() - 1) {
				return;
			}
			m_bones[boneNo]->SetLocalMatrix(m);
		}
		/*!
		 *@brief	�{�[���̐����擾�B
		 */
		int GetNumBones() const
		{
			return static_cast<int>(m_bones.size());
		}
		/*!
		*@brief	���[�h�B
		*@param[in]	filePath	�t�@�C���p�X�B
		*/
		bool Load(const wchar_t* filePath);
		/*!
		 *@brief	�{�[���̖��O����{�[��ID�������B
		 */
		int FindBoneID(const wchar_t* boneName) const
		{
			for (int i = 0; i < (int)m_bones.size(); i++) {
				if (wcscmp(m_bones[i]->GetName(), boneName) == 0) {
					return i;
				}
			}
			//������Ȃ������B
			return -1;
		}
		/*!
		*@brief	�{�[�����擾�B
		*/
		Bone* GetBone(int boneNo)
		{
			return m_bones[boneNo];
		}
		/// <summary>
		/// �S�{�[�����擾�B
		/// </summary>
		/// <returns></returns>
		std::vector<Bone*>& GetAllBone()
		{
			return m_bones;
		}
		/*!
		*@brief	�{�[���s��̔z���GPU�ɓ]���B
		*/
		void SendBoneMatrixArrayToGPU();
	public:
		/*!
		*@brief	�{�[���s��̃X�g���N�`���[�h�o�b�t�@���������B
		*/
		void InitBoneMatrixArrayStructuredBuffer();
		/*!
		*@brief	�{�[���s��̃V�F�[�_�[���\�[�X�r���[���������B
		*/
		void InitBoneMatrixArrayShaderResourceView();
		/*!
		 *@brief	�X�V�B
		 */
		void Update(const CMatrix& mWorld);

		void UpdateBase(const CMatrix& mWorld);
		/*!
		*@brief	�{�[���̃��[���h�s��̍X�V�֐��B
		*@details
		* �ʏ�̓��[�U�[�����̊֐����Ăяo���K�v�͂���܂���B
		*@param[in]	bone		�X�V����{�[���B
		*@param[in]	parentMatrix	�e�̃{�[���̃��[���h�s��B
		*/
		void UpdateBoneWorldMatrix(Bone& bone, const CMatrix& parentMatrix);

		void UpdateBoneBaseWorldMatrix(Bone& bone, const CMatrix& parentMatrix);
	private:

		std::vector<Bone*>			m_bones;					//!<�{�[���̔z��B
		std::vector<CMatrix>		m_boneMatrixs;				//!<�{�[���s��B
		ID3D11Buffer* m_boneMatrixSB = nullptr;	//!<�{�[���s��̃X�g���N�`���[�o�b�t�@�B
		ID3D11ShaderResourceView* m_boneMatrixSRV = nullptr;	//!<�{�[���s���SRV�B

		CMatrix m_worldMatrix = CMatrix::Identity();
		CMatrix m_oldWorldMatrix = CMatrix::Identity();
	};
}