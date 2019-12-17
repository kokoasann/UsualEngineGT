#include "PreCompile.h"
#include "CharacterBoxCollider.h"

namespace UsualEngine
{
	void CharacterBoxCollider::Release()
	{
	}
	void CharacterBoxCollider::OnDestroy()
	{
	}
	void CharacterBoxCollider::Init(const wchar_t* path,Skeleton* ske,float scale, SettingTagFunc func)
	{
		FILE* file = _wfopen(path, L"rb");
		if (!file)
			return;
		int count = 0;
		fread(&count, sizeof(count), 1, file);

		m_rigids.resize(count);
		m_boxs.resize(count);
		m_relationBBList.resize(count);
		for (int i = 0; i < count; i++)
		{
			int len = 0;
			fread(&len, sizeof(len), 1, file);
			char name[255] = { 0 };
			fread(name, sizeof(char), len+1, file);
			char tagname[32] = { 0 };
			fread(&len, sizeof(len), 1, file);
			fread(&tagname, sizeof(char), len + 1, file);
			int tag = func(tagname);
			CVector3 vec[4];
			fread(vec, sizeof(CVector3), 4, file);


			memcpy(m_relationBBList[i].offsetmat.m[0], &vec[0], sizeof(CVector3));
			memcpy(m_relationBBList[i].offsetmat.m[1], &vec[1], sizeof(CVector3));
			memcpy(m_relationBBList[i].offsetmat.m[2], &vec[2], sizeof(CVector3));
			memcpy(m_relationBBList[i].offsetmat.m[3], &vec[3], sizeof(CVector3));
			
			//マルチバイト文字列をワイド文字列に変換する。
			wchar_t boneName[256];
			mbstowcs(boneName, name, 256);
			int id = ske->FindBoneID(boneName);
			auto bone = ske->GetBone(id);
			m_relationBBList[i].bone = bone;

			CVector3 pos, sca;
			CQuaternion rot;
			m_relationBBList[i].offsetmat.CalcMatrixDecompose(pos, rot, sca);

			/*CMatrix tram, rotm,scam;
			tram.MakeTranslation(pos*100);
			rotm.MakeRotationFromQuaternion(rot);
			scam.MakeScaling(CVector3::One()*100);*/
			//rotm.Mul(scam, rotm);
			//m_relationBBList[i].offsetmat.Mul(rotm, tram);

			m_relationBBList[i].box.Create(sca*scale*2);
			auto wmat = bone->GetBindPoseMatrix();
			auto mat = m_relationBBList[i].offsetmat*wmat;
			mat.CalcMatrixDecompose(pos, rot, sca);

			RigidBodyInfo info;
			info.collider = &m_relationBBList[i].box;
			info.mass = 0;
			info.pos = pos;
			info.rot = rot;

			m_rigids[i].Create(info);
			m_relationBBList[i].rigid = &m_rigids[i];
			Physics().AddRigidBody(*m_relationBBList[i].rigid);

			auto rg = m_rigids[i].GetBody();
			rg->setUserIndex(tag);
		}
		fclose(file);
	}
	bool CharacterBoxCollider::Start()
	{
		return true;
	}
	void CharacterBoxCollider::Update()
	{
		//return;
		for (auto& rbb : m_relationBBList)
		{
			CVector3 pos, sca;
			CQuaternion rot;
			auto wmat = rbb.bone->GetWorldMatrix();
			CMatrix mat,tm,rm;
			/*wmat.CalcMatrixDecompose(pos, rot, sca);
			rm.MakeRotationFromQuaternion(rot);
			tm.MakeTranslation(pos);*/
			//wmat.Mul(rm, tm);

			mat.Mul(rbb.offsetmat,wmat);
			mat.CalcMatrixDecompose(pos, rot, sca);
			//pos *= 10;
			auto rg = rbb.rigid->GetBody();
			auto& tr = rg->getWorldTransform();
			tr.setOrigin({ pos.x,pos.y ,pos.z });
			tr.setRotation({ rot.x,rot.y ,rot.z,rot.w });
			
		}
	}
}