#pragma once



namespace UsualEngine
{
	struct SDirectionLight
	{
		CVector3 dir = { 0,-1,0 };
		CVector4 color = CVector4::White();
	};
	struct SPointLight
	{
		CVector3 pos = CVector3::Zero();
		CVector3 posInView = CVector3::Zero();
		CVector4 color = CVector4::White();
		float decay = 0.0;
		float radius = 10.f;
	};
}