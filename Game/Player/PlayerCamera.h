#pragma once

class Player;
class Cannon;

class PlayerCamera :public ue::GameObject
{
public:
	enum CameraMode
	{
		CM_Normal,
		CM_Cannon,
	};

	//�R���X�g���N�^
	PlayerCamera();
	//�f�X�g���N�^
	void OnDestroy() override;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="p">�v���C���[�̃|�C���^</param>
	void Init(Player* p);

	//Start
	bool Start();
	//�X�V
	void Update();

	ue::Camera* GetCamera()
	{
		return m_camera;
	}

	void NormalMode();
	void CannonMode(Cannon* cn);
private:
	ue::Camera* m_camera = nullptr;					//�J�����̃|�C���^
	ue::CVector3 m_pos = ue::CVector3::Zero();		//position
	ue::CVector3 m_target = ue::CVector3::Zero();	//target
	float m_c2tLen = 0.f;							//�J�����̃|�W�V��������^�[�Q�b�g�̋����B
	float m_p2cLen = 0.f;							//�v���C���[����J�����̋����B
	ue::CVector3 m_p2c = ue::CVector3::Zero();		//�v���C���[����J�����̃x�N�g���B
	ue::CVector3 m_c2t = ue::CVector3::Zero();		//�J��������^�[�Q�b�g�̃x�N�g���B
	float m_offsetY = 100.f;						//�J�����̃^�[�Q�b�g�ƃ|�W�V������Y���ɂ������������B
	Player* m_player = nullptr;						//�v���C���[�̃|�C���^

	CameraMode m_cameraMode = CM_Normal;
	Cannon* m_cannon = nullptr;
	ue::Bone* m_cannonBone = nullptr;
	ue::CVector3 m_cannonDir = ue::CVector3::Zero();
};