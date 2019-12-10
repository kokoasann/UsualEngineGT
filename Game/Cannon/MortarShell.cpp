#include "stdafx.h"
#include "MortarShell.h"

MortarShell::~MortarShell()
{
	ue::DeleteGO(m_model);
}

void MortarShell::OnDestroy()
{
	m_model->SetActive(false);
}

bool MortarShell::Start()
{
	m_model = ue::NewGO<ue::SkinModelRender>(0);
	m_model->Init(L"Assets/model/Cannon/MortarShell.cmo", 0, 0, ue::enFbxUpAxisY);

	return true;
}

void MortarShell::Update()
{
}
