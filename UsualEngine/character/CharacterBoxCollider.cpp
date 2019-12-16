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
	void CharacterBoxCollider::Init(const wchar_t* path,Skeleton* ske,float scale, bool isRigidBody)
	{
		FILE* file = _wfopen(path, L"rb");
		if (!file)
			return;
		int count = 0;
		fread(&count, sizeof(count), 1, file);

		if(isRigidBody)
			m_rigids.resize(count);
		m_boxs.resize(count);
		for (int i = 0; i < count; i++)
		{
			int len = 0;
			fread(&len, sizeof(len), 1, file);
			char name[255] = { 0 };
			fread(name, sizeof(char), len+1, file);
			CVector3 vec[4];
			fread(vec, sizeof(CVector3), 4, file);

			CMatrix mat;
			memcpy(mat.m[0], &vec[0], sizeof(CVector3));
			memcpy(mat.m[1], &vec[1], sizeof(CVector3));
			memcpy(mat.m[2], &vec[2], sizeof(CVector3));
			memcpy(mat.m[3], &vec[3], sizeof(CVector3));
			
			//マルチバイト文字列をワイド文字列に変換する。
			wchar_t boneName[256];
			mbstowcs(boneName, name, 256);
			int id = ske->FindBoneID(boneName);
			auto bone = ske->GetBone(id);

			CVector3 pos, sca;
			CQuaternion rot;
			mat.CalcMatrixDecompose(pos, rot, sca);

			BoxCollider box;
			box.Create(sca * scale);
			if (isRigidBody)
			{
				RigidBodyInfo info;
				info.collider = &box;

				m_rigids[i].Create(info);
			}
		}
	}
}