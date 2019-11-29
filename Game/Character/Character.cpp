#include "stdafx.h"
#include "Character.h"

Character::Character()
{
}

Character::~Character()
{
	if(m_model)
		ue::DeleteGO(m_model);
}

void Character::Init(ue::SkinModelRender* smr, float ccradius, float ccheight, const ue::CVector3& offset)
{
	m_model = smr;
	m_characon.Init(ccradius, ccheight, smr->GetPos(),false);
	m_ccOffset = offset;

	m_defaultMoveFunc = [&](ue::CVector3 & pos)
	{
		if (m_actionMode != AM_Move)
			return;
		ue::CVector3 move = ue::CVector3::Zero();
		if (m_footR->IsONGround())
		{
			move += m_footR->GetMove();
		}
		if (m_footL->IsONGround())
		{
			move += m_footL->GetMove();
		}

		if (move.Length() > 0.0001f)
		{
			move.y = 0;
			move *= -1;
			auto rpos = m_characon.Execute(1, move);
			rpos += m_ccOffset;
			pos = rpos;
			m_momentum += move;
		}
	};
	m_defaultRotateFunc = [&](auto & rot)
	{
		if (m_actionMode != AM_Rotate)
			return;

		if (m_footR->IsONGround())
		{
			auto mold = m_waistR->GetOldWorldMatrix();
			auto mnew = m_waistR->GetBaseWorldMatrix();
			//分改！！
			ue::CQuaternion rold, rnew;
			auto pos = ue::CVector3::Zero(), sca = ue::CVector3::Zero();
			mold.CalcMatrixDecompose(pos, rold, sca);
			mnew.CalcMatrixDecompose(pos, rnew, sca);

			//バインディングポーズの
			ue::CQuaternion rloc;
			auto bmat = m_waistR->GetBindPoseMatrix();
			bmat.CalcMatrixDecompose(pos, rloc, sca);
			ue::CVector3 axis = ue::CVector3::AxisZ();
			rloc.Multiply(axis);
			ue::CVector3 vold = axis, vnew = axis;
			rold.Multiply(vold);
			rnew.Multiply(vnew);

			vold.y = 0;
			vnew.y = 0;

			vold.Normalize();
			vnew.Normalize();

			float rad = vold.Dot(vnew);
			rad = min(rad, 1);
			rad = acos(rad);

			ue::CQuaternion add;
			add.SetRotation(ue::CVector3::AxisY(), rad * -1);


			auto fpos = m_waistR->GetWorldMatrix().GetTranslation();
			//fpos -= Rfoot->GetMove();
			auto mpos = m_model->GetPos();
			fpos.y = 0;
			mpos.y = 0;
			auto f2m = mpos - fpos;

			add.Multiply(f2m);
			auto npos = fpos + f2m;
			auto m2n = npos - mpos;

			auto rpos = m_characon.Execute(1, m2n);
			rpos += m_ccOffset;
			m_model->SetPos(rpos);

			auto rot = m_model->GetRot();
			rot.Multiply(add);
			m_model->SetRot(rot);
		}
	};

	m_model->SetMoveFunc(m_defaultMoveFunc);

	m_model->SetRotateFunc(m_defaultRotateFunc);
}

void Character::InitAnimData(ue::AnimationClip* animclip)
{
}

void Character::InitBoneData(wstr footLname, wstr footRname, wstr handLname, wstr handRname, wstr waistLname, wstr waistRname)
{
	for (auto bone : m_model->GetSkinModel().GetSkeleton().GetAllBone())
	{
		if (footLname!=nullptr&&lstrcmpW(bone->GetName(), footLname))
		{
			m_footL = bone;
		}
		else if (footRname != nullptr && lstrcmpW(bone->GetName(), footRname))
		{
			m_footR = bone;
		}
		else if (handLname != nullptr && lstrcmpW(bone->GetName(), handLname))
		{
			m_handL = bone;
		}
		else if (handRname != nullptr && lstrcmpW(bone->GetName(), handRname))
		{
			m_handR = bone;
		}
		else if (waistLname != nullptr && lstrcmpW(bone->GetName(), waistLname))
		{
			m_waistL = bone;
		}
		else if (waistRname != nullptr && lstrcmpW(bone->GetName(), waistRname))
		{
			m_waistR = bone;
		}
	}
}

ue::Bone* Character::FindBone(wstr name, BoneKind bk, bool isSetingIK, int len,float radius)
{
	ue::Bone* resbone = nullptr;
	bool isGet = false;
	for (auto bone : m_model->GetSkinModel().GetSkeleton().GetAllBone())
	{
		if (name != nullptr && lstrcmpW(bone->GetName(), name)==0)
		{
			resbone = bone;
			break;
		}
	}
	if (isSetingIK)
	{
		ue::Bone* endbone = resbone;
		for (int i = 0; i < len; i++)
		{
			endbone = endbone->GetParent();
		}
		m_model->SetingIK(resbone, endbone, radius);
	}

	switch (bk)
	{
	case BK_FootL:
		m_footL = resbone;
		break;
	case BK_FootR:
		m_footR = resbone;
		break;
	case BK_HandL:
		m_handL = resbone;
		break;
	case BK_HandR:
		m_handR = resbone;
		break;
	case BK_WaistL:
		m_waistL = resbone;
		break;
	case BK_WaistR:
		m_waistR = resbone;
		break;
	}
	return resbone;
}

void Character::SetBone(ue::Bone* bone, BoneKind bk, bool isSetingIK, int len, float radius)
{
	if (isSetingIK)
	{
		ue::Bone* endbone = bone;
		for (int i = 0; i < len; i++)
		{
			endbone = endbone->GetParent();
		}
		m_model->SetingIK(bone, endbone, radius);
	}
	switch (bk)
	{
	case BK_None:
		m_boneList.push_back(bone);
		break;
	case BK_FootL:
		m_footL = bone;
		break;
	case BK_FootR:
		m_footR = bone;
		break;
	case BK_HandL:
		m_handL = bone;
		break;
	case BK_HandR:
		m_handR = bone;
		break;
	case BK_WaistL:
		m_waistL = bone;
		break;
	case BK_WaistR:
		m_waistR = bone;
		break;
	}
}

void Character::Update()
{
	if ((m_footL && !m_footL->IsONGround()) || (m_footR && !m_footR->IsONGround()))
	{
		
	}
	auto grav = ue::CVector3(0, m_gravity, 0);
	m_move += grav;
	//m_move += m_momentum;
	auto mn = m_momentum;
	mn.Normalize();
	m_momentum -= m_momentum * 0.05f;
	//char st[255] = { 0 };
	//sprintf_s(st, "x:%f y:%f z:%f\n", m_momentum.x, m_momentum.y, m_momentum.z);
	//OutputDebugString(st);
	auto p = m_characon.Execute(ue::gameTime()->GetDeltaTime(), m_move);
	p += m_ccOffset;
	m_model->SetPos(p);
	m_move = ue::CVector3::Zero();

	auto rot = m_model->GetRot();
	rot.Multiply(m_rotate);
	m_model->SetRot(rot);
	m_rotate = ue::CQuaternion::Identity();
}

void Character::AddEventListener(const ue::Animation::EventListener& ev)
{
	m_model->GetAnimation().AddEventListener(ev);
}

void Character::ModeMove()
{
	m_actionMode = AM_Move;
}

void Character::ModeRotate()
{
	m_actionMode = AM_Rotate;
}

void Character::PlayAnim(int anim, float lerp, float start, ActionMode am)
{
	m_model->GetAnimation().Play(anim, lerp, start);
	m_actionMode = am;
}

