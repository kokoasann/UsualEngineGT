#pragma once

class MortarShell:public ue::GameObject
{
public:
	MortarShell(){}
	~MortarShell();
	/// <summary>
	/// �J���B
	/// </summary>
	void Release() override{}
	/// <summary>
	/// DeleteGO���ꂽ�u�ԌĂ΂��
	/// </summary>
	void OnDestroy() override;
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="pos">��C�̃|�W�V����</param>
	/// <param name="dir">�����Ă������</param>
	void Init(ue::CVector3 pos,ue::CVector3 dir);
	/// <summary>
	/// Start
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;

private:
	ue::SkinModelRender* m_model = nullptr;						//���f��
	ue::CVector3 m_pos = ue::CVector3::Zero();					//�|�W�V����
	ue::CVector3 m_dir = ue::CVector3::Zero();						//�����Ɨ͂����˔������x�N�g��
	ue::CVector3 m_oldDir = ue::CVector3::Zero();				//�Â�����(�g��Ȃ�(����))
	ue::CQuaternion m_rot = ue::CQuaternion::Identity();		//��]�B

	float m_power = 8000.f;// / 50;										//������
	float m_gravity = 9.8f * 350;// / 50;									//�d�́B
	float m_time = 0.0f;															//���ԁB(�g��Ȃ�(����))
	ue::SphereCollider m_collider;											//�R���C�_�[
};