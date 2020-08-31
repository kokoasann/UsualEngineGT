#pragma once
#include "physics/MeshCollider.h"

class Cannon :public ue::GameObject
{
public:
	Cannon(){}
	~Cannon() {}
	/// <summary>
	/// �J���B
	/// </summary>
	void Release() override;
	/// <summary>
	/// DeleteGO���ꂽ�u�ԌĂ΂��
	/// </summary>
	void OnDestroy() override;
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="cannonMesh">�R���C�_�[�p�̃��f��</param>
	/// <param name="pos">�|�W�V����</param>
	/// <param name="rot">��]</param>
	void Init(const ue::SkinModel& cannonMesh,const ue::CVector3& pos,const ue::CQuaternion& rot);
	/// <summary>
	/// �X�^�[�g
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// ���W�b�h�{�f�B�[�̍X�V�B
	/// </summary>
	void Update_RigidBody();
	/// <summary>
	/// ����!!
	/// </summary>
	void Fire();

	/// <summary>
	/// ������L���m���̕ҏW�����܂��錾�B
	/// </summary>
	void DirEdit()
	{
		m_isDirEdit = true;
	}
	/// <summary>
	/// �ҏW���I���܂��錾�B
	/// </summary>
	void EndDirEdit()
	{
		m_isDirEdit = false;
	}

	/// <summary>
	/// �L���m���̃X�P���g�����Q�b�g
	/// </summary>
	/// <returns></returns>
	ue::Skeleton& GetSkeleton()
	{
		return m_cannon->GetSkinModel().GetSkeleton();
	}
	/// <summary>
	/// IK�𓮂������ǂ���
	/// </summary>
	/// <param name="b"></param>
	void SetIKActive(bool b)
	{
		m_cannon->GetAnimation().SetIKActive(b);
	}
	/// <summary>
	/// �|�W�V������Ԃ�
	/// </summary>
	/// <returns></returns>
	const ue::CVector3& GetPos() const
	{
		return m_pos;
	}
	/// <summary>
	/// ��]��Ԃ�
	/// </summary>
	/// <returns></returns>
	const ue::CQuaternion& GetRot() const
	{
		return m_rot;
	}
	/// <summary>
	/// �������Z�b�g
	/// </summary>
	/// <param name="dir"></param>
	void SetDir(const ue::CVector3& dir)
	{
		m_dir = dir;
	}
	/// <summary>
	/// �^�[�Q�b�g���Z�b�g
	/// </summary>
	/// <param name="dir"></param>
	void SetTarget(const ue::CVector3& dir)
	{
		auto nonY = dir;
		nonY.y = 0;
		nonY.Normalize();
		float r = atan2(nonY.x, nonY.z);
		ue::CQuaternion rot;
		rot.SetRotation(ue::CVector3::Up(), r);
		m_cannon->SetRot(rot);
		m_stand->SetRot(rot);
		m_rot = rot;
		m_cannon->GetAnimation().SetIKNextTarget(m_dirBone->GetParent()->GetWorldMatrix().GetTranslation()+(dir*8.f), m_dirBone);
		m_dir = dir;
	}
	/// <summary>
	/// �����p�̃{�[�����Q�b�g
	/// </summary>
	/// <returns></returns>
	ue::Bone* GetDirBone()
	{
		return m_dirBone;
	}
private:
	ue::SkinModelRender* m_cannon = nullptr;		//�L���m���̃��f��
	ue::SkinModelRender* m_stand = nullptr;			//��̃��f��
	ue::MeshCollider m_meshCollider;				//���b�V���R���C�_�[
	ue::RigidBody m_rigidBody;						//���W�b�h�{�f�B

	ue::CVector3 m_dir = ue::CVector3::AxisZ();		//�����B
	ue::CVector3 m_pos = ue::CVector3::Zero();		//�|�W�V����
	ue::CQuaternion m_rot = ue::CQuaternion::Identity();	//��]�B
	ue::Bone* m_dirBone = nullptr;							//�����{�[��
	//ue::CQuaternion m_collisionRot = ue::CQuaternion::Identity();	//
	bool m_isDirEdit = false;		//�ҏW���H
	int m_isFirst = 0;			//���߂Ă��H
};