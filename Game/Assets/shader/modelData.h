
struct SDirectionLight
{
	float3 dir;			//�f�B���N�V����
	float4 color;		//�F
};

StructuredBuffer<SDirectionLight> DirLights : register(t100);

cbuffer LightCB : register(b1)
{
	float3 eyepos : packoffset(c0);			//���_
	int DLcount : packoffset(c0.w);			//�f�B���N�V�������C�g�̐�
	float4 screenSize : packoffset(c1);		//�X�N���[���̃T�C�Y
}