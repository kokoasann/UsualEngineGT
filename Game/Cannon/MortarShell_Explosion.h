#pragma once


class MortarShell_Explosion :public ue::GameObject
{
public:
	
	/// <summary>
	/// 
	/// </summary>
	virtual void Release() override;
	/// <summary>
	/// 
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// 
	/// </summary>
	void Awake() override;
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 
	/// </summary>
	void Update() override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const ue::CVector3& pos)
	{
		m_light->SetPos({pos.x ,pos.y+1.f ,pos.z});
	}
private:
	ue::LightPoint* m_light=nullptr;
	float m_time = 0.f;
	float m_maxPowTime = 1.f;
	float m_endTime = 1.5f;

	ue::CVector4 m_minColor = { 0.f ,0.f ,0. ,0.f };
	float m_minDecay = 0.f;
	float m_minRadius = 0.1f;

	ue::CVector4 m_maxColor = { 1.f ,0.5f ,0.2f ,1.f };
	float m_maxDecay = 0.00f;
	float m_maxRadius = 600.f;
};