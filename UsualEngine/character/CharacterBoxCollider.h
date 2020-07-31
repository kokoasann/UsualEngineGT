#pragma once
#include "physics/BoxCollider.h"

namespace UsualEngine
{
	/// <summary>
	/// blender�Őݒ肵���ꏊ��box collider��ݒu����
	/// (�X�P���g���L��̕��Ɍ���)
	/// </summary>
	class CharacterBoxCollider:public GameObject
	{
	public:
		using SettingTagFunc = std::function<int(const char* tag)>;
		/// <summary>
		/// �֌W����{�[���ƃ{�b�N�X�R���C�_�[�̍\���́B
		/// </summary>
		struct RelationBB
		{
			Bone* bone;			//��������{�[��
			BoxCollider box;	//�R���C�_�[
			RigidBody rigid;	//���́B
			CMatrix offsetmat = CMatrix::Identity();	//�I�t�Z�b�g
		};
		//�R���X�g���N�^
		CharacterBoxCollider() {};
		//�f�X�g���N�^
		~CharacterBoxCollider() {};
		/// <summary>
		/// �J���B
		/// </summary>
		virtual void Release() override;
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual void OnDestroy() override;

		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="path">�t�@�C���p�X</param>
		/// <param name="skeleton">�X�P���g��</param>
		/// <param name="scale">���f���̊g�嗦</param>
		/// <param name="func">���̂Ƀ^�O��t���邽�߂̊֐�</param>
		void Init(const wchar_t* path, Skeleton* skeleton, float scale, SettingTagFunc func = [](auto tag) {return enCollisionAttr_NonHitIK;});
		/// <summary>
		/// �X�^�[�g
		/// </summary>
		/// <returns></returns>
		bool Start() override;
		/// <summary>
		/// �X�V�B
		/// </summary>
		void Update() override;

	private:
		std::vector<RelationBB> m_relationBBList;		//�֌W����{�[���ƃ{�b�N�X�R���C�_�[�̍\���̂̃��X�g
	};
}